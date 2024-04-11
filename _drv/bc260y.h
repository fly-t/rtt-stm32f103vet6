//
// Created by dd21 on 2024/4/10.
//

#ifndef RTTHREAD_BC260Y_H
#define RTTHREAD_BC260Y_H

#include "rtdevice.h"

#define BC260Y_UART "uart2"
#define BC260Y_BAUD BAUD_RATE_9600
#define testdata  "bc260 baud9600 test\n"

struct time{
    uint16_t year;
    uint16_t month;
    uint16_t day;
    uint16_t hour;
    uint16_t min;
    uint16_t sec;
    uint16_t z;
};

/* 串口设备句柄 */
static rt_device_t serial;

/* config the  uart for bc260y */

rt_err_t bc260y_rxcb(rt_device_t dev, rt_size_t size);
rt_err_t bc260y_init();


#endif //RTTHREAD_BC260Y_H
