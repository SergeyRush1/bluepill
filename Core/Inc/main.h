#define LED1_ON() SET_BIT(GPIOA->ODR,GPIO_ODR_ODR2)
#define LED1_OFF() CLEAR_BIT(GPIOA->ODR,GPIO_ODR_ODR2)
#define LED2_ON() SET_BIT(GPIOA->ODR,GPIO_ODR_ODR3)
#define LED2_OFF() CLEAR_BIT(GPIOA->ODR,GPIO_ODR_ODR3)
#define LED3_ON() SET_BIT(GPIOA->ODR,GPIO_ODR_ODR4)
#define LED3_OFF() CLEAR_BIT(GPIOA->ODR,GPIO_ODR_ODR4)
#define LED4_ON() SET_BIT(GPIOA->ODR,GPIO_ODR_ODR5)
#define LED4_OFF() CLEAR_BIT(GPIOA->ODR,GPIO_ODR_ODR5)
#define LED5_ON() SET_BIT(GPIOA->ODR,GPIO_ODR_ODR6)
#define LED5_OFF() CLEAR_BIT(GPIOA->ODR,GPIO_ODR_ODR6)
#define LED6_ON() SET_BIT(GPIOA->ODR,GPIO_ODR_ODR7)
#define LED6_OFF() CLEAR_BIT(GPIOA->ODR,GPIO_ODR_ODR7)
#define LED7_ON() SET_BIT(GPIOB->ODR,GPIO_ODR_ODR0)
#define LED7_OFF() CLEAR_BIT(GPIOB->ODR,GPIO_ODR_ODR0)
#define LED8_ON() SET_BIT(GPIOB->ODR,GPIO_ODR_ODR1)
#define LED8_OFF() CLEAR_BIT(GPIOB->ODR,GPIO_ODR_ODR1)
#define LED9_ON() SET_BIT(GPIOB->ODR,GPIO_ODR_ODR10)
#define LED9_OFF() CLEAR_BIT(GPIOB->ODR,GPIO_ODR_ODR10)
#define LED10_ON() SET_BIT(GPIOB->ODR,GPIO_ODR_ODR11)
#define LED10_OFF() CLEAR_BIT(GPIOB->ODR,GPIO_ODR_ODR11)

/*
 C sources
C_SOURCES =  \
Core/Src/main.c \
Drivers/CMSIS/CM3/core_cm3.c \
Drivers/DeviceSupport/ST/Stm32F10x/system_stm32f10x.c \
Drivers/STM3210x_StdPeriph_Driver/src/misc.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_abc.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_bkp.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_can.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_cec.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_crc.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_dac.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_dbgmcu.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_dma.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_exti.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_flash.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_fsmc.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_gpio.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_i2c.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_iwdg.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_pwr.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_rcc.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_rtc.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_sdio.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_spi.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_tim.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_usart.c \
Drivers/STM3210x_StdPeriph_Driver/src/stm32f10x_wwdg.c */