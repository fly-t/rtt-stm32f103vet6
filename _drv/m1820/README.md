官网: http://mysentech.com/gjdszwdxpm1820

## M1820Z 移植要点

需要调整软件延时时间 delay_us

移植完成调用函数

```c
    M1820_Init();
    M1820_SetConfig(CFG_MPS_Single, CFG_Repeatbility_High);

    while (1){
        float temp;
        M1820_GetTemp(&temp);
        int a = (temp+0.005)*100;
        rt_kprintf("%d.%d",a/100,(a%100));
        rt_thread_mdelay(100);
    }
```