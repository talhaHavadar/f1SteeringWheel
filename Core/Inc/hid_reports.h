/*
 * hid_reports.h
 *
 *  Created on: 14 Nis 2020
 *      Author: talhaHavadar
 */

#ifndef INC_HID_REPORTS_H_
#define INC_HID_REPORTS_H_

#define HID_GAMEPAD_REPORT_ID	0x1

typedef struct {
	uint8_t packet_id;
	uint64_t buttons;
	uint16_t axis[6];
	uint8_t pov[4];
} HIDGamepadReportTypeDef;

HIDGamepadReportTypeDef emptyHIDGamepadReport = {
	.packet_id = HID_GAMEPAD_REPORT_ID,
	.buttons = 0,
	.axis = {0},
	.pov = {0}
};

#endif /* INC_HID_REPORTS_H_ */
