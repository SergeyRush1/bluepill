
#include "main.h"
#include "I2C_conf.h"
#include "stm32f10x.h"
#include "SSD1306x128x32.h"

#define SYSCLOCK 72000000u
__IO uint32_t SysTick_CNT = 0;
int status = 0;
volatile uint32_t Timeout_counter_ms;
int main(void)
{
CMSIS_RCC_CONFIG_72MGhz();
//RCC_Deinit();
//SetSysClockTo72();
//SystemInit();
SysTick_init();
CMSIS_I2C1_Init();
RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_IOPCEN;
GPIOC->CRH = (GPIOC->CRH)|(GPIO_CRH_MODE13_0|GPIO_CRH_MODE13_1);
GPIOC->CRH = (GPIOC->CRH) & ( ~( GPIO_CRH_CNF13_0 | GPIO_CRH_CNF13_1));
GPIOC->ODR = GPIO_ODR_ODR13;
GPIOC->CRH = (GPIOC->CRH)|(GPIO_CRH_MODE14_0|GPIO_CRH_MODE14_1);
GPIOC->CRH = (GPIOC->CRH) & ( ~( GPIO_CRH_CNF14_0 | GPIO_CRH_CNF14_1));
GPIOC->ODR = GPIO_ODR_ODR14;

/***************usb initialization*/
 // Set_System();
  //Set_USBClock();
//USB_Interrupts_Config();
  //USB_Init();
status = CMSIS_I2C_Adress_Device_Scan(I2C1,0x3C,100);
while (status!=1)
{
  status = CMSIS_I2C_Adress_Device_Scan(I2C1,0x3C,100);
}
for (uint8_t i = 0; i < sizeof(ssd1306_init_mass); i++)
{
  status = CMSIS_I2C_Set_command(I2C1,0x3C,ssd1306_init_mass[i],100);
  delay(1000);
  if (status!=1)
  {
     char rer[250] = {"попали в ловешку"};
  }
  
}



 
 
 

  while (1)
  {
    
    delay_ms(1000);
    status = CMSIS_I2C_Adress_Device_Scan(I2C1,0x3C,100);
    while(status==0)
    {
     CMSIS_I2C1_Init();
     status = CMSIS_I2C_Adress_Device_Scan(I2C1,0x3C,100);
    }


    
//    LCD_init();
  //  LCD_Print("B");

  //status = CMSIS_I2C_Data_Transmit(I2C1,0x3C,ssd1306_init,27 ,100);
  //GPIOC->ODR =(~GPIOC->ODR)& GPIO_ODR_ODR13; 
  GPIOC->BSRR|=GPIO_BSRR_BR13;
  GPIOC->BSRR|=GPIO_BSRR_BR14;
  delay_ms(1000);
  GPIOC->BSRR|= GPIO_BSRR_BS13;
  GPIOC->BSRR|= GPIO_BSRR_BS14;
  delay_ms(1000);
  }
}

void delay(__IO uint32_t tck)
{
  while(tck)
  {
    tck--;
  }  
}

void SysTick_Handler(void)
{
  if(SysTick_CNT > 0)  SysTick_CNT--;
  if(Timeout_counter_ms>0) Timeout_counter_ms--;
}

void delay_ms(uint32_t ms)
{
   MODIFY_REG(SysTick->VAL,SysTick_VAL_CURRENT_Msk,SYSCLOCK / 1000 - 1);
   SysTick_CNT = ms;
   while(SysTick_CNT) {}
}