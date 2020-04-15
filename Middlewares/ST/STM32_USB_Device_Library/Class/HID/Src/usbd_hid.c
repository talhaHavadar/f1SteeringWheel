/**
  ******************************************************************************
  * @file    usbd_hid.c
  * @author  MCD Application Team
  * @version V2.4.2
  * @date    11-December-2015
  * @brief   This file provides the HID core functions.
  *
  * @verbatim
  *      
  *          ===================================================================      
  *                                HID Class  Description
  *          =================================================================== 
  *           This module manages the HID class V1.11 following the "Device Class Definition
  *           for Human Interface Devices (HID) Version 1.11 Jun 27, 2001".
  *           This driver implements the following aspects of the specification:
  *             - The Boot Interface Subclass
  *             - The Mouse protocol
  *             - Usage Page : Generic Desktop
  *             - Usage : Joystick
  *             - Collection : Application 
  *      
  * @note     In HS mode and when the DMA is used, all variables and data structures
  *           dealing with the DMA during the transaction process should be 32-bit aligned.
  *           
  *      
  *  @endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_hid.h"
#include "usbd_hid.h"
#include "usbd_desc.h"
#include "usbd_ctlreq.h"


/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_HID
  * @brief usbd core module
  * @{
  */ 

/** @defgroup USBD_HID_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBD_HID_Private_Defines
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup USBD_HID_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 




/** @defgroup USBD_HID_Private_Variables
  * @{
  */ 

USBD_ClassTypeDef  USBD_HID =
{
  USBD_HID_Init,
  USBD_HID_DeInit,
  USBD_HID_Setup,
  NULL, /*EP0_TxSent*/  
  NULL, /*EP0_RxReady*/
  USBD_HID_DataIn, /*DataIn*/
  NULL, /*DataOut*/
  NULL, /*SOF */
  NULL,
  NULL,      
  NULL,
  NULL,
  NULL,
  USBD_HID_GetDeviceQualifierDesc,
};

/* USB HID device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_HID_Desc[USB_HID_DESC_SIZ]  __ALIGN_END  =
{
  /* 18 */
	0x09,         /*bLength: HID Descriptor size*/
	HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
	0x11,         /*bcdHID: HID Class Spec release number*/
	0x01,
	0x00,         /*bCountryCode: Hardware target country*/
	0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
	0x22,         /*bDescriptorType*/
	LOBYTE(USBD_CUSTOM_HID_REPORT_DESC_SIZE), /*wItemLength: Total length of Report descriptor*/
	HIBYTE(USBD_CUSTOM_HID_REPORT_DESC_SIZE)
};

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_HID_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC]  __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
		// Keyboard
	    	0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
			0x09, 0x06,                    // USAGE (Keyboard)
		    0xa1, 0x01,                    // COLLECTION (Application)
		    0x85, 0x07,                    //   REPORT_ID (7)
		    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
		    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
		    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
		    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
		    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
		    0x75, 0x01,                    //   REPORT_SIZE (1)
		    0x95, 0x08,                    //   REPORT_COUNT (8)
		    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
		    0x95, 0x01,                    //   REPORT_COUNT (1)
		    0x75, 0x08,                    //   REPORT_SIZE (8)
		    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
		    0x95, 0x06,                    //   REPORT_COUNT (6)
		    0x75, 0x08,                    //   REPORT_SIZE (8)
		    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
		    0x25, 0xFF,                    //   LOGICAL_MAXIMUM (255)
		    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
		    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
		    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
		    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
			0xc0,                          // END_COLLECTION
			// System Control
