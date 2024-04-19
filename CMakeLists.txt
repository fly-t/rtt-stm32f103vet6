CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR cortex-m3)
#SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET(CMAKE_C_COMPILER "D:/env/env-windows-1.5.0/tools/bin/../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-gcc.exe")
SET(CMAKE_ASM_COMPILER "D:/env/env-windows-1.5.0/tools/bin/../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-gcc.exe")
SET(CMAKE_C_FLAGS " -mcpu=cortex-m3 -mthumb -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g")
SET(CMAKE_ASM_FLAGS " -c -mcpu=cortex-m3 -mthumb -ffunction-sections -fdata-sections -x assembler-with-cpp -Wa,-mimplicit-it=thumb  -gdwarf-2")
SET(CMAKE_C_COMPILER_WORKS TRUE)

SET(CMAKE_CXX_COMPILER "D:/env/env-windows-1.5.0/tools/bin/../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-g++.exe")
SET(CMAKE_CXX_FLAGS " -mcpu=cortex-m3 -mthumb -ffunction-sections -fdata-sections -Dgcc -O0 -gdwarf-2 -g")
SET(CMAKE_CXX_COMPILER_WORKS TRUE)

SET(CMAKE_OBJCOPY "D:/env/env-windows-1.5.0/tools/bin/../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-objcopy.exe")
SET(CMAKE_SIZE "D:/env/env-windows-1.5.0/tools/bin/../../tools/gnu_gcc/arm_gcc/mingw/bin/arm-none-eabi-size.exe")

SET(CMAKE_EXE_LINKER_FLAGS " -mcpu=cortex-m3 -mthumb -ffunction-sections -fdata-sections -Wl,--gc-sections,-Map=rt-thread.map,-cref,-u,Reset_Handler -T ${CMAKE_SOURCE_DIR}/board/linker_scripts/link.lds")

SET(CMAKE_CXX_STANDARD 14)
PROJECT(rtthread C CXX ASM)
INCLUDE_DIRECTORIES(
	_drv
	applications
	rt-thread/components/net/at/include
	packages/cJSON-v1.7.17
	rt-thread/components/libc/compilers/common/include
	rt-thread/components/libc/compilers/newlib
	rt-thread/components/drivers/include
	board
	board/CubeMX_Config/Inc
	libraries/HAL_Drivers/drivers
	libraries/HAL_Drivers/drivers/config
	libraries/HAL_Drivers
	libraries/HAL_Drivers/CMSIS/Include
	rt-thread/components/dfs/dfs_v1/include
	rt-thread/components/dfs/dfs_v1/filesystems/devfs
	rt-thread/components/finsh
	_drv/il9341
	.
	rt-thread/include
	rt-thread/libcpu/arm/common
	rt-thread/libcpu/arm/cortex-m3
	libraries/STM32F1xx_HAL/CMSIS/Device/ST/STM32F1xx/Include
	libraries/STM32F1xx_HAL/STM32F1xx_HAL_Driver/Inc
	_drv/m1820
	rt-thread/components/libc/posix/io/epoll
	rt-thread/components/libc/posix/io/eventfd
	rt-thread/components/libc/posix/io/poll
	rt-thread/components/libc/posix/ipc
	rt-thread/components/net/netdev/include
	rt-thread/components/net/sal/include
	rt-thread/components/net/sal/include/socket
	rt-thread/components/net/sal/include/dfs_net
	rt-thread/components/net/sal/include/socket/sys_socket
	rt-thread/components/utilities/resource
)

ADD_DEFINITIONS(
	-DRT_USING_LIBC
	-DRT_USING_NEWLIBC
	-DSTM32F103xE
	-DUSE_HAL_DRIVER
	-D_POSIX_C_SOURCE=1
	-D__RTTHREAD__
)

