#include "I2C_conf.h"
#include "stm32f10x.h"
#include "stdint.h"
#include "stdio.h"
#include "main.h"
extern volatile uint32_t Timeout_counter_ms;

#define I2C_CCR_CCR_Pos                     (0U)                               
#define I2C_CCR_CCR_Msk                     (0xFFFUL << I2C_CCR_CCR_Pos)        /*!< 0x00000FFF */
#define I2C_CR2_FREQ_Pos                    (0U)                               
#define I2C_CR2_FREQ_Msk                    (0x3FUL << I2C_CR2_FREQ_Pos)        /*!< 0x0000003F */
#define I2C_TRISE_TRISE_Pos                 (0U)                               
#define I2C_TRISE_TRISE_Msk                 (0x3FUL << I2C_TRISE_TRISE_Pos)     /*!< 0x0000003F */

void CMSIS_I2C1_Init(void) {
	//Настройки тактирования
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN); //Запуск тактирование порта B
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN); //Запуск тактирования альтернативных функций
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C1EN); //Запуск тактирования I2C1
    	//Настройки ножек SDA и SCL
	//PB7 SDA (I2C Data I/O) Alternate function open drain
    GPIOB->CRL|= GPIO_CRL_MODE7_0|GPIO_CRL_MODE7_1;
    GPIOB->CRL|= GPIO_CRL_CNF7_0|GPIO_CRL_CNF7_1;

    GPIOB->CRL|=GPIO_CRL_MODE6_0|GPIO_CRL_MODE6_1;
    GPIOB->CRL|= GPIO_CRL_CNF6_0|GPIO_CRL_CNF6_1;
    /*
	MODIFY_REG(GPIOB->CRL, GPIO_CRL_CNF7_Msk, 0b11 << GPIO_CRL_CNF7_Pos); //Alternate function open drain
	MODIFY_REG(GPIOB->CRL, GPIO_CRL_MODE7_Msk, 0b11 << GPIO_CRL_MODE7_Pos); //Maximum output speed 50 MHz
	//PB6 SCL (I2C clock) Alternate function open drain
	MODIFY_REG(GPIOB->CRL, GPIO_CRL_CNF6_Msk, 0b11 << GPIO_CRL_CNF6_Pos); //Alternate function open drain
	MODIFY_REG(GPIOB->CRL, GPIO_CRL_MODE6_Msk, 0b11 << GPIO_CRL_MODE6_Pos); //Maximum output speed 50 MHz
    */

   	/*Это все для инита не нужно. После сброса итак будет в 0. */
	CLEAR_BIT(I2C1->CR1, I2C_CR1_ALERT); //Releases SMBA pin high.Alert Response Address Header followed by NACK
	CLEAR_BIT(I2C1->CR1, I2C_CR1_PEC); //No PEC transfer
	CLEAR_BIT(I2C1->CR1, I2C_CR1_POS); //ACK bit controls the (N)ACK of the current byte being received in the shift register
	CLEAR_BIT(I2C1->CR1, I2C_CR1_ACK); //No acknowledge returned
	CLEAR_BIT(I2C1->CR1, I2C_CR1_STOP); //No Stop generation
	CLEAR_BIT(I2C1->CR1, I2C_CR1_START); //No Start generation
	CLEAR_BIT(I2C1->CR1, I2C_CR1_NOSTRETCH); //Clock stretching enabled
	CLEAR_BIT(I2C1->CR1, I2C_CR1_ENGC); //General call disabled. Address 00h is NACKed.
	CLEAR_BIT(I2C1->CR1, I2C_CR1_ENPEC); //PEC calculation disabled
	CLEAR_BIT(I2C1->CR1, I2C_CR1_ENARP); //ARP disable
	CLEAR_BIT(I2C1->CR1, I2C_CR1_SMBTYPE); //SMBus Device
	CLEAR_BIT(I2C1->CR1, I2C_CR1_SMBUS); //I2C mode
	
	//п.п. 26.6.2 I2C Control register 2(I2C_CR2)(стр.774)
	CLEAR_BIT(I2C1->CR2, I2C_CR2_LAST); //Next DMA EOT is not the last transfer
	CLEAR_BIT(I2C1->CR2, I2C_CR2_DMAEN); //DMA requests disabled
	CLEAR_BIT(I2C1->CR2, I2C_CR2_ITBUFEN); //TxE = 1 or RxNE = 1 does not generate any interrupt.
	CLEAR_BIT(I2C1->CR2, I2C_CR2_ITEVTEN); //Event interrupt disabled
	CLEAR_BIT(I2C1->CR2, I2C_CR2_ITERREN); //Error interrupt disabled
	//MODIFY_REG(I2C1->CR2, I2C_CR2_FREQ_Msk, 36 << I2C_CR2_FREQ_Pos); //f PCLK1 = 36 Мгц
	I2C1->CR2&=0xFFC0;
	I2C1->CR2|=36;
		
	//п.п. 26.6.3 I2C Own address register 1(I2C_OAR1)(стр.776)
	I2C1->OAR1 = 0;
	//п.п. 26.6.4 I2C Own address register 1(I2C_OAR2)(стр.776)
	I2C1->OAR2 = 0;
	
	//п.п. 26.6.8 I2C Clock control register (I2C_CCR)(стр.781)
	//CLEAR_BIT(I2C1->CCR, I2C_CCR_FS); //Standard mode I2C
	SET_BIT(I2C1->CCR, I2C_CCR_FS); //Fast mode I2C

	//CLEAR_BIT(I2C1->CCR, I2C_CCR_DUTY); //Fm mode tlow/thigh = 2
	SET_BIT(I2C1->CCR, I2C_CCR_DUTY); //Fm mode tlow/thigh = 16/9 (see CCR)
	
	//Расчет CCR. Смотри примеры расчета
	//MODIFY_REG(I2C1->CCR, I2C_CCR_CCR_Msk, 180 << I2C_CCR_CCR_Pos); //для Sm mode
	MODIFY_REG(I2C1->CCR, I2C_CCR_CCR_Msk, 30 << I2C_CCR_CCR_Pos); //для Fm mode. DUTY 0.
	//MODIFY_REG(I2C1->CCR, I2C_CCR_CCR_Msk, 4 << I2C_CCR_CCR_Pos); //для Fm mode. DUTY 1.
	
	//п.п. 26.6.9 I2C TRISE register (I2C_TRISE)(стр. 782)
	//MODIFY_REG(I2C1->TRISE, I2C_TRISE_TRISE_Msk, 37 << I2C_TRISE_TRISE_Pos); //для Sm mode
	MODIFY_REG(I2C1->TRISE, I2C_TRISE_TRISE_Msk, 12 << I2C_TRISE_TRISE_Pos); //для Fm mode
	
	SET_BIT(I2C1->CR1, I2C_CR1_PE); //I2C1 enable
}

