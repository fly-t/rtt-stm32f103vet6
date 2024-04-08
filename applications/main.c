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
#include "button.h"


#define KEY1_PIN          GET_PIN(A,0)
#define KEY2_PIN          GET_PIN(C,13)
#define KEY_PRESS_VALUE  1

rt_device_t  led_rgb;


void key_cb(bsp_button_t *button)
{
    switch (button->event)
    {
        case BUTTON_EVENT_CLICK_UP:
            rt_pin_mode(LEDR_PIN,PIN_MODE_OUTPUT);
            rt_pin_write(LEDR_PIN,PIN_LOW);
            rt_kprintf("This is click up callback!\n");
            break;
        case BUTTON_EVENT_HOLD_CYC:
            rt_kprintf("This is hold cyc callback!\n");
            break;
        default:
            ;
    }
}
void key2_cb(bsp_button_t *button)
{
    switch (button->event)
    {
        case BUTTON_EVENT_CLICK_UP:
            rt_pin_mode(LEDR_PIN,PIN_MODE_OUTPUT);
            rt_pin_write(LEDR_PIN,PIN_HIGH);
            rt_kprintf("key2: This is click up callback!\n");
            break;
        case BUTTON_EVENT_HOLD_CYC:
            rt_kprintf("key2: This is hold cyc callback!\n");
            break;
        default:
            ;
    }
}
int main(void)
{

    /* user app entry */
    bsp_button_t key = {0};
    bsp_button_t key2 = {0};

    key.press_logic_level = KEY_PRESS_VALUE;
    key.hold_cyc_period = 100;
    key.cb = (void*)key_cb;
    key.pin = KEY1_PIN;

    key2.press_logic_level = KEY_PRESS_VALUE;
    key2.hold_cyc_period = 100;
    key2.cb = (void *)key2_cb;
    key2.pin = KEY2_PIN;

    bsp_button_register(&key);
    bsp_button_register(&key2);

    bsp_button_start();
    while (1){
        rt_thread_mdelay(10);
    }
    return RT_EOK;
}
