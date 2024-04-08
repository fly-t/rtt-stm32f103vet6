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
#include <board.h>
#include "led.h"

/* defined the LED0 pin: PB1 */


int main(void)
{
    rt_device_t led_rgb = RT_NULL;

    led_rgb = rt_device_find("rgb");
    if(led_rgb==RT_NULL){
        rt_kprintf("device led-rgb not find\n");
        return RT_ERROR;
    }

    /* call the bsp function */
    rt_device_init(led_rgb);
    rt_device_open(led_rgb,RT_DEVICE_FLAG_RDWR);

//    rt_led_rgb_t led_demo;
//    led_demo.pin = GET_PIN(B,1);
//    led_demo.status = PIN_LOW;
//    rt_device_write(led_rgb,RT_NULL,(void*)&led_demo,sizeof(led_demo));
    return RT_EOK;
}
