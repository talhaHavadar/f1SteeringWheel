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

void StartShowTelemetryDataTask(void const * argument) {
	printf("StartShowTelemetryDataTask started.\r\n");
	char msg[] = "Serial Test\r\n";
	/* Infinite loop */
	for(;;)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		printf("Serial Test\r\n");
		CDC_Transmit_FS(msg, sizeof(msg));
		osDelay(1000);
	}
}
