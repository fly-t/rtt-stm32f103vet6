//
// Created by dd21 on 2024/4/7.
//
#include <rtdevice.h>
#include "drv_log.h"
#include "led.h"

rt_err_t  led_init(rt_device_t dev){
    rt_pin_mode(LEDR_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LEDG_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LEDB_PIN, PIN_MODE_OUTPUT);

    rt_pin_write(LEDR_PIN,PIN_HIGH);
    rt_pin_write(LEDG_PIN,PIN_HIGH);
    rt_pin_write(LEDB_PIN,PIN_HIGH);
    rt_kprintf("led init\n");
    return 0;
}
rt_err_t  led_open(rt_device_t dev, rt_uint16_t oflag){
    rt_kprintf("led open\n");
    return 0;
}
rt_err_t  led_close(rt_device_t dev){
    rt_kprintf("led close\n");
    return 0;
}

rt_ssize_t led_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size){
    return 0;
}


rt_ssize_t led_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size){
    rt_kprintf("led led_write\n");

    return 0;
}


int rt_led_init (void){
    rt_device_t led_rgb = RT_NULL;
    led_rgb =  rt_device_create(RT_Device_Class_Char,1);

    /* if create led device failed will print message and return */
    if(RT_NULL == led_rgb){
        LOG_E("led - rt_device_create failed\n");
        return RT_EFAULT;
    }

    /* if create successful */
    led_rgb->init = led_init;
    led_rgb->open = led_open;
    led_rgb->close = led_close;
    led_rgb->write = led_write;
    led_rgb->read = led_read;

    /* register led_rgb, for user find name "led-rgb", flag set: Read, Write or R&W  */
    rt_device_register(led_rgb,"rgb",RT_DEVICE_FLAG_RDWR);
    return 0;
}

INIT_BOARD_EXPORT(rt_led_init);