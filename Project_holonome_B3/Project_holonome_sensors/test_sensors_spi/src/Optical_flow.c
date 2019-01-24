/*
 * Optical_flow.c
 *
 *  Created on: 11 janv. 2019
 *      Author: pascal
 */

#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "Optical_flow.h"


void init_clock_spi(void)
{
	RCC_PCLK2Config(RCC_HCLK_Div2);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	/* APB2 correspond a la configuration de la clock en haute vitesse
	 * et fixe la valeur de fr�quence a 82 MHz
	 * on active donc la clock en haute vitesse pour le port spi1
	 */
}

void config_port(void)
{
	GPIO_DeInit(GPIOB);
	GPIO_InitTypeDef GPIO_Optical;
	GPIO_Optical.GPIO_Pin = GPIO_Pin_1;
	GPIO_Optical.GPIO_Mode = 0x01;
	GPIO_Init(GPIOB, &GPIO_Optical);

	GPIO_DeInit(GPIOA);
	GPIO_Optical.GPIO_Pin = GPIO_Pin_5;
	GPIO_Optical.GPIO_Mode = 0x01;
	GPIO_Init(GPIOA, &GPIO_Optical);

	GPIO_Optical.GPIO_Pin = GPIO_Pin_6;
	GPIO_Optical.GPIO_Mode = 0x00;
	GPIO_Init(GPIOA, &GPIO_Optical);

	GPIO_Optical.GPIO_Pin = GPIO_Pin_7;
	GPIO_Optical.GPIO_Mode = 0x01;
	GPIO_Init(GPIOA, &GPIO_Optical);
}

void init_spi_Optical(void)
{
	SPI_I2S_DeInit(SPI1);
	SPI_InitTypeDef SPI_Optical;
	SPI_Optical.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_Optical.SPI_Mode = SPI_Mode_Master;
	SPI_Optical.SPI_DataSize = SPI_DataSize_16b;
	SPI_Optical.SPI_CPOL = SPI_CPOL_Low;
	SPI_Optical.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_Optical.SPI_NSS = SPI_NSS_Hard;
	SPI_Optical.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_Optical.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Optical.SPI_CRCPolynomial = 0;
	SPI_Init(SPI1, &SPI_Optical);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef ChipEnable;
	ChipEnable.GPIO_Pin = GPIO_Pin_1;
	ChipEnable.GPIO_Mode = GPIO_Mode_OUT;

	/* Configure le port 2 de la spi, avec comme param�tres
	 * --> full duplex 2 lignes
	 * --> spi en mode maitre
	 * --> trame en format 8 bits
	 * --> etat de repos a l'etat haut
	 * --> capture sur deuxieme front
	 * --> desactive le slave select logiciel
	 * --> divise la frequence par 16 --> 5.25MHz
	 * --> configure le msb comme premier bit transmis
	 * --> desactive la CRC
	 *
	 * Configure le port B pin 1 en sortie pour le CS de la spi
	 */
}

void Config_Optical(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	for(uint16_t temps =0; temps<1000;temps++);
	SPI_I2S_SendData(SPI1,0x8A00);
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
}
void SPI_ENABLE_Optical(void)
{
	SPI_Cmd(SPI1, ENABLE);
	/*active la communication SPI sur le port 4*/
}

uint16_t SPI_RECEIVEDATA_Optical(volatile uint16_t data_optical)
{
	data_optical = SPI_I2S_ReceiveData(SPI1);
	return data_optical;
	/*fonction permettant la reception de la donn�e du capteur optique de
	 * souris
	 */
}

uint16_t SPI_SENDDATA_Optical(volatile uint16_t data_optical)
{
	SPI_I2S_SendData(SPI1, data_optical);
	return data_optical;
}

uint8_t SPI_BUSY_Optical(volatile uint8_t SPI_BUSY)
{
	SPI_BUSY = SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY);
	return SPI_BUSY;
	/*fonction permettant de savoir si la spi est libre*/
}

uint8_t SPI_RX_BUSY_Optical(volatile uint8_t SPI_RX_BUSY)
{
	SPI_RX_BUSY = SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE);
	return SPI_RX_BUSY;
	/*fonction permettant de savoir si la reception de la spi est libre*/
}

void SPI_CLEAR_BUSY(void)
{
	SPI_I2S_ClearFlag(SPI1, SPI_I2S_FLAG_BSY);
	SPI_I2S_ClearFlag(SPI1, SPI_I2S_FLAG_RXNE);
	/*remet � zero les flags de status*/
}
