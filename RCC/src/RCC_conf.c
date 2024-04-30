#include "RCC_conf.h"
//#include "stm32f10x.h"
#define RCC_CFGR_MCO_PLLCLK_DIV2             0x07000000U 
#define FLASH_ACR_LATENCY_Pos               (0U) 
void RCC_Deinit(){
    SET_BIT(RCC->CR, RCC_CR_HSION);//вкл HSI
    while(READ_BIT(RCC->CR, RCC_CR_HSIRDY == RESET)) {}
    //сбрасываем колибровку
    MODIFY_REG(RCC->CR, RCC_CR_HSITRIM, 0x80U);
     //полностью очищаем конфигурационный регистр
    CLEAR_REG(RCC->CFGR);
    while (READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RESET) {}
     //PLL
    CLEAR_BIT(RCC->CR, RCC_CR_PLLON);
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) != RESET) {}
    //выключаем HSE и детектор тактового силнала 
    CLEAR_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_CSSON);
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) != RESET) {}
    //сбрасываем бит разрешающий использование внешнего генератора 
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP);
    //Cбрасываем флаги прерываний
    SET_BIT(RCC->CSR, RCC_CSR_RMVF);
    //Запрещаем все прерывания
    CLEAR_REG(RCC->CIR);
}

void CMSIS_RCC_CONFIG_72MGhz(void){

    SET_BIT(RCC->CR, RCC_CR_HSION); //Запустим внутренний RC генератор на 8 МГц
    while (READ_BIT(RCC->CR, RCC_CR_HSIRDY) == 0); //Дождемся поднятия флага о готовности
    CLEAR_BIT(RCC->CR, RCC_CR_HSEBYP); //Просто сбросим этот бит в 0(Хотя изначально он и так должен быть в 0).
    SET_BIT(RCC->CR, RCC_CR_HSEON); //Запустим внешний кварцевый резонатор. Он у нас на 8 MHz.
    while (READ_BIT(RCC->CR, RCC_CR_HSERDY) == 0); //Дождемся поднятия флага о готовности
    SET_BIT(RCC->CR, RCC_CR_CSSON); //Включим CSS
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_HSE); //Выберем HSE в качестве System Clock(PLL лучше пока не выбирать, он у нас отключен)
	//p.s. Спасибо KARMA Electronics за подсказку.

    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1); //APB Prescaler /1
	//Вот тут в Note пишется отсылка к Flash(см. стр. 58)

	//Поэтому прервемся и настроим Flash (Flash access control register (FLASH_ACR))
    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, 0b010 << FLASH_ACR_LATENCY_Pos); //010 Two wait states, if 48 MHz < SYSCLK <= 72 MHz
	
    SET_BIT(FLASH->ACR, FLASH_ACR_PRFTBE); //Prefetch is enabled
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV2); //APB1 Prescaler /2, т.к. PCLK1 max 36MHz
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV1); //APB2 Prescaler /1. Тут нас ничего не ограничивает. Будет 72MHz.
	
    MODIFY_REG(RCC->CFGR, RCC_CFGR_ADCPRE, RCC_CFGR_ADCPRE_DIV6); //ADC Prescaler /6, чтоб было 12MHz, т.к. максимальная частота тут 14 MHz
	SET_BIT(RCC->CFGR, RCC_CFGR_PLLSRC); //В качестве входного сигнала для PLL выберем HSE
    CLEAR_BIT(RCC->CFGR, RCC_CFGR_PLLXTPRE_HSE); //Никакое предделение перед PLL нам не нужно. Поэтому /1.
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PLLMULL, RCC_CFGR_PLLMULL9); //т.к. кварц у нас 8Mhz, а нам нужно 72MHz, то в PLL нужно сделать умножение на 9. 8MHz * 9 = 72MHz.
	CLEAR_BIT(RCC->CFGR, RCC_CFGR_USBPRE); //Для USB 72MHz/1.5 = 48MHz
    MODIFY_REG(RCC->CFGR, RCC_CFGR_MCO, RCC_CFGR_MCO_PLLCLK_DIV2); //В качестве тактирования для MCO выбрал PLL. Будет 36 MHz.
	//Чтоб воспользоваться выводом MCO, нужно настроить ножку PA8 в альтернативную функцию на выход.
    SET_BIT(RCC->CR, RCC_CR_PLLON); //Запустим PLL
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL); //Выберем PLL в качестве System Clock
    while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) == 0); //Дожидемся поднятия флага включения PLL

}

void SetSysClockTo72(void)
{

    /*
    SET_BIT(RCC->CR, RCC_CR_HSION);//запускаем внутренний RC генератор на 8Мгц
    while(READ_BIT(RCC->CR, RCC_CR_HSIRDY == RESET)) {}
    SET_BIT(RCC->CR, RCC_CR_HSEON);//запускаем внутренний RC генератор на 8Мгц
    while(READ_BIT(RCC->CR, RCC_CR_HSERDY == RESET)) {}
    CLEAR_BIT(RCC->CR,RCC_CR_HSEBYP);
    SET_BIT(RCC->CR,RCC_CR_CSSON);
   // SET_BIT(RCC->CR, RCC_CR_PLLON);
    //while(READ_BIT(RCC->CR,RCC_CR_PLLRDY)){}
    MODIFY_REG(RCC->CFGR,RCC_CFGR_SW,RCC_CFGR_SW_PLL);//выбираем PLL в качестве systemClock
    MODIFY_REG(RCC->CFGR,RCC_CFGR_SWS,RCC_CFGR_SWS_PLL);//используем pll 
    MODIFY_REG(RCC->CFGR,RCC_CFGR_HPRE,RCC_CFGR_HPRE_DIV1);//AHB PRESCALER

    MODIFY_REG(FLASH->ACR,FLASH_ACR_LATENCY,FLASH_ACR_LATENCY_2);
    SET_BIT(FLASH->ACR,FLASH_ACR_PRFTBE);
    SET_BIT(FLASH->ACR,FLASH_ACR_PRFTBS);

    MODIFY_REG(RCC->CFGR,RCC_CFGR_PPRE1,RCC_CFGR_PPRE1_DIV2);
    */

    //включаем HSE
    SET_BIT(RCC->CR, RCC_CR_HSEON);//запускаем внутренний RC генератор на 8Мгц
    while(READ_BIT(RCC->CR, RCC_CR_HSERDY == RESET)) {}
    //включаем буфер предварительной выборки
    CLEAR_BIT(FLASH->ACR, FLASH_ACR_PRFTBE);
    SET_BIT(FLASH->ACR, FLASH_ACR_PRFTBE);
    MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_2);
    //настраиваем значения всех делителей
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV1);
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV2);
    //Настраиваем PLL на 9
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL,
    RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9);
    //Включаем PLL
    SET_BIT(RCC->CR, RCC_CR_PLLON);
    while(READ_BIT(RCC->CR, RCC_CR_PLLRDY) != (RCC_CR_PLLRDY)) {}
    //Выбираем PLL в качестве системного делителя
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);
    while(READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}

}