//			0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
//		    0x09, 0x80,                    // USAGE (System Control)
//		    0xa1, 0x01,                    // COLLECTION (Application)
//		    0x85, 0x08,                    //   REPORT_ID (8)
//		    0x19, 0x81,                    //   USAGE_MINIMUM (System Power Down)
//		    0x29, 0x83,                    //   USAGE_MAXIMUM (System Wake Up)
//		    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
//		    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
//		    0x95, 0x03,                    //   REPORT_COUNT (3)
//		    0x75, 0x01,                    //   REPORT_SIZE (1)
//		    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
//		    0x95, 0x01,                    //   REPORT_COUNT (1)
//		    0x75, 0x05,                    //   REPORT_SIZE (5)
//		    0x81, 0x01,                    //   INPUT (Cnst,Ary,Abs)
//			0xc0,                          //     END_COLLECTION
			// Mouse
			0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
			0x09, 0x02,                    // USAGE (Mouse)
			0xa1, 0x01,                    // COLLECTION (Application)
			0x85, 0x06,                    //   REPORT_ID (6)
			0x09, 0x01,                    //   USAGE (Pointer)
			0x05, 0x09,                    //     USAGE_PAGE (Button)
			0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
			0x29, 0x08,                    //     USAGE_MAXIMUM (Button 8)
			0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
			0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
			0x95, 0x08,                    //     REPORT_COUNT (8)
			0x75, 0x01,                    //     REPORT_SIZE (1)
			0x81, 0x02,                    //     INPUT (Data,Var,Abs)
			0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
			0x09, 0x30,                    //     USAGE (X)
			0x09, 0x31,                    //     USAGE (Y)
			0x09, 0x38,                    //     USAGE (Wheel)
			0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
			0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
			0x75, 0x08,                    //     REPORT_SIZE (8)
			0x95, 0x03,                    //     REPORT_COUNT (3)
			0x81, 0x06,                    //     INPUT (Data,Var,Rel)
		    0xc0  ,                         // END_COLLECTION
			// Gamepad
			0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
			0x09, 0x05,                    // USAGE (Game Pad)
			0xa1, 0x01,                    // COLLECTION (Application)
			0x85, 0x01,                    //   REPORT_ID (1)
			0x05, 0x09,                    //     USAGE_PAGE (Button)
			0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
			0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
			0x75, 0x01,                    //     REPORT_SIZE (1)
			0x95, 0x40,                    //     REPORT_COUNT (64)
			0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
			0x29, 0x40,                    //     USAGE_MAXIMUM (Button 64)
			0x81, 0x02,                    //     INPUT (Data,Var,Abs)
			0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
			0x09, 0x33,                    //     USAGE (Rx)
			0x09, 0x34,                    //     USAGE (Ry)
			0x09, 0x35,                    //     USAGE (Rz)
			0x09, 0x36,                    //     USAGE (Slider1)
			0x09, 0x36,                    //     USAGE (Slider2)
			0x09, 0x32,                    //     USAGE (Z)
			0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
			0x26, 0xff, 0x0f,              //     LOGICAL_MAXIMUM (4095)
			0x75, 0x10,                    //     REPORT_SIZE (16)
			0x95, 0x06,                    //     REPORT_COUNT (6)
			0x81, 0x02,                    //     INPUT (Data,Var,Abs)
			0x09, 0x39,                    //   USAGE (Hat switch)
			0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
			0x25, 0x07,                    //   LOGICAL_MAXIMUM (7)
			0x35, 0x00,                    //   PHYSICAL_MINIMUM (0)
			0x46, 0x3b, 0x01,              //   PHYSICAL_MAXIMUM (315)
			0x65, 0x12,                    //   UNIT (SI Rot:Angular Pos)
			0x75, 0x08,                    //   REPORT_SIZE (8)
			0x95, 0x01,                    //   REPORT_COUNT (1)
			0x81, 0x02,                    //   INPUT (Data,Var,Abs)
			0x09, 0x39,                    //   USAGE (Hat switch)
			0x81, 0x02,                    //   INPUT (Data,Var,Abs)
			0x09, 0x39,                    //   USAGE (Hat switch)
			0x81, 0x02,                    //   INPUT (Data,Var,Abs)
			0x09, 0x39,                    //   USAGE (Hat switch)
			0x81, 0x02,                    //   INPUT (Data,Var,Abs)
			// Custom Stuff
			0x06, 0x00, 0xff,              //   USAGE_PAGE (Vendor Defined Page 1)
			0x85, 0x02,                    //   REPORT_ID (2)
			0x09, 0x01,                    //   USAGE (Vendor Usage 1)
			0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
			0x26, 0xff, 0x00,              //   LOGICAL_MAXIMUM (255)
			0x95, 0x3F,                     //   REPORT_COUNT (63)
			0x75, 0x08,                    //   REPORT_SIZE (8)
			0x91, 0x82,                    //   OUTPUT (Data,Var,Abs,Vol)
			0x85, 0x05,                    //   REPORT_ID (5)
			0x09, 0x04,                    //   USAGE (Vendor Usage 4)
			0x81, 0x82,                    //   INPUT (Data,Var,Abs,Vol)
			0x85, 0x04,                    //   REPORT_ID (4)
			0x09, 0x03,                    //   USAGE (Vendor Usage 3)
			0x81, 0x82,                    //   INPUT (Data,Var,Abs,Vol)
			0x85, 0x03,                    //   REPORT_ID (3)
			0x09, 0x02,                    //   USAGE (Vendor Usage 2)
			0x95, 0x01,                    //   REPORT_COUNT (1)
			0x75, 0x08,                    //   REPORT_SIZE (8)
			0x91, 0x82,                    //   OUTPUT (Data,Var,Abs,Vol)
			0xc0,                           // END_COLLECTION
			// Additional Multimedia keys
			0x05, 0x0c,                    // USAGE_PAGE (Consumer Devices)
			0x09, 0x01,                    // USAGE (Consumer Control)
			0xa1, 0x01,                    // COLLECTION (Application)
			0x85, 0x09,                    //   REPORT_ID (9)
			0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
			0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
			0x75, 0x01,                    //   REPORT_SIZE (1)
