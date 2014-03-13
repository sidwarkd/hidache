/********************************************************************
 FileName:     	usb_function_hid.c
 Dependencies:	See INCLUDES section
 Processor:		PIC18 or PIC24 USB Microcontrollers
 Hardware:		The code is natively intended to be used on the following
 				hardware platforms: PICDEM™ FS USB Demo Board, 
 				PIC18F87J50 FS USB Plug-In Module, or
 				Explorer 16 + PIC24 USB PIM.  The firmware may be
 				modified for use on other USB platforms by editing the
 				HardwareProfile.h file.
 Complier:  	Microchip C18 (for PIC18) or C30 (for PIC24)
 Company:		Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the “Company”) for its PIC® Microcontroller is intended and
 supplied to you, the Company’s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************
 File Description:

 Change History:
  Rev   Date         Description
  1.0   11/19/2004   Initial release
  2.1   02/26/2007   Updated for simplicity and to use common
                     coding style
*******************************************************************/

/** INCLUDES *******************************************************/
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "usb_config.h"
#include "usb_device.h"
#include "usb_function_hid.h"

/** VARIABLES ******************************************************/
#pragma udata
BYTE idle_rate;
BYTE active_protocol;   // [0] Boot Protocol [1] Report Protocol
BYTE hid_rpt_rx_len;

/** PRIVATE PROTOTYPES *********************************************/
void HIDGetReportHandler(void);
void HIDSetReportHandler(void);

/** DECLARATIONS ***************************************************/
#pragma code

/** CLASS SPECIFIC REQUESTS ****************************************/
/********************************************************************
 * Function:        void USBCheckHIDRequest(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine checks the setup data packet to see
 *                  if it knows how to handle it
 *
 * Note:            None
 *******************************************************************/
void USBCheckHIDRequest(void)
{
    if(SetupPkt.Recipient != RCPT_INTF) return;
    if(SetupPkt.bIntfID != HID_INTF_ID) return;
    
    /*
     * There are two standard requests that hid.c may support.
     * 1. GET_DSC(DSC_HID,DSC_RPT,DSC_PHY);
     * 2. SET_DSC(DSC_HID,DSC_RPT,DSC_PHY);
     */
    if(SetupPkt.bRequest == GET_DSC)
    {
        switch(SetupPkt.bDescriptorType)
        {
            case DSC_HID:           
                if(USBActiveConfiguration == 1)
                {
                    USBEP0SendROMPtr(
                        (ROM BYTE*)&configDescriptor1 + 18,
                        sizeof(USB_HID_DSC)+3,     // RRoj hack
                        USB_EP0_INCLUDE_ZERO);
                }
                break;
            case DSC_RPT:             
                if(USBActiveConfiguration == 1)
                {
                    USBEP0SendROMPtr(
                        (ROM BYTE*)&hid_rpt01,
                        sizeof(hid_rpt01),     //See usbcfg.h
                        USB_EP0_INCLUDE_ZERO);
                }
                break;
            case DSC_PHY:
                USBEP0Transmit(USB_EP0_NO_DATA);
                break;
        }//end switch(SetupPkt.bDescriptorType)
    }//end if(SetupPkt.bRequest == GET_DSC)
    
    if(SetupPkt.RequestType != CLASS) return;
    switch(SetupPkt.bRequest)
    {
        case GET_REPORT:
            HIDGetReportHandler();
            break;
        case SET_REPORT:
            HIDSetReportHandler();            
            break;
        case GET_IDLE:
            USBEP0SendRAMPtr(
                (BYTE*)&idle_rate,
                1,
                USB_EP0_INCLUDE_ZERO);
            break;
        case SET_IDLE:
            USBEP0Transmit(USB_EP0_NO_DATA);
            idle_rate = SetupPkt.W_Value.byte.HB;
            break;
        case GET_PROTOCOL:
            USBEP0SendRAMPtr(
                (BYTE*)&active_protocol,
                1,
                USB_EP0_NO_OPTIONS);
            break;
        case SET_PROTOCOL:
            USBEP0Transmit(USB_EP0_NO_DATA);
            active_protocol = SetupPkt.W_Value.byte.LB;
            break;
    }//end switch(SetupPkt.bRequest)

}//end USBCheckHIDRequest

void HIDGetReportHandler(void)
{
}//end HIDGetReportHandler

void HIDSetReportHandler(void)
{
}//end HIDSetReportHandler

/** USER API *******************************************************/

/********************************************************************
 * Function:        void HIDInitEP(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        HIDInitEP initializes HID endpoints, buffer
 *                  descriptors, internal state-machine, and
 *                  variables. It should be called after the USB
 *                  host has sent out a SET_CONFIGURATION request.
 *                  See USBStdSetCfgHandler() in usbd.c for examples.
 *
 * Note:            None
 *******************************************************************/
#if !defined(USB_DYNAMIC_EP_CONFIG)
void HIDInitEP(void)
{   
}//end HIDInitEP
#endif

/** EOF hid.c ******************************************************/
