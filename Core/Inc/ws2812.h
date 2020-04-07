/*
 * ws2812.h
 *
 *  Created on: 7 Nis 2020
 *      Author: talhaHavadar
 */

#ifndef INC_WS2812_H_
#define INC_WS2812_H_

#include <stdio.h>
#include <string.h>
#include "stm32f1xx_hal.h"

//WS2812
#define WS2812_FREQ											(800000) 			// it is fixed: WS2812 require 800kHz
#define TIMER_CLOCK_FREQ									(16000000)   	// can be modified - multiples of 0.8MHz are suggested
#define TIMER_PERIOD										(TIMER_CLOCK_FREQ / WS2812_FREQ)
#define LED_NUMBER											(16)					// how many LEDs the MCU should control?
#define LED_DATA_SIZE_FOR_ONE								(24)
#define LED_DATA_SIZE										(LED_NUMBER * LED_DATA_SIZE_FOR_ONE)
#define RESET_SLOTS_BEGIN									(50)
#define RESET_SLOTS_END										(50)
#define WS2812_LAST_SLOT									(1)
#define LED_BUFFER_SIZE										(RESET_SLOTS_BEGIN + LED_DATA_SIZE + WS2812_LAST_SLOT + RESET_SLOTS_END)
#define WS2812_0											(TIMER_PERIOD / 3)				// WS2812's zero high time is long about one third of the period
#define WS2812_1											(TIMER_PERIOD * 2 / 3)		// WS2812's one high time is long about two thirds of the period
#define WS2812_RESET										(0)
#define COLOR_RED											(&(ColorStc){.red = 242, .green = 56, .blue = 10})
#define COLOR_GREEN											(&(ColorStc){.red = 91, .green = 242, .blue = 10})
#define COLOR_BLUE											(&(ColorStc){.red = 10, .green = 107, .blue = 242})
#define COLOR_YELLOW										(&(ColorStc){.red = 240, .green = 236, .blue = 31})
#define COLOR_ORANGE										(&(ColorStc){.red = 235, .green = 156, .blue = 0})

typedef struct {
	uint8_t green[8];
	uint8_t red[8];
	uint8_t blue[8];
} Ws2812LedStc;

typedef struct {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} ColorStc;

void TurnOffAllLEDs(void);
void SetAllLEDsToWhite(void);
void SetLedColor(ColorStc* pColor, uint8_t ledIndex);
void Ws2812Init(void);

#endif /* INC_WS2812_H_ */
