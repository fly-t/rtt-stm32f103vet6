//
// Created by dd21 on 2024/4/17.
//

#include "lcd_9341.h"
#include "rtdevice.h"
#include "rtthread.h"


#define LCD_CMD_REG  (*(uint16_t*)((uint32_t)(LCD_CMD_ADDR)))
#define LCD_DATA_REG (*(uint16_t*)((uint32_t)(LCD_DATA_ADDR)))
#define ILI9341_WRITE_DATA LCD_DATA_REG

//  Orientation params
#define ILI9341_MADCTL_MY  0x80
#define ILI9341_MADCTL_MX  0x40
#define ILI9341_MADCTL_MV  0x20
#define ILI9341_MADCTL_ML  0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH  0x04
#define ILI9341_WRITE_REG  LCD_CMD_REG
#define ILI9341_WIDTH  240
#define ILI9341_HEIGHT 320
#define ILI9341_ROTATION (ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR)

void lcd_write_cmd(uint16_t cmd);
void lcd_write_data(uint16_t data);

void ILI9341_WriteCommand(uint16_t cmd){
    lcd_write_cmd(cmd);
}


void ILI9341_WriteData(uint16_t data){
    lcd_write_data(data);
}
void lcd_config();

SRAM_HandleTypeDef hsram1;
void MX_FSMC_Init();
void ILI9341_Init();
#define	ILI9341_BLACK   0x0000
#define	ILI9341_BLUE    0x001F
#define	ILI9341_RED     0xF800
#define	ILI9341_GREEN   0x07E0
#define ILI9341_CYAN    0x07FF
#define ILI9341_MAGENTA 0xF81F
#define ILI9341_YELLOW  0xFFE0
#define ILI9341_WHITE   0xFFFF

static void ILI9341_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    // column address set
    ILI9341_WriteCommand(0x2A); // CASET
    {
        ILI9341_WriteData(x0 >> 8);
        ILI9341_WriteData(x0 & 0x00FF);
        ILI9341_WriteData(x1 >> 8);
        ILI9341_WriteData(x1 & 0x00FF);
    }
    // row address set
    ILI9341_WriteCommand(0x2B); // RASET
    {
        ILI9341_WriteData(y0 >> 8);
        ILI9341_WriteData(y0 & 0x00FF);
        ILI9341_WriteData(y1 >> 8);
        ILI9341_WriteData(y1 & 0x00FF);
    }
    // write to RAM
    ILI9341_WriteCommand(0x2C); // RAMWR
}
void ILI9341_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    // clipping
    if((x >= ILI9341_WIDTH) || (y >= ILI9341_HEIGHT)) return;
    if((x + w - 1) >= ILI9341_WIDTH) w = ILI9341_WIDTH - x;
    if((y + h - 1) >= ILI9341_HEIGHT) h = ILI9341_HEIGHT - y;

    ILI9341_SetAddressWindow(x, y, x+w-1, y+h-1);
    for(y = h; y > 0; y--) {
        for(x = w; x > 0; x--) {
            ILI9341_WriteData(color);
        }
    }
}
void ILI9341_FillScreen(uint16_t color) {
    ILI9341_FillRectangle(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, color);
}

