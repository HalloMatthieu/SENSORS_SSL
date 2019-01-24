/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */


#include "stm32f4xx.h"
//#include "stm32f4_discovery.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"
#include "Optical_flow.h"


int main(void)
{
	uint8_t BUSY_OPTIC;
	long long delay;
	char data_optical;
	uint16_t data;
	uint8_t boucle;

	/*STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);*/

	/*Initialisation de la spi et des broches en sortie et mets le CS � 1*/
	init_clock_spi();
	init_spi_Optical();
	Config_Optical();
	config_port();
	GPIO_SetBits(GPIOB, GPIO_Pin_1);

	/*active la communication spi*/
	SPI_ENABLE_Optical();

	/*raz les flags de la spi*/
	SPI_CLEAR_BUSY();


	for(;;)
	{
		while(SPI_I2S_FLAG_TXE == 0);
		SPI_SENDDATA_Optical(0x55);
		SPI_CLEAR_BUSY();
	}
}