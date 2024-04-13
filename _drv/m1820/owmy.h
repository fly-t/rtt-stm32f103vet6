/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef OWMY_H
#define OWMY_H

/* Includes ------------------------------------------------------------------*/
//#include "HAL_device.h"
//#include "hal_conf.h"
//#include "types.h"
#include "MY_stdtype.h"
#include <rtdevice.h>
#include <board.h>

/*********�û�����������us����ʱ************/

#define ow_Delay_us			rt_thread_delay
#define DELAY_Ms(x)			rt_thread_delay(1000*x)

/* Definition of DQ pin for one-wire communication*/


#define DQ_PIN GET_PIN(E,6)
/* Macros for DQ manipulation*/
#define ow_DQ_set()     rt_pin_write(DQ_PIN,PIN_HIGH)
#define ow_DQ_reset()     rt_pin_write(DQ_PIN,PIN_LOW)
#define ow_DQ_get()     rt_pin_read(DQ_PIN)


typedef enum {
  READY       = 0,
  BUSY    		= 1
} OW_SLAVESTATUS;

/* Exported_Functions----------------------------------------------------------*/
void M1820_Init(void);
BOOL OW_ResetPresence(void);
void OW_WriteByte(uint8_t data);
uint8_t OW_ReadByte(void);
OW_SLAVESTATUS OW_ReadStatus(void);

uint8_t OW_Read2Bits(void);


#endif /* OWMY_H */
