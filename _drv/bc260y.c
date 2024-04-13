//
// Created by dd21 on 2024/4/10.
//
#include "stdio.h"
#include "bc260y.h"
#include "drv_log.h"
#include "at.h"

/* 接收信号量 */
struct rt_semaphore sem_rx;

struct time bc260y_time;
/* 发送到服务器的数据 */
struct mqtt_data mdata;
char  bc260y_ip[16];

/* at device response */
at_response_t resp = RT_NULL;



rt_err_t bc260y_rxcb(rt_device_t dev, rt_size_t size){
    rt_sem_release(&sem_rx);
    return RT_EOK;
}

rt_err_t bc260y_uart_init(){
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;  /* 初始化配置参数 */

    /* 查找串口设备 */
    serial = rt_device_find(BC260Y_UART);
    if(serial ==RT_NULL){
        LOG_E("bc260y_uart_init failed...\n");
        return -RT_EINVAL;
    }
    /* 修改串口配置参数 */
    config.baud_rate = BC260Y_BAUD;        //修改波特率为 9600
    config.data_bits = DATA_BITS_8;           //数据位 8
    config.stop_bits = STOP_BITS_1;           //停止位 1
    config.bufsz     = 128;                   //修改缓冲区 buff size 为 128
    config.parity    = PARITY_NONE;           //无奇偶校验位

    /* 控制串口设备。通过控制接口传入命令控制字，与控制参数 */
    rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);

    /* 打开串口设备。以中断接收及轮询发送模式打开串口设备 */
    if(rt_device_open(serial, RT_DEVICE_FLAG_INT_RX|RT_DEVICE_OFLAG_RDWR) !=RT_EOK){
        LOG_E("bc260y rt_device_open failed...\n");
        return -RT_EINVAL;
    }
    return RT_EOK;
}

void entry_bc260y(){
    bc260y_uart_init();

    /* 创建sem然后在回调函数中使用 */
    rt_sem_init(&sem_rx,"rx_sem",0,RT_IPC_FLAG_FIFO);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(rt_device_find(BC260Y_UART),bc260y_rxcb);

    for (int i = 0; i < 10; ++i) {
        rt_device_write(rt_device_find(BC260Y_UART),0,testdata, rt_strlen(testdata));
    }

    while (1){
        char rx_buf;
        /* 如果接收到数据不是1(终端接收模式每次接收一个字符,一个字符触发一次) */
        while (rt_device_read(rt_device_find(BC260Y_UART),0,&rx_buf,1)!=1){
            /* 接收回调函数中释放该函数 */

            rt_sem_take(&sem_rx,RT_WAITING_FOREVER);
        }
        rt_kprintf("%c",rx_buf);
    }
}

int bc26_device_register(){
    static rt_thread_t tid1 = RT_NULL;

    tid1 = rt_thread_create("bc260_uart",
                            entry_bc260y, RT_NULL,
                            1024,
                            20, 10);

/* 如果获得线程控制块，启动这个线程 */
    if (tid1 != RT_NULL){
        rt_thread_startup(tid1);
        return RT_EOK;
    }
    return -RT_ERROR;
}

//INIT_APP_EXPORT(bc26_device_register);


rt_err_t bc260y_get_time(){
    resp = at_create_resp(64, 0, 5000);

    at_exec_cmd(resp, "AT+CCLK?");
    at_resp_parse_line_args_by_kw(resp, "+CCLK:", "+CCLK: \"%d/%d/%d,%d:%d:%d+%d",
                                                                                  &bc260y_time.year,
                                                                                  &bc260y_time.month,
                                                                                  &bc260y_time.day,
                                                                                  &bc260y_time.hour,
                                                                                  &bc260y_time.min,
                                                                                  &bc260y_time.sec,
                                                                                  &bc260y_time.z);


    rt_kprintf("%d/%.2d/%.2d,%.2d:%.2d:%.2d\n", bc260y_time.year,
               bc260y_time.month,
               bc260y_time.day,
               (bc260y_time.hour+bc260y_time.z)%24,
               bc260y_time.min,
               bc260y_time.sec);
    at_delete_resp(resp);
    return RT_EOK;
}

rt_err_t bc260y_get_ip(){
    resp = at_create_resp(64, 0, 5000);

    at_exec_cmd(resp, "AT+CGPADDR=0");
    at_resp_parse_line_args_by_kw(resp, "+CGPADDR:", "+CGPADDR: 0,\"%[^,\"]", bc260y_ip);


    rt_kprintf("IP:%s\n", bc260y_ip);
    at_delete_resp(resp);
    return RT_EOK;
}

