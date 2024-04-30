#include "stm32f10x.h"
#include "SysTick.h"

void SysTick_init(){
//1ms
MODIFY_REG(SysTick->LOAD,SysTick_LOAD_RELOAD_Msk,SYSCLOCK / 1000 - 1);
//сбрасываем счетчик
CLEAR_BIT(SysTick->VAL, SysTick_VAL_CURRENT_Msk);
//Настроим управляющий регистр, настроив в качестве источника неразделённую на 8 (оригинальную) системную частоту, разрешив прерывания и включив собственно счётчиk
SET_BIT(SysTick->CTRL, SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
}