void ILI9341_Test(void)
{
    ILI9341_FillScreen(ILI9341_WHITE);
    rt_thread_mdelay(1000);
//    ILI9341_WriteString(10, 10, "Fuxk you shit", Font_16x26, ILI9341_RED, ILI9341_WHITE);
    rt_thread_mdelay(2000);
    ILI9341_FillScreen(ILI9341_RED);
    rt_thread_mdelay(1000);
    ILI9341_FillScreen(ILI9341_BLUE);
    rt_thread_mdelay(1000);
    ILI9341_FillScreen(ILI9341_GREEN);
    rt_thread_mdelay(1000);
    ILI9341_FillScreen(ILI9341_YELLOW);
    rt_thread_mdelay(1000);
    ILI9341_FillScreen(ILI9341_CYAN);
    rt_thread_mdelay(1000);
    ILI9341_FillScreen(ILI9341_MAGENTA);
    rt_thread_mdelay(1000);
    ILI9341_FillScreen(ILI9341_BLACK);
    rt_thread_mdelay(2000);

    ILI9341_FillRectangle(10, 10, 30, 20, ILI9341_WHITE);
    rt_thread_mdelay(1500);
    ILI9341_FillScreen(ILI9341_BLACK);
//
//    ILI9341_DrawLine(10, 10, 30, 30, ILI9341_WHITE);
//    rt_thread_mdelay(1500);
//    ILI9341_FillScreen(ILI9341_BLACK);
//
//    ILI9341_DrawCircle(100, 100, 50, ILI9341_WHITE);
//    rt_thread_mdelay(1500);
//    ILI9341_FillScreen(ILI9341_BLACK);
//
//    ILI9341_DrawFilledCircle(100, 100, 50, ILI9341_WHITE);
//    rt_thread_mdelay(1500);
//    ILI9341_FillScreen(ILI9341_BLACK);
//
//    ILI9341_DrawTriangle(10, 10, 50, 50, 100, 10, ILI9341_WHITE);
//    rt_thread_mdelay(1500);
//    ILI9341_FillScreen(ILI9341_BLACK);
//
//    ILI9341_DrawFilledTriangle(10, 10, 50, 50, 100, 10, ILI9341_WHITE);
//    rt_thread_mdelay(1500);
//    ILI9341_FillScreen(ILI9341_WHITE);
//    rt_thread_mdelay(1000);
}



int LCD_init(){
    rt_pin_mode(LCD_PIN_REST,PIN_MODE_OUTPUT);
    rt_pin_mode(LCD_PIN_BL,PIN_MODE_OUTPUT);

    rt_pin_write(LCD_PIN_REST,PIN_LOW);
    rt_thread_mdelay(1000);
    rt_pin_write(LCD_PIN_REST,PIN_HIGH);
    rt_pin_write(LCD_PIN_BL,PIN_LOW);
    MX_FSMC_Init();
    rt_kprintf("cmd:%#x data:%#x\n",LCD_CMD_ADDR,LCD_DATA_ADDR);
    ILI9341_Init();
    ILI9341_Test();
    return 0;
}

INIT_DEVICE_EXPORT(LCD_init);

void ILI9341_Reset(){
    rt_pin_write(LCD_PIN_REST,PIN_LOW);
    rt_thread_mdelay(1000);
    rt_pin_write(LCD_PIN_REST,PIN_HIGH);

}
static void ILI9341_WriteDataMultiple(uint16_t * datas, uint32_t dataNums) {
    while (dataNums--)
    {
        ILI9341_WRITE_DATA = *datas++;
    }
}

