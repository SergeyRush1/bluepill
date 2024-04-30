#include "stm32f10x.h"
#include "stdint.h"
//#include "stdbool.h"x
//PB6 SCL
//PB7 SDA
#define CR2_FREQ_Reset          ((uint16_t)0xFFC0)
#define CCR_CCR_Set             ((uint16_t)0x0FFF)
void CMSIS_I2C1_Init(void);
void CMSIS_I2C_Reset(void);
int CMSIS_I2C_Adress_Device_Scan(I2C_TypeDef *I2C, uint8_t Adress_Device, uint32_t Timeout_ms);
int CMSIS_I2C_Data_Transmit(I2C_TypeDef *I2C, uint8_t Adress_Device, uint8_t* data, uint16_t Size_data, uint32_t Timeout_ms);
int CMSIS_I2C_Set_command(I2C_TypeDef *I2C, uint8_t Adress_Device, uint8_t* data, uint32_t Timeout_ms);