SET(PROJECT_SOURCES
	_drv/button.c
	_drv/led.c
	_drv/bc260y.c
	_drv/decode.c
	_drv/beep.c
	applications/main.c
	rt-thread/components/net/at/src/at_utils.c
	rt-thread/components/net/at/src/at_client.c
	rt-thread/components/net/at/src/at_cli.c
	packages/cJSON-v1.7.17/cJSON.c
	packages/cJSON-v1.7.17/cJSON_Utils.c
	rt-thread/components/libc/compilers/common/cctype.c
	rt-thread/components/libc/compilers/common/cstdlib.c
	rt-thread/components/libc/compilers/common/cstring.c
	rt-thread/components/libc/compilers/common/ctime.c
	rt-thread/components/libc/compilers/common/cunistd.c
	rt-thread/components/libc/compilers/common/cwchar.c
	rt-thread/components/libc/compilers/newlib/syscalls.c
	rt-thread/components/drivers/core/device.c
	rt-thread/components/drivers/ipc/completion.c
	rt-thread/components/drivers/ipc/condvar.c
	rt-thread/components/drivers/ipc/dataqueue.c
	rt-thread/components/drivers/ipc/pipe.c
	rt-thread/components/drivers/ipc/ringblk_buf.c
	rt-thread/components/drivers/ipc/ringbuffer.c
	rt-thread/components/drivers/ipc/waitqueue.c
	rt-thread/components/drivers/ipc/workqueue.c
	rt-thread/components/drivers/misc/rt_drv_pwm.c
	rt-thread/components/drivers/pin/pin.c
	rt-thread/components/drivers/rtc/rtc.c
	rt-thread/components/drivers/serial/serial.c
	board/CubeMX_Config/Src/stm32f1xx_hal_msp.c
	board/board.c
	libraries/HAL_Drivers/drivers/drv_gpio.c
	libraries/HAL_Drivers/drivers/drv_pwm.c
	libraries/HAL_Drivers/drivers/drv_tim.c
	libraries/HAL_Drivers/drivers/drv_usart.c
	libraries/HAL_Drivers/drv_common.c
	libraries/STM32F1xx_HAL/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xe.s
	rt-thread/components/dfs/dfs_v1/filesystems/devfs/devfs.c
	rt-thread/components/dfs/dfs_v1/src/dfs.c
	rt-thread/components/dfs/dfs_v1/src/dfs_file.c
	rt-thread/components/dfs/dfs_v1/src/dfs_fs.c
	rt-thread/components/dfs/dfs_v1/src/dfs_posix.c
	rt-thread/components/finsh/shell.c
	rt-thread/components/finsh/msh.c
	rt-thread/components/finsh/msh_parse.c
	rt-thread/components/finsh/cmd.c
	rt-thread/components/finsh/msh_file.c
	_drv/il9341/fonts.c
	_drv/il9341/ili9341_touch.c
		_drv/il9341/lcd_thread.c
	_drv/il9341/ili9341.c
	rt-thread/src/clock.c
	rt-thread/src/components.c
	rt-thread/src/idle.c
	rt-thread/src/ipc.c
	rt-thread/src/irq.c
	rt-thread/src/klibc/kstdio.c
	rt-thread/src/klibc/kstring.c
	rt-thread/src/kservice.c
	rt-thread/src/mem.c
	rt-thread/src/mempool.c
	rt-thread/src/object.c
	rt-thread/src/scheduler_comm.c
	rt-thread/src/scheduler_up.c
	rt-thread/src/thread.c
	rt-thread/src/timer.c
	rt-thread/libcpu/arm/common/atomic_arm.c
	rt-thread/libcpu/arm/common/div0.c
	rt-thread/libcpu/arm/common/showmem.c
	rt-thread/libcpu/arm/cortex-m3/context_gcc.S
	rt-thread/libcpu/arm/cortex-m3/cpuport.c
	libraries/STM32F1xx_HAL/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c
	libraries/STM32F1xx_HAL/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
	libraries/STM32F1xx_HAL/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rtc_ex.c
	libraries/STM32F1xx_HAL/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c
	libraries/STM32F1xx_HAL/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rtc.c
	libraries/STM32F1xx_HAL/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
	libraries/STM32F1xx_HAL/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c
	libraries/STM32F1xx_HAL/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c
	libraries/STM32F1xx_HAL/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c
	libraries/STM32F1xx_HAL/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c
	libraries/STM32F1xx_HAL/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_crc.c
	libraries/STM32F1xx_HAL/CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c
	libraries/STM32F1xx_HAL/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c
	libraries/STM32F1xx_HAL/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_sram.c
	libraries/STM32F1xx_HAL/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
	libraries/STM32F1xx_HAL/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_fsmc.c
	libraries/STM32F1xx_HAL/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cec.c
	libraries/STM32F1xx_HAL/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_usart.c
	libraries/STM32F1xx_HAL/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c
	_drv/m1820/M601.c
	_drv/m1820/owmy.c
	_drv/m1820/reg_m1820z.c
	rt-thread/components/libc/posix/io/poll/poll.c
	rt-thread/components/libc/posix/io/poll/select.c
	rt-thread/components/net/netdev/src/netdev.c
	rt-thread/components/net/netdev/src/netdev_ipaddr.c
	rt-thread/components/net/sal/dfs_net/dfs_net.c
	rt-thread/components/net/sal/socket/net_netdb.c
	rt-thread/components/net/sal/socket/net_sockets.c
	rt-thread/components/net/sal/src/sal_socket.c
	rt-thread/components/utilities/resource/resource_id.c
)

LINK_DIRECTORIES(
)

LINK_LIBRARIES(
	c
	m
)

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}.elf ${PROJECT_SOURCES})
ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME}.elf POST_BUILD 
COMMAND ${CMAKE_OBJCOPY} -O binary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin COMMAND ${CMAKE_SIZE} ${CMAKE_PROJECT_NAME}.elf)