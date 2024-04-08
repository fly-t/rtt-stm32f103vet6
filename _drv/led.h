//
// Created by dd21 on 2024/4/8.
//

#ifndef RTTHREAD_LED_H
#define RTTHREAD_LED_H

#define LEDG_PIN    GET_PIN(B, 1)
#define LEDB_PIN    GET_PIN(B, 2)
#define LEDR_PIN    GET_PIN(B, 5)

#include "board.h"

typedef struct rt_led_rgb{
    rt_base_t pin;
    rt_ssize_t status;
}rt_led_rgb_t;


#endif //RTTHREAD_LED_H