void CMSIS_I2C_Reset(void) {
	//п.п. 26.6.1 I2C Control register 1 (I2C_CR1) (стр. 772)
	SET_BIT(I2C1->CR1, I2C_CR1_SWRST); //: I2C Peripheral not under reset
	while (READ_BIT(I2C1->CR1, I2C_CR1_SWRST) == 0) ;
	CLEAR_BIT(I2C1->CR1, I2C_CR1_SWRST); //: I2C Peripheral not under reset
	while (READ_BIT(I2C1->CR1, I2C_CR1_SWRST)) ;
	/* Примечание: Этот бит можно использовать для повторной инициализации 
	 * периферийного устройства после ошибки или заблокированного состояния.
	 * Например, если бит BUSY установлен и остается заблокированным из-за сбоя на шине,
	 * бит SWRST можно использовать для выхода из этого состояния.*/
}


int CMSIS_I2C_Adress_Device_Scan(I2C_TypeDef *I2C, uint8_t Adress_Device, uint32_t Timeout_ms) {
	
	/*-------------------Проверка занятости шины-------------------*/
	if (READ_BIT(I2C->SR2, I2C_SR2_BUSY)) {
		//Если шина занята
		
		if ((READ_BIT(GPIOB->IDR, GPIO_IDR_IDR6)) && (READ_BIT(GPIOB->IDR, GPIO_IDR_IDR7))) {
			//Если линия на самом деле свободна, а BUSY висит
			CMSIS_I2C_Reset(); //ресет
			CMSIS_I2C1_Init(); //повторная инициализация
		} 
		
		if (READ_BIT(I2C->SR2, I2C_SR2_MSL)) {
			//Если стоит статус, что мы в мастере
			SET_BIT(I2C->CR1, I2C_CR1_STOP); //Отправим сигнал STOP
		} 
		
		if (I2C->CR1 != 1) {
			//Если в CR1 что-то лишнее, то перезагрузим I2C
			CLEAR_BIT(I2C->CR1, I2C_CR1_PE);
			SET_BIT(I2C->CR1, I2C_CR1_PE);
		} 	
		
		return 0;	
	}
	/*-------------------Проверка занятости шины-------------------*/
	
	CLEAR_BIT(I2C->CR1, I2C_CR1_POS); //Бит ACK управляет (N)ACK текущего байта, принимаемого в сдвиговом регистре.
	SET_BIT(I2C->CR1, I2C_CR1_START); //Отправляем сигнал START
	
	Timeout_counter_ms = Timeout_ms;
	while (READ_BIT(I2C->SR1, I2C_SR1_SB) == 0) {
		//Ожидаем до момента, пока не сработает Start condition generated
	
		if (!Timeout_counter_ms) {
			return 0;
		}
		
	} 
	//ВНИМАНИЕ!
	/* Бит I2C_SR1_SB очищается программно путем чтения регистра SR1 с последующей записью данных в регистр DR или когда PE=0*/
	I2C->SR1;
	I2C->DR = (Adress_Device << 1); //Адрес + Write
	
	Timeout_counter_ms = Timeout_ms;
	while ((READ_BIT(I2C->SR1, I2C_SR1_AF) == 0) && (READ_BIT(I2C->SR1, I2C_SR1_ADDR) == 0)) {
		//Ждем, пока адрес отзовется
		
		if (!Timeout_counter_ms) {
			return 0;
		}
		
	}
	
	if (READ_BIT(I2C->SR1, I2C_SR1_ADDR)) {
		//Если устройство отозвалось
		SET_BIT(I2C->CR1, I2C_CR1_STOP); //Отправляем сигнал STOP
		/*Сброс бита ADDR производится чтением SR1, а потом SR2*/
		I2C->SR1;
		I2C->SR2;
		return 1;
	} else {
		//Если устройство не отозвалось, прилетит 1 в I2C_SR1_AF 
		SET_BIT(I2C->CR1, I2C_CR1_STOP); //Отправляем сигнал STOP
		CLEAR_BIT(I2C->SR1, I2C_SR1_AF); //Сбрасываем бит AF
		return 0;
	}
}

