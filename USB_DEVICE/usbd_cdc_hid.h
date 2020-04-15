/*
 * usbd_cdc_hid.h
 *
 *  Created on: 11 Nis 2020
 *      Author: talhaHavadar
 */

#ifndef USBD_CDC_HID_H_
#define USBD_CDC_HID_H_

#include  "usbd_ioreq.h"
#include "usbd_cdc.h"
#include "usbd_hid.h"

#define USBD_HID_INTERFACE_IDX	(0x2)
#define USBD_CDC_INTERFACE_IDX	(0x0)

#define USBD_CDC_CMD_EP_IDX		(0x1)
#define USBD_CDC_EP_IDX			(0x2)
#define USBD_HID_EP_IDX			(0x3)

#define USBD_IN_EP_DIR			(0x80)

#define USBD_CDC_CMD_EP			(USBD_CDC_CMD_EP_IDX | USBD_IN_EP_DIR)
#define USBD_CDC_OUT_EP			(USBD_CDC_EP_IDX)
#define USBD_CDC_IN_EP			(USBD_CDC_EP_IDX | USBD_IN_EP_DIR)
#define USBD_HID_IN_EP			(USBD_HID_EP_IDX | USBD_IN_EP_DIR)
#define USBD_HID_OUT_EP			(USBD_HID_EP_IDX)

#define USB_CDC_HID_CONFIG_DESC_SIZE		107
typedef struct {
	USBD_CDC_HandleTypeDef cdc;
	USBD_HID_HandleTypeDef hid;
} USBD_CDC_HID_HandleTypeDef;

#endif /* USBD_CDC_HID_H_ */
