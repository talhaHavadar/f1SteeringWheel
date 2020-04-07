/*
 * rev_indicator.c
 *
 *  Created on: 7 Nis 2020
 *      Author: talhaHavadar
 */
#include "ws2812.h"

/**
 * First 4 LEDs => Green 1-25%
 * Next 2 LEDs => Yellow 26-37%
 * Next 3 LEDs => Orange 38-55%
 * Next 3 LEDs => Red 56-75%
 * Next 4 LEDs => Blue 76-100%
 */
void SetRpmPercentage(uint8_t percentage)
{
	TurnOffAllLEDs();
	int activeLedCount = 0;
	for (float i = 0.0; (i < 100) && (activeLedCount < LED_NUMBER); i += (100 / 16)) {
		if (i < percentage) {
			ColorStc* pColor = NULL;
			if (activeLedCount < 3) {
				pColor = COLOR_GREEN;
			} else if (activeLedCount < 5) {
				pColor = COLOR_YELLOW;
			} else if (activeLedCount < 8) {
				pColor = COLOR_ORANGE;
			} else if (activeLedCount < 14) {
				pColor = COLOR_RED;
			} else if (activeLedCount < 16) {
				pColor = COLOR_BLUE;
			}
			SetLedColor(pColor, activeLedCount++);
		}
	}
}

