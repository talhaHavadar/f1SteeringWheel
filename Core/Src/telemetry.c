/*
 * telemetry.c
 *
 *  Created on: 5 Nis 2020
 *      Author: talhaHavadar
 */
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "printf.h"
#include "gpio.h"
#include "usbd_cdc_if.h"
#include "dashboard.h"
#include "usart.h"


void StartShowTelemetryDataTask(void const * argument) {
	SteeringDashMessageStc msg = {0};

	printf("StartShowTelemetryDataTask started.\r\n");
	/* Infinite loop */
	for(;;)
	{
		if (HandleSteeringDashData() == pdPASS) {
			//HAL_UART_Transmit(&huart2, (uint8_t*) &steeringDashboard.rxBuffer[msg.index], msg.len, 100);
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		}
		// CDC_Transmit_FS(msg, sizeof(msg));
	}
}
