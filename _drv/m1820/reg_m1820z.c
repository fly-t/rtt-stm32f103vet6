//
// Created by dd21 on 2024/4/13.
//
/* M1820 温度传感器 */
#include "owmy.h"
#include "M601.h"
extern struct rt_mailbox mb;

void entry_m1820(){
    /* M1820 初始化 */
    M1820_Init();
    M1820_SetConfig(CFG_MPS_Single, CFG_Repeatbility_High);

    while (1){
        float temp;
        for(int i=0;i<10;i++){
            M1820_GetTemp(&temp);
//            int a = (temp+0.005)*100;
//            rt_kprintf("rec->%d.%d \n",a/100,(a%100));
            rt_thread_mdelay(100);
        }
        rt_mb_send(&mb, (rt_uint32_t)&temp);
        rt_thread_mdelay(4*60*1000);
    }
}

int reg_m1820(){
    static rt_thread_t tid1 = RT_NULL;

    tid1 = rt_thread_create("1820",
                            entry_m1820, RT_NULL,
                            1024,
                            30, 10);

/* 如果获得线程控制块，启动这个线程 */
    if (tid1 != RT_NULL){
        rt_thread_startup(tid1);
        rt_kprintf("M1820 init successful.\n");
        return RT_EOK;
    }
    return -RT_ERROR;
}

INIT_APP_EXPORT(reg_m1820);