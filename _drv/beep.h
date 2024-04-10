//
// Created by dd21 on 2024/4/9.
//

#ifndef RTTHREAD_BEEP_H
#define RTTHREAD_BEEP_H


#include <rtthread.h>

#define BEEP_PWM_DEVICE  "pwm1"
#define BEEP_PWM_CH      1

int beep_init(void);                         //蜂鸣器初始化
int beep_on(void);                           //蜂鸣器开
int beep_off(void);                          //蜂鸣器关
int beep_set(uint16_t freq, uint8_t volume); //蜂鸣器设定


#endif //RTTHREAD_BEEP_H
