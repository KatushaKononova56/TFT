/*
 * main.c
 *
 *  Created on: Oct 23, 2021
 *      Author: Maria
 */

#include "global.h"


uint8_t status_dma_tx=1;

uint8_t data_for_spi[8] = {0,0,0,0,0,0,0,0};
uint8_t data_for_spi_ready = 0;
uint8_t size_data_spi = 0;
uint16_t MAX_Y=240;
uint16_t MAX_X=320;

void clock48Mhz_init();

#define il9348

int main(void){
#ifdef il9348
	clock48Mhz_init();
	GPIOC_DIODES_Init();
	TFT_init();
	while (1)
	{

	}
#endif
}


void clock48Mhz_init() {
	RCC->CR &= ~RCC_CR_PLLON;
	while(RCC->CR & RCC_CR_PLLRDY);

	RCC->CFGR |= RCC_CFGR_PLLMUL12;

	RCC->CR |= RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);

	RCC->CFGR |= RCC_CFGR_SW_1;	//as PLL

	SystemCoreClockUpdate();
}

