# led bsp driver demo

`main.c`

``` c
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
    rt_device_open(led_rgb,RT_DEVICE_OFLAG_OPEN);
    rt_device_init(led_rgb);

    rt_led_rgb_t ledStat;
    ledStat.pin = LEDB_PIN;
    ledStat.status = PIN_LOW;
    rt_device_write(led_rgb,RT_NULL,(void*)&ledStat, sizeof(ledStat));

    rt_led_rgb_t ledStat2;
    ledStat2.pin = LEDB_PIN;
    rt_device_read(led_rgb,RT_NULL,(void*)&ledStat2,sizeof(ledStat));

    rt_kprintf("led B: %d\n",ledStat2.status);
    return RT_EOK;
}

```

## button bsp driver

通过软件定时器进行检测按键是否按下.

> 需要注意

```c
# 官方实现 有问题
#define KEY_PIN          1
#define KEY_PRESS_VALUE  0
```
将上述地方修改为

```c
#define KEY1_PIN          GET_PIN(A,0)
```
将结构体修改为

```c
typedef struct bsp_button{
    rt_uint8_t  press_logic_level;  /* 按键按下时被检测到的电平(目标检测电平): 0->low  1->high */
    rt_uint16_t cnt;                /* 连续扫描到按下状态的次数 */
    rt_uint16_t hold_cyc_period;    /* 长按周期回调的周期 */
    rt_base_t pin;                  /* 按键对应的 pin 编号 */

    enum bsp_button_event event;    /* 按键的触发的事件 */
    void  (*cb)(void* arg );/* 按键触发事件回调函数 */
}bsp_button_t;
```

## beep driver

`方法 1:` 在`rtconfig.h`中添加pwm宏:

```c
#define BSP_USING_PWM
#define BSP_USING_PWM1
#define BSP_USING_PWM1_CH1
```

`方法 2:` 在修改`/board/konfig`文件 其中添加pwm:
> - 添加完之后使用menuconfig多了pwm的选项, 然后勾选
> - 使用scons 进行项目配置, 后会发现在rtconfig.h中多了方法1中的宏
> - 编写beep驱动

```c
menuconfig BSP_USING_PWM
            bool "Enable PWM"
            default n
            select RT_USING_PWM
            if BSP_USING_PWM
            menuconfig BSP_USING_PWM1
                bool "Enable timer1 output PWM"
                default n
                if BSP_USING_PWM1
                    config BSP_USING_PWM1_CH1
                        bool "Enable PWM1 channel1"
                        default n
                endif
            endif
```