rt_err_t bc260y_mqtt_open(){
    int v1=-1,v2=-1;
    /* 这里网络延时比较高,会先返回OK, 如果写line=0, 会立即结束, 无法接收到后续数据 */
    resp = at_create_resp(64, 4, 15000);

    at_exec_cmd(resp, "AT+QMTOPEN=0,\"DN2HGX3J4C.iotcloud.tencentdevices.com\",1883");

    at_resp_parse_line_args_by_kw(resp, "+QMTOPEN:", "+QMTOPEN: %d,%d", &v1,&v2);

    at_delete_resp(resp);
    if((v1&&v2)!=0){
        LOG_E("bc260y_mqtt_open failed...\n");
        return -RT_ERROR;
    }
    return RT_EOK;
}
rt_err_t bc260y_mqtt_uart_send(char* data){
    return rt_device_write(rt_device_find(BC260Y_UART),0,data, rt_strlen(data));
}
rt_err_t bc260y_mqtt_connect(){
    int v1=-1,v2=-1,v3=-1;
    /* 这里网络延时比较高,会先返回OK, 如果写line=0, 会立即结束, 无法接收到后续数据 */
    resp = at_create_resp(256, 4, 15000);

    at_exec_cmd(resp, "AT+QMTCONN=0,\"device00\",\"DN2HGX3J4Cdevice00;12010126;28cae;1720627200\",\"225c7c43b439d5365823ff0862becb2ecf84a0fcd648951b47ff0fe488f2af40;hmacsha256\"");

    at_resp_parse_line_args_by_kw(resp, "+QMTCONN:", "+QMTCONN: %d,%d,%d", &v1,&v2,&v3);

    rt_kprintf("connect:v1:%d   v2:%d v3:%d\n", v1,v2,v3);
    at_delete_resp(resp);
    if((v1&&v2&&v3)!=0){
        LOG_E("bc260y_mqtt_connect failed...\n");
        return -RT_ERROR;
    }
    return RT_EOK;
}

rt_err_t bc260y_rest(){
    resp = at_create_resp(64, 0, 200);
    at_exec_cmd(resp, "AT+QRST=1");
    at_delete_resp(resp);
    return RT_EOK;
}

rt_err_t bc260y_mqtt_topic_pub(){
    bc260y_mqtt_uart_send("AT+QMTPUB=0,1,1,0,\"$thing/up/property/DN2HGX3J4C/device00\"\r\n");
    rt_thread_mdelay(50);

    return RT_EOK;
}

rt_err_t bc260y_mqtt_set_pub_data(float temp, float longitude,float latitude,int signal,uint32_t utc){
    char str1[256];

    sprintf(str1, "{\"method\":\"report\",\"clientToken\":\"bc260y\",\"timestamp\":%d,\"params\":{\"lac\":0,\"cid\":0,\"mnc\":0,\"mcc\":0,\"networkType\":1,\"y\":0,\"z\":0,\"x\":0,\"voltage\":3300,\"rsrq\":10,\"temp\":%f,\"level\":0}""}",utc, temp);

    bc260y_mqtt_uart_send(str1);
    bc260y_mqtt_uart_send("\x1a");

    return RT_EOK;
}


int bc260y_at_init(){
    bc260y_uart_init();
    /* bc260 at client init */
    at_client_init(BC260Y_UART,256,512);

    return 0;
}
INIT_APP_EXPORT(bc260y_at_init);
float i=1.5f;
void entry_bc260y_mqtt(){
    bc260y_rest();
    rt_thread_mdelay(10000);
    bc260y_get_time();
    bc260y_get_ip();
    bc260y_mqtt_open();
    bc260y_mqtt_connect();

    while (1){

        bc260y_mqtt_topic_pub();
        bc260y_mqtt_set_pub_data(i,RT_NULL,RT_NULL,RT_NULL,1681448942);
        rt_thread_mdelay(10*60000);
        i+=1;
    }
}
int bc260mqttregister(){
    static rt_thread_t tid2 = RT_NULL;

    tid2 = rt_thread_create("mqtt",
                            entry_bc260y_mqtt, RT_NULL,
                            2048,
                            25, 10);

/* 如果获得线程控制块，启动这个线程 */
    if (tid2 != RT_NULL){
        rt_thread_startup(tid2);
        return RT_EOK;
    }
    return -RT_ERROR;
}

INIT_APP_EXPORT(bc260mqttregister);