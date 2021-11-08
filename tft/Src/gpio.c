/*
 * gpio.c
 *
 *  Created on: Oct 23, 2021
 *      Author: Maria
 */


#include "gpio.h"


void GPIOA_init_spi_TFT(){
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	//PA5 - MOSI, PA7 - SCK , PA6 - MISO
	GPIOA->MODER |= GPIO_MODER_MODER5_1 | GPIO_MODER_MODER6_1| GPIO_MODER_MODER7_1;
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL5 | GPIO_AFRL_AFRL7| GPIO_AFRL_AFRL6);
	//for CS and other
	GPIOA->MODER |= GPIO_MODER_MODER4_0|GPIO_MODER_MODER2_0|GPIO_MODER_MODER3_0|GPIO_MODER_MODER1_0;
	SET_CS();
	RESET_IDLE();
}

void GPIOC_Set_Reset_PIN(int pin_num, int pin_state)
{
	GPIOC->BSRR = pin_state == 0 ? 1 << (pin_num + 16) : 1 << pin_num;
}


void GPIOC_DIODES_Init(){
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0;
}