/**
 **************************************************************************************************
 *  @breif Функция передачи данных по I2C
 *  @param  *I2C - шина I2C
 *  @param  Adress_Device - Адрес устройства      
 *  @param  *data - Данные, которые будем отправлять
 *  @param  Size_data - Размер, сколько байт будем отправлять.
 *  @retval  Возвращает статус отправки данных. True - Успешно. False - Ошибка.
 **************************************************************************************************
 */
int CMSIS_I2C_Data_Transmit(I2C_TypeDef *I2C, uint8_t Adress_Device, uint8_t* data, uint16_t Size_data, uint32_t Timeout_ms) {
	
	/*-------------------Проверка занятости шины-------------------*/
	if (READ_BIT(I2C->SR2, I2C_SR2_BUSY)) {
		//Если шина занята
		
		if ((READ_BIT(GPIOB->IDR, GPIO_IDR_IDR6)) && (READ_BIT(GPIOB->IDR, GPIO_IDR_IDR7))) {
			//Если линия на самом деле свободна, а BUSY висит
			CMSIS_I2C_Reset(); //ресет
			CMSIS_I2C1_Init(); //повторная инициализация
		} 
		
		if (READ_BIT(I2C->SR2, I2C_SR2_MSL)) {
			//Если стоит статус, что мы в мастере
			SET_BIT(I2C->CR1, I2C_CR1_STOP); //Отправим сигнал STOP
		} 
		
		if (I2C->CR1 != 1) {
			//Если в CR1 что-то лишнее, то перезагрузим I2C
			CLEAR_BIT(I2C->CR1, I2C_CR1_PE);
			SET_BIT(I2C->CR1, I2C_CR1_PE);
		} 	
		
		return 0;	
	}
	/*-------------------Проверка занятости шины-------------------*/
	
	CLEAR_BIT(I2C->CR1, I2C_CR1_POS); //Бит ACK управляет (N)ACK текущего байта, принимаемого в сдвиговом регистре.
	SET_BIT(I2C->CR1, I2C_CR1_START); //Стартуем.
	
	Timeout_counter_ms = Timeout_ms;
	while (READ_BIT(I2C->SR1, I2C_SR1_SB) == 0) {
		//Ожидаем до момента, пока не сработает Start condition generated
	
		if (!Timeout_counter_ms) {
			return 0;
		}
		
	} 
	//ВНИМАНИЕ!
	/* Бит I2C_SR1_SB очищается программно путем чтения регистра SR1 с последующей записью в регистр DR или когда PE=0*/
	I2C->SR1;
	I2C->DR = (Adress_Device << 1); //Адрес + Write
	
	Timeout_counter_ms = Timeout_ms;
	while ((READ_BIT(I2C->SR1, I2C_SR1_AF) == 0) && (READ_BIT(I2C->SR1, I2C_SR1_ADDR) == 0)) {
		//Ждем, пока адрес отзовется
		
		if (!Timeout_counter_ms) {
			return 0;
		}
		
	}
	
	if (READ_BIT(I2C->SR1, I2C_SR1_ADDR)) {
		//Если устройство отозвалось, сбросим бит ADDR
		/*Сброс бита ADDR производится чтением SR1, а потом SR2*/
		I2C->SR1;
		I2C->SR2;
		
		/*Отправим данные*/
		for (uint16_t i = 0; i < Size_data; i++) {
			I2C->DR = *(data + i); //Запись байта
			while (READ_BIT(I2C->SR1, I2C_SR1_TXE) == 0) {
				//Ждем, пока данные загрузятся в регистр сдвига.
				
				if ((READ_BIT(I2C->SR1, I2C_SR1_AF) == 1)) {
					//Если устройство не отозвалось, прилетит 1 в I2C_SR1_AF 
					SET_BIT(I2C->CR1, I2C_CR1_STOP); //Останавливаем
					CLEAR_BIT(I2C->SR1, I2C_SR1_AF); //Сбрасываем бит AF
					return 0;
				}
			} 
		}
		
		SET_BIT(I2C->CR1, I2C_CR1_STOP); //Останавливаем

		return 1;
	
	} else {
		//Если устройство не отозвалось, прилетит 1 в I2C_SR1_AF 
		SET_BIT(I2C->CR1, I2C_CR1_STOP); //Останавливаем
		CLEAR_BIT(I2C->SR1, I2C_SR1_AF); //Сбрасываем бит AF
		
		return 0;
	}
}

