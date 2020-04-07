/*
 * ws2812.c
 *
 *  Created on: 7 Nis 2020
 *      Author: talhaHavadar
 */
#include "stm32f1xx_hal.h"
#include "ws2812.h"

static uint8_t LEDbuffer[LED_BUFFER_SIZE] = {0};

TIM_HandleTypeDef htim2 = {0};
DMA_HandleTypeDef hdma_tim2_ch1 = {0};

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* TIM2 init function */
static void MX_TIM2_Init(void)
{
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim2.Instance = TIM2;
  htim2.Init.Period = TIMER_PERIOD - 1;
  htim2.Init.Prescaler = (uint32_t)( (SystemCoreClock / TIMER_CLOCK_FREQ) - 1);
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim2);

  HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *) LEDbuffer, LED_BUFFER_SIZE);
}

void TurnOffAllLEDs(void)
{
	uint8_t* pLEDs = &LEDbuffer[RESET_SLOTS_BEGIN];
	for(int i = 0; i < LED_DATA_SIZE; i++)
	{
		pLEDs[i] = WS2812_0;
	}
}

void SetAllLEDsToWhite(void)
{
	uint8_t* pLEDs = &LEDbuffer[RESET_SLOTS_BEGIN];
	for(int i = 0; i < LED_DATA_SIZE; i++)
	{
		pLEDs[i] = WS2812_1;
	}
}

void SetLedColor(ColorStc* pColor, uint8_t ledIndex)
{
	ledIndex = ledIndex % LED_NUMBER;
	Ws2812LedStc* pLed = (Ws2812LedStc*) &LEDbuffer[RESET_SLOTS_BEGIN + (ledIndex * LED_DATA_SIZE_FOR_ONE)];
	for (int i = 0; i < 8; i++) {
		pLed->green[i] = ( (pColor->green << i) & 0x80) ? WS2812_1: WS2812_0;
	}
	for (int i = 0; i < 8; i++) {
		pLed->red[i] = ( (pColor->red << i) & 0x80) ? WS2812_1: WS2812_0;
	}
	for (int i = 0; i < 8; i++) {
		pLed->blue[i] = ( (pColor->blue << i) & 0x80) ? WS2812_1: WS2812_0;
	}
}

void Ws2812Init(void) {
	memset(LEDbuffer, 0, sizeof(LEDbuffer));
	TurnOffAllLEDs();
	MX_TIM2_Init();
}

