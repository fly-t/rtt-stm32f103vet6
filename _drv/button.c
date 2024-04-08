//
// Created by dd21 on 2024/4/8.
//

#include "button.h"
#include "rtdbg.h"

#define MY_BUTTON_CALL(func, argv) \
    do { if ((func) != RT_NULL) func argv; } while (0)


typedef struct bsp_button_manage
{
    rt_uint8_t num;                 /* 已注册的按键的数目 */
    rt_timer_t timer;               /* 按键扫描用到的定时器 */
    bsp_button_t *button_list[MY_BUTTON_LIST_MAX];  /* 存储按键指针的数组 */
}bsp_button_manage_t;

static bsp_button_manage_t button_manage;

int bsp_button_register(bsp_button_t *button) {
    /* 初始化按键默认的高低电平 */
    if(button->press_logic_level==0){
        rt_pin_mode(button->pin,PIN_MODE_INPUT_PULLUP);
    }
    else{
        rt_pin_mode(button->pin,PIN_MODE_INPUT_PULLDOWN);
    }

    /* 初始化按键结构体 */
    button->cnt = 0;
    button->event = BUTTON_EVENT_NONE;

    /* 添加按键到管理列表 */
    button_manage.button_list[button_manage.num++] = button;
    return 0;
}


static void my_button_scan(void *param){
    rt_uint16_t cnt_old;

    /* 遍历所有按键对象 */
    for (uint8_t i = 0; i < button_manage.num; i++){
        cnt_old = button_manage.button_list[i]->cnt;

        /* 判断是否为设置的电平 */
        if(rt_pin_read(button_manage.button_list[i]->pin)==(button_manage.button_list[i]->press_logic_level)){
            /* 按键扫描的计数值加一 */
            button_manage.button_list[i]->cnt+=1;

            /* 连续按下的时间达到单击按下事件触发的阈值: */
            if(button_manage.button_list[i]->cnt == MY_BUTTON_DOWN_MS/MY_BUTTON_SCAN_SPACE_MS){
                LOG_D("BUTTON_DOWN");
                button_manage.button_list[i]->event = BUTTON_EVENT_CLICK_DOWN;
                MY_BUTTON_CALL(button_manage.button_list[i]->cb,(button_manage.button_list[i]));
            }
            else if(button_manage.button_list[i]->cnt == MY_BUTTON_HOLD_MS/MY_BUTTON_SCAN_SPACE_MS){
                LOG_D("BUTTON_EVENT_HOLD");
                button_manage.button_list[i]->event = BUTTON_EVENT_HOLD;
                MY_BUTTON_CALL(button_manage.button_list[i]->cb,(button_manage.button_list[i]));
            }
            else if(button_manage.button_list[i]->cnt > MY_BUTTON_HOLD_MS/MY_BUTTON_SCAN_SPACE_MS){
                LOG_D("BUTTON_EVENT_HOLD_CYC");
                button_manage.button_list[i]->event = BUTTON_EVENT_HOLD_CYC;
                /* 判断有多少个holding 周期, 需要回调多少次回调函数 */
                /*if (hold_cyc_period &&                                                  3.
                                        (cnt %                                            2.
                                            (hold_cyc_period / MY_BUTTON_SCAN_SPACE_MS))  1.
                                                == 0)                                     4.                     */
                if (button_manage.button_list[i]->hold_cyc_period &&
                        button_manage.button_list[i]->cnt %
                            (button_manage.button_list[i]->hold_cyc_period / MY_BUTTON_SCAN_SPACE_MS) == 0){
                    MY_BUTTON_CALL(button_manage.button_list[i]->cb,(button_manage.button_list[i]));
                }
            }
        }
        /* 检测按键的电平状态为抬起状态 */
        else{
            /* 计数复位 */
            button_manage.button_list[i]->cnt = 0;
            if (cnt_old >= MY_BUTTON_DOWN_MS / MY_BUTTON_SCAN_SPACE_MS  /* 如果抬起的时间 >= 单击的时间 */
                    &&
                cnt_old < MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS){ /* 并且 抬起的时间 < holding的时间 */
                LOG_D("BUTTON_CLICK_UP");
                button_manage.button_list[i]->event = BUTTON_EVENT_CLICK_UP;
                MY_BUTTON_CALL(button_manage.button_list[i]->cb, (button_manage.button_list[i]));
            }
            /* 如果 抬起时候的计数值 >= holding的时间 判断为 holding抬起 */
            else if (cnt_old >= MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS)
            {
                LOG_D("BUTTON_HOLD_UP");
                button_manage.button_list[i]->event = BUTTON_EVENT_HOLD_UP;
                MY_BUTTON_CALL(button_manage.button_list[i]->cb, (button_manage.button_list[i]));
            }

        }
    }
}



/* 启动软件定时器 */
int bsp_button_start() {
    if(button_manage.timer != RT_NULL)
        return RT_ERROR;

    /* 创建定时器 */
    button_manage.timer = rt_timer_create("timer1", /* 定时器名字是 timer1 */
                                          my_button_scan, /* 超时时回调的处理函数 */
                                          RT_NULL, /* 超时函数的入口参数 */
                                          RT_TICK_PER_SECOND * MY_BUTTON_SCAN_SPACE_MS / 1000, /* 定时长度，以OS Tick为单位，即10个OS Tick */
                                          RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER); /* 周期性软件定时器 */

    /* 启动定时器 */
    if (button_manage.timer != RT_NULL)
        rt_timer_start(button_manage.timer);
    return 0;
}
