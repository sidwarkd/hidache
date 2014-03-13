/*******************************************************************************

    USB Header File

This file aggregates all necessary header files for the Microchip USB Host,
Device, and OTG libraries.  It provides a single-file can be included in
application code.  The USB libraries simplify the implementation of USB
applications by providing an abstraction of the USB module and its registers
and bits such that the source code for the can be the same across various
hardware platforms.

Note that this file does not include the header files for any client or
function drivers.
******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************
 FileName:     usb.h
 Dependencies: See INCLUDES section
 Processor:    PIC18, PIC24, & PIC32 USB Microcontrollers
 Hardware:
 Complier:     Microchip C18 (for PIC18), C30 (for PIC24), or C32 (for PIC32)
 Company:      Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the �Company�) for its PICmicro� Microcontroller is intended and
 supplied to you, the Company�s customer, for use solely and
 exclusively on Microchip PICmicro Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************

 Change History:
 Date         Description
 02/20/2008   Initial version
********************************************************************/
//DOM-IGNORE-END

//DOM-IGNORE-BEGIN
#ifndef _USB_H_
#define _USB_H_
//DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: All necessary USB Library headers
// *****************************************************************************
// *****************************************************************************

#include "usb_config.h"             // Must be defined by the application

#include "usb_common.h"         // Common USB library definitions
#include "usb_ch9.h"            // USB device framework definitions
#include "usb_hal.h"            // Hardware Abstraction Layer interface

#if defined( USB_SUPPORT_DEVICE )
    #include "usb_device.h"     // USB Device abstraction layer interface
#endif

#if defined( USB_SUPPORT_HOST )
    #include "usb_host.h"       // USB Host abstraction layer interface
#endif

#if defined ( USB_SUPORT_OTG )
    #error "USB OTG is not supported."
#endif


// *****************************************************************************
// *****************************************************************************
// Section: Host Firmware Version
// *****************************************************************************
// *****************************************************************************

#define USB_MAJOR_VER   1       // Firmware version, major release number.
#define USB_MINOR_VER   0       // Firmware version, minor release number.
#define USB_DOT_VER     0       // Firmware version, dot release number.


#endif // _USB_H_
/*************************************************************************
 * EOF
 */

