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

static rt_mutex_t dynamic_mutex = RT_NULL;

int i=0;
/* 线程 1 入口 */
void thread1_entry(void* parameter)
{


    while (1)
    {
        rt_mutex_take(dynamic_mutex,RT_WAITING_FOREVER);
        i+=1;
        rt_kprintf("thread-1:%d\n", i);

        /* 延时 100ms */
        rt_thread_mdelay(1000);
        rt_mutex_release(dynamic_mutex);
    }
}

/* 线程 2 入口 */
void thread2_entry(void* parameter)
{


    while (1)
    {
        /* mutex 如果获取失败那就直接将线程挂起 waitting forever 不需要判断*/
        if(rt_mutex_take(dynamic_mutex,RT_WAITING_FOREVER)==RT_EOK){
            i+=1;
            rt_kprintf("thread--2:%d\n", i);
            /* 延时 100ms */
            rt_thread_mdelay(5000);
            rt_mutex_release(dynamic_mutex);
        }

    }
}
int main(void)
{
    /* value 0 means only one source */
    dynamic_mutex = rt_mutex_create("mute", RT_IPC_FLAG_PRIO);


    rt_thread_t thread2_ptr;
    rt_thread_t thread1_ptr;
    thread1_ptr = rt_thread_create("thread1",
                                   thread1_entry, RT_NULL,
                                   512, 30, 10);

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