int CMSIS_I2C_Set_command(I2C_TypeDef *I2C, uint8_t Adress_Device, uint8_t* data, uint32_t Timeout_ms) {
	
	/*-------------------Проверка занятости шины-------------------*/
	if (READ_BIT(I2C->SR2, I2C_SR2_BUSY)) {
		//Если шина занята
		
		if ((READ_BIT(GPIOB->IDR, GPIO_IDR_IDR6)) && (READ_BIT(GPIOB->IDR, GPIO_IDR_IDR7))) {
			//Если линия на самом деле свободна, а BUSY висит
			CMSIS_I2C_Reset(); //ресет
			CMSIS_I2C1_Init(); //повторная инициализация
		} 
		
		if (READ_BIT(I2C->SR2, I2C_SR2_MSL)) {
			//Если стоит статус, что мы в мастере
			SET_BIT(I2C->CR1, I2C_CR1_STOP); //Отправим сигнал STOP
		} 
		
		if (I2C->CR1 != 1) {
			//Если в CR1 что-то лишнее, то перезагрузим I2C
			CLEAR_BIT(I2C->CR1, I2C_CR1_PE);
			SET_BIT(I2C->CR1, I2C_CR1_PE);
		} 	
		
		return 0;	
	}
	/*-------------------Проверка занятости шины-------------------*/
	
	CLEAR_BIT(I2C->CR1, I2C_CR1_POS); //Бит ACK управляет (N)ACK текущего байта, принимаемого в сдвиговом регистре.
	SET_BIT(I2C->CR1, I2C_CR1_START); //Стартуем.
	
	Timeout_counter_ms = Timeout_ms;
	while (READ_BIT(I2C->SR1, I2C_SR1_SB) == 0) {
		//Ожидаем до момента, пока не сработает Start condition generated
	
		if (!Timeout_counter_ms) {
			return 0;
		}
		
	} 
	//ВНИМАНИЕ!
	/* Бит I2C_SR1_SB очищается программно путем чтения регистра SR1 с последующей записью в регистр DR или когда PE=0*/
	I2C->SR1;
	I2C->DR = (Adress_Device << 1); //Адрес + Write
	
	Timeout_counter_ms = Timeout_ms;
	while ((READ_BIT(I2C->SR1, I2C_SR1_AF) == 0) && (READ_BIT(I2C->SR1, I2C_SR1_ADDR) == 0)) {
		//Ждем, пока адрес отзовется
		
		if (!Timeout_counter_ms) {
			return 0;
		}
		
	}
	
	if (READ_BIT(I2C->SR1, I2C_SR1_ADDR)) {
		//Если устройство отозвалось, сбросим бит ADDR
		/*Сброс бита ADDR производится чтением SR1, а потом SR2*/
		I2C->SR1;
		I2C->SR2;
		
		/*Отправим 0*/
		I2C->DR = 0x00; //Запись байта
			while (READ_BIT(I2C->SR1, I2C_SR1_TXE) == 0) {
				//Ждем, пока данные загрузятся в регистр сдвига.
				
				if ((READ_BIT(I2C->SR1, I2C_SR1_AF) == 1)) {
					//Если устройство не отозвалось, прилетит 1 в I2C_SR1_AF 
					SET_BIT(I2C->CR1, I2C_CR1_STOP); //Останавливаем
					CLEAR_BIT(I2C->SR1, I2C_SR1_AF); //Сбрасываем бит AF
					return 0;
				}
			} 

			I2C->DR = *(data); //Запись байта
			while (READ_BIT(I2C->SR1, I2C_SR1_TXE) == 0) {
				//Ждем, пока данные загрузятся в регистр сдвига.
				
				if ((READ_BIT(I2C->SR1, I2C_SR1_AF) == 1)) {
					//Если устройство не отозвалось, прилетит 1 в I2C_SR1_AF 
					SET_BIT(I2C->CR1, I2C_CR1_STOP); //Останавливаем
					CLEAR_BIT(I2C->SR1, I2C_SR1_AF); //Сбрасываем бит AF
					return 0;
				}
			} 
		
		
		SET_BIT(I2C->CR1, I2C_CR1_STOP); //Останавливаем

		return 1;
	
	} else {
		//Если устройство не отозвалось, прилетит 1 в I2C_SR1_AF 
		SET_BIT(I2C->CR1, I2C_CR1_STOP); //Останавливаем
		CLEAR_BIT(I2C->SR1, I2C_SR1_AF); //Сбрасываем бит AF
		
		return 0;
	}
}


