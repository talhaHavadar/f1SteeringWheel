/*
 * dashboard.h
 *
 *  Created on: 9 Nis 2020
 *      Author: talhaHavadar
 */

#ifndef INC_DASHBOARD_H_
#define INC_DASHBOARD_H_
#include "FreeRTOS.h"
#include "queue.h"

#define DASHBOARD_RXQ_LENGTH		(16)
#define DASHBOARD_RX_BUFFER_LENGTH	(512)
#define DASHBOARD_RXQ_ITEM_SIZE		(sizeof(SteeringDashMessageStc) )

typedef struct {
	char rxBuffer[DASHBOARD_RX_BUFFER_LENGTH];
	uint8_t nextRxIndex;
	QueueHandle_t rxQueue;
} SteeringDashboardStc;

typedef struct {
	uint8_t index;
	uint32_t len;
} SteeringDashMessageStc;

extern SteeringDashboardStc steeringDashboard;

BaseType_t HandleSteeringDashData(void);
BaseType_t ReadMessageFromSteeringDashQueue(SteeringDashMessageStc* pMsg);
BaseType_t SendMessageToSteeringDashFromISR(uint8_t* pData, uint32_t len);
void InitializeSteeringDashboard(void);

#endif /* INC_DASHBOARD_H_ */
