/*
 * usbd_cdc_hid.c
 *
 *  Created on: 11 Nis 2020
 *      Author: talhaHavadar
 */
#include "usbd_cdc_hid.h"
#include "usbd_desc.h"
#include "usbd_ctlreq.h"

static uint8_t  USBD_CDC_HID_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t  USBD_CDC_HID_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t  USBD_CDC_HID_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t  USBD_CDC_HID_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t  USBD_CDC_HID_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t  USBD_CDC_HID_EP0_RxReady(USBD_HandleTypeDef *pdev);
static uint8_t* USBD_CDC_HID_GetFSCfgDesc(uint16_t *length);
static uint8_t* USBD_CDC_HID_GetDeviceQualifierDescriptor(uint16_t *length);

/* CDC interface class callbacks structure */
USBD_ClassTypeDef  USBD_CDC_HID =
{
	USBD_CDC_HID_Init,
	USBD_CDC_HID_DeInit,
	USBD_CDC_HID_Setup,
	NULL,                 /* EP0_TxSent, */
	USBD_CDC_HID_EP0_RxReady,
	USBD_CDC_HID_DataIn,
	USBD_CDC_HID_DataOut,
	NULL,
	NULL,
	NULL,
	USBD_CDC_HID_GetFSCfgDesc,
	USBD_CDC_HID_GetFSCfgDesc,
	USBD_CDC_HID_GetFSCfgDesc,
	USBD_CDC_HID_GetDeviceQualifierDescriptor,
};

/* USB CDC device Configuration Descriptor */
__ALIGN_BEGIN uint8_t USBD_CDC_HID_CfgFSDesc[USB_CDC_HID_CONFIG_DESC_SIZE] __ALIGN_END =
{
	0x09,   /* bLength: Configuration Descriptor size */
	USB_DESC_TYPE_CONFIGURATION,      /* bDescriptorType: Configuration */
	USB_CDC_HID_CONFIG_DESC_SIZE,                /* wTotalLength:no of returned bytes */
	0x00,
	0x03,   /* bNumInterfaces: 3 interface */
	0x01,   /* bConfigurationValue: Configuration value */
	0x02,   /* iConfiguration: Index of string descriptor describing the configuration */
	0xE0,   /* bmAttributes: self powered */
	0x32,   /* MaxPower 100 mA */
	0x08, /* bLength */ /******** IAD should be positioned just before the CDC interfaces ****** IAD to associate the two CDC interfaces */
	0x0B, /* bDescriptorType */
	USBD_CDC_INTERFACE_IDX, /* bFirstInterface */
	0x02, /* bInterfaceCount */
	0x02, /* bFunctionClass */
	0x02, /* bFunctionSubClass */
	0x01, /* bFunctionProtocol */
	0x00, /* iFunction (Index of string descriptor describing this function) */
	0x09,   /* bLength: Interface Descriptor size */ /*Interface Descriptor */
	USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: Interface */
	USBD_CDC_INTERFACE_IDX,   /* bInterfaceNumber: Number of Interface */
	0x00,   /* bAlternateSetting: Alternate setting */
	0x01,   /* bNumEndpoints: One endpoints used */
	0x02,   /* bInterfaceClass: Communication Interface Class */
	0x02,   /* bInterfaceSubClass: Abstract Control Model */
	0x01,   /* bInterfaceProtocol: Common AT commands */
	0x05,   /* iInterface: */
	0x05,   /* bLength: Endpoint Descriptor size */ /*Header Functional Descriptor*/
	0x24,   /* bDescriptorType: CS_INTERFACE */
	0x00,   /* bDescriptorSubtype: Header Func Desc */
	0x10,   /* bcdCDC: spec release number */
	0x01,
	0x05,   /* bFunctionLength */ /*Call Management Functional Descriptor*/
	0x24,   /* bDescriptorType: CS_INTERFACE */
	0x01,   /* bDescriptorSubtype: Call Management Func Desc */
	0x00,   /* bmCapabilities: D0+D1 */
	USBD_CDC_INTERFACE_IDX + 1,   /* bDataInterface: 1 */
	0x04,   /* bFunctionLength */ /*ACM Functional Descriptor*/
	0x24,   /* bDescriptorType: CS_INTERFACE */
	0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
	0x02,   /* bmCapabilities */
	0x05,   /* bFunctionLength */ /*Union Functional Descriptor*/
	0x24,   /* bDescriptorType: CS_INTERFACE */
	0x06,   /* bDescriptorSubtype: Union func desc */
	USBD_CDC_INTERFACE_IDX,   /* bMasterInterface: Communication class interface */
	USBD_CDC_INTERFACE_IDX + 1,   /* bSlaveInterface0: Data Class Interface */
	0x07, /* bLength: Endpoint Descriptor size */ /*Endpoint 2 Descriptor*/
	USB_DESC_TYPE_ENDPOINT,   /* bDescriptorType: Endpoint */
	USBD_CDC_CMD_EP,                     /* bEndpointAddress */
	0x03,                           /* bmAttributes: Interrupt */
	LOBYTE(CDC_CMD_PACKET_SIZE),     /* wMaxPacketSize: */
	HIBYTE(CDC_CMD_PACKET_SIZE),
	0x10,                           /* bInterval: */
	0x09,   /* bLength: Endpoint Descriptor size */ /*Data class interface descriptor*/
	USB_DESC_TYPE_INTERFACE,  /* bDescriptorType: */
	USBD_CDC_INTERFACE_IDX + 1,   /* bInterfaceNumber: Number of Interface */
	0x00,   /* bAlternateSetting: Alternate setting */
	0x02,   /* bNumEndpoints: Two endpoints used */
	0x0A,   /* bInterfaceClass: CDC */
	0x00,   /* bInterfaceSubClass: */
	0x00,   /* bInterfaceProtocol: */
	0x00,   /* iInterface: */
	0x07,   /* bLength: Endpoint Descriptor size */ /*Endpoint OUT Descriptor*/
	USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
	USBD_CDC_OUT_EP,                        /* bEndpointAddress */
	0x02,                              /* bmAttributes: Bulk */
	LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
	0x00,                              /* bInterval: ignore for Bulk transfer */
	0x07,   /* bLength: Endpoint Descriptor size */ /*Endpoint IN Descriptor*/
	USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
	USBD_CDC_IN_EP,                         /* bEndpointAddress */
	0x02,                              /* bmAttributes: Bulk */
	LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),  /* wMaxPacketSize: */
	HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
	0x00,                               /* bInterval: ignore for Bulk transfer */
	0x09,         /*bLength: Interface Descriptor size*/ /************** Descriptor of Joystick Mouse interface ****************/
	USB_DESC_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
	USBD_HID_INTERFACE_IDX,         /*bInterfaceNumber: Number of Interface*/
	0x00,         /*bAlternateSetting: Alternate setting*/
	0x01,         /*bNumEndpoints*/
	0x03,         /*bInterfaceClass: HID*/
	0x00,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
	0x00,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
	0x00,            /*iInterface: Index of string descriptor*/
	0x09,         /*bLength: HID Descriptor size*/ /******************** Descriptor of Joystick Mouse HID ********************/
	HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
	0x11,         /*bcdHID: HID Class Spec release number*/
	0x01,
	0x00,         /*bCountryCode: Hardware target country*/
	0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
	0x22,         /*bDescriptorType*/
	LOBYTE(USBD_CUSTOM_HID_REPORT_DESC_SIZE), /*wItemLength: Total length of Report descriptor*/
	HIBYTE(USBD_CUSTOM_HID_REPORT_DESC_SIZE),
	0x07,          /*bLength: Endpoint Descriptor size*/ /******************** Descriptor of Mouse endpoint ********************/
	USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/
	USBD_HID_IN_EP,     /*bEndpointAddress: Endpoint Address (IN)*/
	0x03,          /*bmAttributes: Interrupt endpoint*/
	HID_EPIN_SIZE, /*wMaxPacketSize: 64 Byte max */
	0x00,
	HID_FS_BINTERVAL,          /*bInterval: Polling Interval (10 ms)*/
	0x07,          /*bLength: Endpoint Descriptor size*/ /******************** Descriptor of Mouse endpoint ********************/
	USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/
	USBD_HID_OUT_EP,     /*bEndpointAddress: Endpoint Address (IN)*/
	0x03,          /*bmAttributes: Interrupt endpoint*/
	HID_EPIN_SIZE, /*wMaxPacketSize: 64 Byte max */
	0x00,
	HID_FS_BINTERVAL          /*bInterval: Polling Interval (10 ms)*/
};

