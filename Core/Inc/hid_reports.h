/*
 * hid_reports.h
 *
 *  Created on: 14 Nis 2020
 *      Author: talhaHavadar
 */

#ifndef INC_HID_REPORTS_H_
#define INC_HID_REPORTS_H_

typedef struct {
	uint32_t buttons;
	uint8_t axis[4];
} HIDGamepadReportTypeDef;

HIDGamepadReportTypeDef emptyHIDGamepadReport = {
	.buttons = 0,
	.axis = {0}
};

#endif /* INC_HID_REPORTS_H_ */
