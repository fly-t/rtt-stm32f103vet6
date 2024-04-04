/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

/* defined the LED0 pin: PB1 */
#define LED0_PIN    GET_PIN(B, 1)



/* 线程 1 入口 */
void thread1_entry(void* parameter)
{
    int i=0;

    while (1)
    {
        i+=1;
        rt_kprintf("thread-1:%d\n", i);

        /* 延时 100ms */
//        rt_thread_mdelay(1000);
        if (i>200){
            return;
        }
    }
}

/* 线程 2 入口 */
void thread2_entry(void* parameter)
{
    int i=0;

    while (1)
    {
        i+=1;
        rt_kprintf("thread--2:%d\n", i);

        /* 延时 100ms */
//        rt_thread_mdelay(1000);
        if (i>200){
            return;
        }
    }
}
int main(void)
{
    rt_thread_t thread2_ptr;
    rt_thread_t thread1_ptr;
    thread1_ptr = rt_thread_create("thread1",
                                   thread1_entry, RT_NULL,
                                   512, 30, 20);

    thread2_ptr = rt_thread_create("thread2",
                                   thread2_entry, RT_NULL,
                                   512, 30, 10);

    /* 启动线程 */
    if (thread1_ptr != RT_NULL) rt_thread_startup(thread1_ptr);
    if (thread2_ptr != RT_NULL) rt_thread_startup(thread2_ptr);



    int count = 1;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    while (count++)
    {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }



    return RT_EOK;
}
