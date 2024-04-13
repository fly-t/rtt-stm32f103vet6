//
// Created by dd21 on 2024/4/10.
//

#ifndef RTTHREAD_BC260Y_H
#define RTTHREAD_BC260Y_H

#include "rtdevice.h"
#include "board.h"

#define BC260Y_RESET_PIN GET_PIN(B,7)
#define BC260Y_UART "uart2"
#define BC260Y_BAUD BAUD_RATE_9600
#define testdata  "bc260 baud9600 test\n"

struct time{
    uint16_t year;
    uint16_t month;
    uint16_t day;
    uint16_t hour;
    uint16_t min;
    uint16_t sec;
    uint16_t z;
};

struct mqtt_data{
    float temp;         // 温度
    float longitude;    // 经度
    float latitude;     // 纬度
    int signal;         // 信号强度
    uint32_t utc;       // utc时间
};

/* 串口设备句柄 */
static rt_device_t serial;

/* config the  uart for bc260y */

rt_err_t bc260y_rxcb(rt_device_t dev, rt_size_t size);
rt_err_t bc260y_init();
rt_err_t bc260y_uart_init();
rt_err_t bc260y_get_time();
rt_err_t bc260y_get_ip();
rt_err_t bc260y_keepalive_forever();
rt_err_t bc260y_mqtt_disconnect();
rt_err_t bc260y_mqtt_open();
#endif //RTTHREAD_BC260Y_H
