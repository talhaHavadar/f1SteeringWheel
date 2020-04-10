#include "string.h"
#include "dashboard.h"
#include "usart.h"

SteeringDashboardStc steeringDashboard = {0};

BaseType_t ReadMessageFromSteeringDashQueue(SteeringDashMessageStc* pMsg) {
	static const TickType_t tenMs = portTICK_PERIOD_MS * 10;
	BaseType_t result = pdFAIL;
	result = xQueueReceive(steeringDashboard.rxQueue, pMsg, tenMs);
	return result;
}

BaseType_t SendMessageToSteeringDashFromISR(uint8_t* pData, uint32_t len) {
	BaseType_t result = pdFALSE;

	if (len <= DASHBOARD_RX_BUFFER_LENGTH) {
		SteeringDashMessageStc msg = {0};
		uint8_t availableRxIdx = 0;

		if(steeringDashboard.nextRxIndex + len > DASHBOARD_RX_BUFFER_LENGTH) {
			availableRxIdx = 0;
		} else {
			availableRxIdx = steeringDashboard.nextRxIndex;
		}

		memcpy((void*) &steeringDashboard.rxBuffer[availableRxIdx], pData, len);
		msg.len = len;
		msg.index = availableRxIdx;

		result = xQueueSendFromISR(steeringDashboard.rxQueue, &msg, NULL);
		if (result == pdTRUE) {
			steeringDashboard.nextRxIndex = availableRxIdx + len;
		}
	}

	return result;
}

#include "math.h"
extern void SetRpmPercentage(uint8_t percentage);

BaseType_t HandleSteeringDashData(void) {
	BaseType_t result = pdFAIL;
	SteeringDashMessageStc msg = {0};

	if ( (result = ReadMessageFromSteeringDashQueue(&msg) ) == pdPASS) {
		HAL_UART_Transmit(&huart2, (uint8_t*) &steeringDashboard.rxBuffer[msg.index], msg.len, 100);
		if (strncmp(&steeringDashboard.rxBuffer[msg.index], "rpm_val.val=", 12) == 0) {
			char* pValStart = &steeringDashboard.rxBuffer[msg.index + 12];
			int digitCount = (msg.len - 12 - 3);
			int val = 0;
			for (int i = 0; i < digitCount; ++i) {
				val += (pValStart[i] - '0') * pow(10, (digitCount - 1) - i);
			}

			SetRpmPercentage(val);
		}
	}

	return result;
}

void InitializeSteeringDashboard(void) {

	steeringDashboard.nextRxIndex = 0;
	steeringDashboard.rxQueue = xQueueCreate(DASHBOARD_RXQ_LENGTH, DASHBOARD_RXQ_ITEM_SIZE);


	// TODO: check if rxQueue is created successfully.
}