void ILI9341_Init() {
    ILI9341_Reset();
    // command list is based on https://github.com/martnak/STM32-ILI9341
    // SOFTWARE RESET
    ILI9341_WriteCommand(0x01);
    rt_thread_mdelay(500);

    // POWER CONTROL A
    ILI9341_WriteCommand(0xCB);
    {
        uint16_t data[] = { 0x39, 0x2C, 0x00, 0x34, 0x02 };
        ILI9341_WriteDataMultiple(data, 5);
    }

    // POWER CONTROL B
    ILI9341_WriteCommand(0xCF);
    {
        uint16_t data[] = { 0x00, 0xC1, 0x30 };
        ILI9341_WriteDataMultiple(data, 3);
    }

    // DRIVER TIMING CONTROL A
    ILI9341_WriteCommand(0xE8);
    {
        uint16_t data[] = { 0x85, 0x00, 0x78 };
        ILI9341_WriteDataMultiple(data, 3);
    }

    // DRIVER TIMING CONTROL B
    ILI9341_WriteCommand(0x00EA);
    {
        uint16_t data[] = { 0x00, 0x00 };
        ILI9341_WriteDataMultiple(data, 2);
    }

    // POWER ON SEQUENCE CONTROL
    ILI9341_WriteCommand(0xED);
    {
        uint16_t data[] = { 0x04, 0x03, 0x12, 0x81 };
        ILI9341_WriteDataMultiple(data, 4);
    }

    // PUMP RATIO CONTROL
    ILI9341_WriteCommand(0xF7);
    {
        uint16_t data[] = { 0x20 };
        ILI9341_WriteDataMultiple(data, 1);
    }

    // POWER CONTROL,VRH[5:0]
    ILI9341_WriteCommand(0xC0);
    {
        uint16_t data[] = { 0x23 };
        ILI9341_WriteDataMultiple(data, 1);
    }

    // POWER CONTROL,SAP[2:0];BT[3:0]
    ILI9341_WriteCommand(0xC1);
    {
        uint16_t data[] = { 0x10 };
        ILI9341_WriteDataMultiple(data, 1);
    }

    // VCM CONTROL
    ILI9341_WriteCommand(0xC5);
    {
        uint16_t data[] = { 0x3E, 0x28 };
        ILI9341_WriteDataMultiple(data, 2);
    }

    // VCM CONTROL 2
    ILI9341_WriteCommand(0xC7);
    {
        uint16_t data[] = { 0x86 };
        ILI9341_WriteDataMultiple(data, 1);
    }

    // MEMORY ACCESS CONTROL
    ILI9341_WriteCommand(0x36);
    {
        uint16_t data[] = { 0x48 };
        ILI9341_WriteDataMultiple(data, 1);
    }

    // PIXEL FORMAT
    ILI9341_WriteCommand(0x3A);
    {
        uint16_t data[] = { 0x55 };
        ILI9341_WriteDataMultiple(data, 1);
    }

    // FRAME RATIO CONTROL, STANDARD RGB COLOR
    ILI9341_WriteCommand(0xB1);
    {
        uint16_t data[] = { 0x00, 0x18 };
        ILI9341_WriteDataMultiple(data, 1);
    }

    // DISPLAY FUNCTION CONTROL
    ILI9341_WriteCommand(0xB6);
    {
        uint16_t data[] = { 0x08, 0x82, 0x27 };
        ILI9341_WriteDataMultiple(data, 3);
    }

    // 3GAMMA FUNCTION DISABLE
    ILI9341_WriteCommand(0xF2);
    {
        uint16_t data[] = { 0x00 };
        ILI9341_WriteDataMultiple(data, 1);
    }

    // GAMMA CURVE SELECTED
    ILI9341_WriteCommand(0x26);
    {
        uint16_t data[] = { 0x01 };
        ILI9341_WriteDataMultiple(data, 1);
    }

    // POSITIVE GAMMA CORRECTION
    ILI9341_WriteCommand(0xE0);
    {
        uint16_t data[] = { 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1,
                            0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00 };
        ILI9341_WriteDataMultiple(data, 15);
    }

    // NEGATIVE GAMMA CORRECTION
    ILI9341_WriteCommand(0xE1);
    {
        uint16_t data[] = { 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1,
                            0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F };
        ILI9341_WriteDataMultiple(data, 15);
    }

    // EXIT SLEEP
    ILI9341_WriteCommand(0x11);
    rt_thread_mdelay(120);

    // TURN ON DISPLAY
    ILI9341_WriteCommand(0x29);

    // MADCTL
    ILI9341_WriteCommand(0x36);
    {
        uint16_t data[] = { ILI9341_ROTATION };
        ILI9341_WriteDataMultiple(data, 1);
    }
}




void lcd_write_cmd(uint16_t cmd){
    LCD_CMD_REG = cmd;
}

void lcd_write_data(uint16_t data){
    LCD_DATA_REG = data;
}



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
    Timing.AddressSetupTime = 6;
    Timing.AddressHoldTime = 15;
    Timing.DataSetupTime = 16;
    Timing.BusTurnAroundDuration = 0;
    Timing.CLKDivision = 16;
    Timing.DataLatency = 17;
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
