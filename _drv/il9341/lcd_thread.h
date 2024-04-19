//
// Created by dd21 on 2024/4/17.
//

#ifndef RTTHREAD_LCD_THREAD_H
#define RTTHREAD_LCD_THREAD_H

#include "board.h"

#define LCD_PIN_REST GET_PIN(E,1)
#define LCD_PIN_BL GET_PIN(D,12)

#define NEx  1     // ne1
#define PAx  16    // PA16

/* command reg */
#define LCD_CMD_ADDR (uint32_t)(0x60000000 +(0x4000000)*(NEx-1))

/* data reg */
#define LCD_DATA_ADDR (uint32_t)(LCD_CMD_ADDR|(2<<PAx))      // 2^PAx<<1 -> (1<<PAx)<<1) ->(2<<PAx))



#endif //RTTHREAD_LCD_THREAD_H
