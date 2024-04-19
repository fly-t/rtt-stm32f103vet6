//
// Created by dd21 on 2024/4/17.
//

#include "lcd_thread.h"
#include "rtdevice.h"
#include "rtthread.h"
#include "ili9341.h"

//#define LCD_CMD_REG  (*(uint16_t*)((uint32_t)(LCD_CMD_ADDR)))
//#define LCD_DATA_REG (*(uint16_t*)((uint32_t)(LCD_DATA_ADDR)))

//void lcd_write_cmd(uint16_t cmd);
//void lcd_write_data(uint16_t data);


SRAM_HandleTypeDef hsram1;
void MX_FSMC_Init();

/* init the pin and set the backlight ON and rest the lcd */
static void lcd_pin_init(){
    rt_pin_mode(LCD_PIN_REST,PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_PIN_BL,PIN_MODE_OUTPUT);

    rt_pin_write(LCD_PIN_REST,PIN_LOW);
    rt_thread_mdelay(1000);
    rt_pin_write(LCD_PIN_REST,PIN_HIGH);
    rt_pin_write(LCD_PIN_BL,PIN_LOW);
}



int LCD_init(){
    lcd_pin_init();
    MX_FSMC_Init();
    ILI9341_Init();
    return 0;
}

void lcd_il9341_entry(){
    LCD_init();
//    while(1){
////        ILI9341_Test();
//        FPS_Test();
//        rt_thread_mdelay(1);
//    }
}

int lcd_reg(){
    rt_thread_t lcd_thread =  rt_thread_create("lcd_il9341",&lcd_il9341_entry,RT_NULL,1024,15,10);
    if(lcd_thread!=RT_NULL){
        rt_thread_startup(lcd_thread);
    }
    return 0;
}

//INIT_DEVICE_EXPORT(lcd_reg);


/* FSMC initialization function */
void MX_FSMC_Init(void)
{

    /* USER CODE BEGIN FSMC_Init 0 */

    /* USER CODE END FSMC_Init 0 */

    FSMC_NORSRAM_TimingTypeDef Timing = {0};

    /* USER CODE BEGIN FSMC_Init 1 */

    /* USER CODE END FSMC_Init 1 */

    /** Perform the SRAM1 memory initialization sequence
    */
    hsram1.Instance = FSMC_NORSRAM_DEVICE;
    hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
    /* hsram1.Init */
    hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
    hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
    hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
    hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
    hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
    hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
    hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
    hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
    hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
    hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
    /* Timing */
    Timing.AddressSetupTime = 2;
    Timing.AddressHoldTime = 2;
    Timing.DataSetupTime = 16;
    Timing.BusTurnAroundDuration = 2;
    Timing.CLKDivision = 16;
    Timing.DataLatency = 2;
    Timing.AccessMode = FSMC_ACCESS_MODE_A;
    /* ExtTiming */

    if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
    {
        Error_Handler( );
    }

    /** Disconnect NADV
    */

    __HAL_AFIO_FSMCNADV_DISCONNECTED();

    /* USER CODE BEGIN FSMC_Init 2 */

    /* USER CODE END FSMC_Init 2 */
}
