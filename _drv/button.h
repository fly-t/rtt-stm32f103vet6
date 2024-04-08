//
// Created by dd21 on 2024/4/8.
//

#ifndef RTTHREAD_BUTTON_H
#define RTTHREAD_BUTTON_H

/* 判定按键状态为:`按下`的阈值 */
#define MY_BUTTON_DOWN_MS        50
#define MY_BUTTON_HOLD_MS        700

/* 检测周期 */
#define MY_BUTTON_SCAN_SPACE_MS  20

#define MY_BUTTON_LIST_MAX       2

#include "rtdevice.h"
typedef void (*my_button_callback)(void*);

enum bsp_button_event{
    BUTTON_EVENT_CLICK_DOWN, /* 按键单击按下事件 */
    BUTTON_EVENT_CLICK_UP,   /* 按键单击结束事件 */
    BUTTON_EVENT_HOLD,       /* 按键长按开始事件 */
    BUTTON_EVENT_HOLD_CYC,   /* 按键长按周期性触发事件 */
    BUTTON_EVENT_HOLD_UP,    /* 按键长按结束事件 */
    BUTTON_EVENT_NONE        /* 无按键事件 */
};

typedef struct bsp_button{
    rt_uint8_t  press_logic_level;  /* 按键按下时被检测到的电平(目标检测电平): 0->low  1->high */
    rt_uint16_t cnt;                /* 连续扫描到按下状态的次数 */
    rt_uint16_t hold_cyc_period;    /* 长按周期回调的周期 */
    rt_base_t pin;                  /* 按键对应的 pin 编号 */

    enum bsp_button_event event;    /* 按键的触发的事件 */
    void  (*cb)(void* arg );/* 按键触发事件回调函数 */
}bsp_button_t;

int bsp_button_register( bsp_button_t *button);
int bsp_button_start();

#endif //RTTHREAD_BUTTON_H
