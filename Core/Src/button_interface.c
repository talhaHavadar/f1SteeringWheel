/*
 * button_interface.c
 *
 *  Created on: 20 Nis 2020
 *      Author: talhaHavadar
 */
#include "gpio.h"
#include "button_interface.h"

void ScanButtons(uint32_t * pButtons)
{
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5))
	{
		*pButtons = (1ULL << 16);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
	}
}


void InitializeButtonInterface(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);

	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