//			0x95, 0x08,                    //   REPORT_COUNT (8)
			0x95, 0x10, // REPORT_COUNT (16)
			0x09, 0xb5,					   //   USAGE (Next Track)
			0x09, 0xb6,					   //   USAGE (Previous Track)
			0x09, 0xb7,					   //   USAGE (Stop)
			0x09, 0xb8,					   //   USAGE (Eject)
			0x09, 0xcd,                    //   USAGE (Play/Pause)
			0x09, 0xe2,                    //   USAGE (Mute)
			0x09, 0xea,                    //   USAGE (Volume Down)
			0x09, 0xe9,                    //   USAGE (Volume Up)
//			0x81, 0x02,                    //   INPUT (Data,Var,Abs)
			0x0a, 0x92, 0x01,              //   USAGE (AL Calculator)
			0x0a, 0x94, 0x01,              //   USAGE (AL Local Machine Browser)
			0x0a, 0x9F, 0x01,              //   USAGE (AL Control Panel)
			0x0a, 0xA1, 0x01,              //   USAGE (AL Task Manager)
			0x0a, 0xC7, 0x01,              //   USAGE (AL Audio Player)
			0x0a, 0x23, 0x02,              //   USAGE (AC Home)
			0x0a, 0x24, 0x02,              //   USAGE (AC Back)
			0x0a, 0x25, 0x02,              //   USAGE (AC Forward)
//			0x81, 0x02,                    //   INPUT (Data,Var,Abs)
//			0x81, 0x62, // INPUT (Data,Var,Abs,NPrf,Null)
			0x81, 0x06, // INPUT (Data,Var,Rel)
			0xc0,                          //     END_COLLECTION

};

/**
  * @}
  */ 

/** @defgroup USBD_HID_Private_Functions
  * @{
  */ 

/**
  * @brief  USBD_HID_Init
  *         Initialize the HID interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
uint8_t  USBD_HID_Init (USBD_HandleTypeDef *pdev,
                               uint8_t cfgidx)
{
  uint8_t ret = 0;

  /* Open EP IN */
  USBD_LL_OpenEP(pdev,
  		  	     USBD_HID_IN_EP,
                   USBD_EP_TYPE_INTR,
                   HID_EPIN_SIZE);
  USBD_LL_OpenEP(pdev,
  		  	     USBD_HID_OUT_EP,
                   USBD_EP_TYPE_INTR,
                   HID_EPIN_SIZE);
  
  if (pdev->pClassData == NULL) {
	  pdev->pClassData = USBD_malloc(sizeof (USBD_CDC_HID_HandleTypeDef));
  }
  
  if(pdev->pClassData == NULL)
  {
    ret = 1; 
  }
  else
  {
    ((USBD_CDC_HID_HandleTypeDef *)pdev->pClassData)->hid.state = 0;
  }
  return ret;
}

/**
  * @brief  USBD_HID_Init
  *         DeInitialize the HID layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
uint8_t  USBD_HID_DeInit (USBD_HandleTypeDef *pdev,
                                 uint8_t cfgidx)
{
  /* Close HID EPs */
  USBD_LL_CloseEP(pdev,
		  	  	  USBD_HID_IN_EP);
  
  /* FRee allocated memory */
  if(pdev->pClassData != NULL)
  {
    USBD_free(pdev->pClassData);
    pdev->pClassData = NULL;
  }

  return USBD_OK;
}