static uint8_t  USBD_CDC_HID_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx) {

	uint8_t result = USBD_CDC_Init(pdev, cfgidx);
	if (result != USBD_OK) {
		return result;
	}

	result = USBD_HID_Init(pdev, cfgidx);

	return result;
}

static uint8_t  USBD_CDC_HID_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx) {
	USBD_CDC_DeInit(pdev, cfgidx);
	USBD_HID_DeInit(pdev, cfgidx);
	return USBD_OK;
}

static uint8_t  USBD_CDC_HID_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req) {

	// Route requests to HID interface or its endpoints
	if(((req->bmRequest & USB_REQ_RECIPIENT_MASK) == USB_REQ_RECIPIENT_INTERFACE && LOBYTE(req->wIndex) == USBD_HID_INTERFACE_IDX) ||
	   ((req->bmRequest & USB_REQ_RECIPIENT_MASK) == USB_REQ_RECIPIENT_ENDPOINT && ((req->wIndex & 0x7F) == USBD_HID_EP_IDX)))
	{
		return USBD_HID_Setup(pdev, req);
	}

	return USBD_CDC_Setup(pdev, req);
}

static uint8_t  USBD_CDC_HID_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum) {
	if (epnum == USBD_HID_EP_IDX) {
		return USBD_HID_DataIn(pdev, epnum);
	}

	return USBD_CDC_DataIn(pdev, epnum);
}

static uint8_t  USBD_CDC_HID_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum) {
	if (epnum == USBD_HID_EP_IDX) {
		return USBD_HID_DataOut(pdev, epnum);
	}

	return USBD_CDC_DataOut(pdev, epnum);
}

static uint8_t  USBD_CDC_HID_EP0_RxReady(USBD_HandleTypeDef *pdev) {
	return USBD_CDC_EP0_RxReady(pdev);
}

static uint8_t* USBD_CDC_HID_GetFSCfgDesc(uint16_t *length) {
	*length = USB_CDC_HID_CONFIG_DESC_SIZE;
	return USBD_CDC_HID_CfgFSDesc;
}

static uint8_t* USBD_CDC_HID_GetDeviceQualifierDescriptor(uint16_t *length) {
	return USBD_CDC_GetDeviceQualifierDescriptor(length);
}