/**
  * @brief  USBD_HID_Setup
  *         Handle the HID specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
uint8_t  USBD_HID_Setup (USBD_HandleTypeDef *pdev,
                                USBD_SetupReqTypedef *req)
{
  uint16_t len = 0;
  uint8_t  *pbuf = NULL;
  USBD_HID_HandleTypeDef     *hhid = &( ( (USBD_CDC_HID_HandleTypeDef*) pdev->pClassData)->hid);
  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
  case USB_REQ_TYPE_CLASS :  
    switch (req->bRequest)
    {
      
      
    case HID_REQ_SET_PROTOCOL:
      hhid->Protocol = (uint8_t)(req->wValue);
      break;
      
    case HID_REQ_GET_PROTOCOL:
      USBD_CtlSendData (pdev, 
                        (uint8_t *)&hhid->Protocol,
                        1);    
      break;
      
    case HID_REQ_SET_IDLE:
      hhid->IdleState = (uint8_t)(req->wValue >> 8);
      break;
      
    case HID_REQ_GET_IDLE:
      USBD_CtlSendData (pdev, 
                        (uint8_t *)&hhid->IdleState,
                        1);        
      break;      

    default:
      USBD_CtlError (pdev, req);
      return USBD_FAIL; 
    }
    break;
    
  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    case USB_REQ_GET_DESCRIPTOR: 
      if( (req->wValue >> 8) == HID_REPORT_DESC)
      {
        len = MIN(USBD_CUSTOM_HID_REPORT_DESC_SIZE , req->wLength);
        pbuf = CUSTOM_HID_ReportDesc_FS;
      }
      else if( (req->wValue >> 8) == HID_DESCRIPTOR_TYPE)
      {
        pbuf = USBD_HID_Desc;
        len = MIN(USB_HID_DESC_SIZ , req->wLength);
      }
      
      USBD_CtlSendData (pdev, 
                        pbuf,
                        len);
      
      break;
      
    case USB_REQ_GET_INTERFACE :
      USBD_CtlSendData (pdev,
                        (uint8_t *)&hhid->AltSetting,
                        1);
      break;
      
    case USB_REQ_SET_INTERFACE :
      hhid->AltSetting = (uint8_t)(req->wValue);
      break;
    }
  }
  return USBD_OK;
}

/**
  * @brief  USBD_HID_SendReport
  *         Send HID Report
  * @param  pdev: device instance
  * @param  buff: pointer to report
  * @retval status
  */
uint8_t USBD_HID_SendReport     (USBD_HandleTypeDef  *pdev,
                                 uint8_t *report,
                                 uint16_t len)
{
  USBD_HID_HandleTypeDef     *hhid = &( ( (USBD_CDC_HID_HandleTypeDef*)pdev->pClassData)->hid);
  
  if (pdev->dev_state == USBD_STATE_CONFIGURED )
  {
    if(hhid->state == 0)
    {
      hhid->state = 1;
      USBD_LL_Transmit (pdev, 
    		  	  	  	USBD_HID_IN_EP,
                        report,
                        len);
    }
  }
  return USBD_OK;
}

/**
  * @brief  USBD_HID_GetPollingInterval
  *         return polling interval from endpoint descriptor
  * @param  pdev: device instance
  * @retval polling interval
  */
uint32_t USBD_HID_GetPollingInterval (USBD_HandleTypeDef *pdev)
{
  uint32_t polling_interval = 0;

  /* HIGH-speed endpoints */
  if(pdev->dev_speed == USBD_SPEED_HIGH)
  {
   /* Sets the data transfer polling interval for high speed transfers.
    Values between 1..16 are allowed. Values correspond to interval
    of 2 ^ (bInterval-1). This option (8 ms, corresponds to HID_HS_BINTERVAL */
    polling_interval = (((1 <<(HID_HS_BINTERVAL - 1)))/8);
  }
  else   /* LOW and FULL-speed endpoints */
  {
    /* Sets the data transfer polling interval for low and full
    speed transfers */
    polling_interval =  HID_FS_BINTERVAL;
  }

  return ((uint32_t)(polling_interval));
}


/**
  * @brief  USBD_HID_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
uint8_t  USBD_HID_DataIn (USBD_HandleTypeDef *pdev,
                              uint8_t epnum)
{
  
  /* Ensure that the FIFO is empty before a new transfer, this condition could 
  be caused by  a new transfer before the end of the previous transfer */
  ((USBD_CDC_HID_HandleTypeDef *)pdev->pClassData)->hid.state = HID_IDLE;
  return USBD_OK;
}


/**
* @brief  DeviceQualifierDescriptor 
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t  *USBD_HID_GetDeviceQualifierDesc (uint16_t *length)
{
  *length = sizeof (USBD_HID_DeviceQualifierDesc);
  return USBD_HID_DeviceQualifierDesc;
}

/**
  * @brief  USBD_CUSTOM_HID_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
uint8_t  USBD_HID_DataOut (USBD_HandleTypeDef *pdev,
                              uint8_t epnum)
{

	USBD_HID_HandleTypeDef     *hhid = &(((USBD_CDC_HID_HandleTypeDef*)pdev->pClassData)->hid);

//  ((USBD_CUSTOM_HID_ItfTypeDef *)pdev->pUserData)->OutEvent(hhid->Report_buf[0],
//                                                            hhid->Report_buf[1]);
  static uint8_t reportBuf[64];
  USBD_LL_PrepareReceive(pdev, USBD_HID_OUT_EP , reportBuf, 0x40);

  return USBD_OK;
}

/**
  * @}
  */ 


/**
  * @}
  */ 


/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
