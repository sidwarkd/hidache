/********************************************************************
 FileName:     	HardwareProfile.h
 Dependencies:	See INCLUDES section
 Processor:		PIC18 or PIC24 USB Microcontrollers
 Hardware:		The code is natively intended to be used on the following
 				hardware platforms: PICDEM™ FS USB Demo Board, 
 				PIC18F87J50 FS USB Plug-In Module, or
 				Explorer 16 + PIC24 USB PIM.  The firmware may be
 				modified for use on other USB platforms by editing this
 				file (HardwareProfile.h).
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
  1.0	06/01/2008	 Modified to be HIDAche firmware - HardlySoftware
					 Revision changed to 1.0 because it will be the
					 first release of HIDAche
********************************************************************/

#ifndef HARDWARE_PROFILE_H
#define HARDWARE_PROFILE_H

#define DEMO_BOARD 
#define HIDACHE_BOARD

#if !defined(DEMO_BOARD)
    #if defined(__C30__)
        #if defined(__PIC24FJ256GB110__)
            #define DEMO_BOARD PIC24FJ256GB110_PIM
            #define EXPLORER_16
			#define PIC24FJ256GB110_PIM
            #define CLOCK_FREQ 32000000
        #endif
    #endif

    #if defined(__18CXX)
        #if defined(__18F4550)
            #define DEMO_BOARD PICDEM_FS_USB
            #define PICDEM_FS_USB
            #define CLOCK_FREQ 48000000
        #elif defined(__18F87J50) || defined(__18F85J50)
            #define DEMO_BOARD PIC18F87J50_PIM
            #define PIC18F87J50_PIM
            #define CLOCK_FREQ 48000000
        #endif
    #endif
#endif

#if !defined(DEMO_BOARD)
    #error "Demo board not defined.  Either define DEMO_BOARD for a custom board or select the correct processor for the demo board."
#endif

//#define ENABLE_CONSOLE

/** TRIS ***********************************************************/
#define INPUT_PIN           1
#define OUTPUT_PIN          0

/** USB ************************************************************/
#if defined(HIDACHE_BOARD)
	//#define USE_SELF_POWER_SENSE_IO	
	//#define USE_USB_BUS_SENSE_IO

	//Prototype for the translate function
	BOOL TranslateAsciiToHID(char asciiKey, char *hidKey, BOOL *needShiftKey);

	#define PROGRAMMABLE_WITH_USB_HID_BOOTLOADER

	#define U1ADDR UADDR
    #define U1IE UIE
    #define U1IR UIR
    #define U1EIR UEIR
    #define U1EIE UEIE
    #define U1CON UCON
    #define U1EP0 UEP0
    #define U1CONbits UCONbits
    #define U1EP1 UEP1
    #define U1CNFG1 UCFG
    #define U1STAT USTAT
    #define U1EP0bits UEP0bits

	#if defined(USE_USB_BUS_SENSE_IO)
    #define USB_BUS_SENSE       PORTAbits.RA1
    #else
    #define USB_BUS_SENSE       1
    #endif
    
    #define tris_self_power     TRISAbits.TRISA2    // Input
    
    #if defined(USE_SELF_POWER_SENSE_IO)
    #define self_power          PORTAbits.RA2
    #else
    #define self_power          1
    #endif

	// I leave this code here commented out in case I want to use a switch
	// or LED for debug purposes.
	/** LED ************************************************************/
    //#define mInitAllLEDs()      LATA &= 0x00; TRISA &= 0x00;
    
    //#define statusLED           LATAbits.LATA1   
    //#define statusLED_On()      mLED_1 = 1;    
    //#define statusLED_Off()     mLED_1 = 0; 
    //#define statusLED_Toggle()  mLED_1 = !mLED_1;
    
    /** SWITCH *********************************************************/
    //#define mInitAllSwitches()  TRISAbits.TRISA0=1;
    //#define mSwitch             PORTAbits.RA0

	/** Pin Initialization *********************************************/
	// Make all unused pins output and pull them low
	#define InitializePins()	TRISB = 0x00; TRISC &= 0x38; TRISA &= 0xC3; PORTA &= 0xC3; PORTB &= 0x00; PORTC &= 0x38;

	// EEPROM MEMORY LAYOUT ********************************************
	#define DEVICE_STRUCT_LOC	0x00
	#define INTERVAL_LOC		0x0A
	#define DEV_MODE_LOC		0x0B
	#define MOUSE_MODE_LOC		0x0C
	#define KEYS_LENGTH_LOC		0x0D
	#define PHRASE_START_LOC	0x0E

	// HIDACHE STRUCTURE DEFINITION ************************************
	// The EEPROM layout for hidache is as follows:
	// Interval - 0x0A
	// Device Mode - 0x0B
	// Mouse Mode - 0x0C
	// Phrase Length - 0x0D
	// Phrase - 0x0E - 0xFF
	enum DEVICE_MODE
	{
		MODE_MOUSE		= 0x00,
		MODE_KEYBOARD	= 0x01,
		MODE_BOTH		= 0x02
	};//DEVICE_MODE;
	enum MOUSE_MODE
	{
		MM_CIRCLE		= 0x00,
		MM_BOTTOM_LEFT	= 0x01,
		MM_TOP_RIGHT	= 0x02,
		MM_UP			= 0x03,
		MM_DOWN			= 0x04,
		MM_LEFT			= 0x05,
		MM_RIGHT		= 0x06,
		MM_RANDOM		= 0x07,
		MM_SIDE_TO_SIDE = 0x08
	};//MOUSE_MODE;

	enum SEND_TYPE
	{
		MOUSE = 0x00,
		KEYSTROKE = 0x01
	};

#endif

#if defined(PICDEM_FS_USB)
	//The PICDEM FS USB Demo Board platform supports the USE_SELF_POWER_SENSE_IO
	//and USE_USB_BUS_SENSE_IO features.  Uncomment the below line(s) if
	//it is desireable to use one or both of the features.
	//#define USE_SELF_POWER_SENSE_IO	
	//#define USE_USB_BUS_SENSE_IO

	#define PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER	//Uncomment this to make the output HEX of this project work with the MCHPUSB Bootloader

    #define U1ADDR UADDR
    #define U1IE UIE
    #define U1IR UIR
    #define U1EIR UEIR
    #define U1EIE UEIE
    #define U1CON UCON
    #define U1EP0 UEP0
    #define U1CONbits UCONbits
    #define U1EP1 UEP1
    #define U1CNFG1 UCFG
    #define U1STAT USTAT
    #define U1EP0bits UEP0bits
    
    #define tris_usb_bus_sense  TRISAbits.TRISA1    // Input
    
    #if defined(USE_USB_BUS_SENSE_IO)
    #define USB_BUS_SENSE       PORTAbits.RA1
    #else
    #define USB_BUS_SENSE       1
    #endif
    
    #define tris_self_power     TRISAbits.TRISA2    // Input
    
    #if defined(USE_SELF_POWER_SENSE_IO)
    #define self_power          PORTAbits.RA2
    #else
    #define self_power          1
    #endif
    
    // External Transceiver Interface
    #define tris_usb_vpo        TRISBbits.TRISB3    // Output
    #define tris_usb_vmo        TRISBbits.TRISB2    // Output
    #define tris_usb_rcv        TRISAbits.TRISA4    // Input
    #define tris_usb_vp         TRISCbits.TRISC5    // Input
    #define tris_usb_vm         TRISCbits.TRISC4    // Input
    #define tris_usb_oe         TRISCbits.TRISC1    // Output
    
    #define tris_usb_suspnd     TRISAbits.TRISA3    // Output
    
    /** LED ************************************************************/
    #define mInitAllLEDs()      LATD &= 0xF0; TRISD &= 0xF0;
    
    #define mLED_1              LATDbits.LATD0
    #define mLED_2              LATDbits.LATD1
    #define mLED_3              LATDbits.LATD2
    #define mLED_4              LATDbits.LATD3
    
    #define mLED_1_On()         mLED_1 = 1;
    #define mLED_2_On()         mLED_2 = 1;
    #define mLED_3_On()         mLED_3 = 1;
    #define mLED_4_On()         mLED_4 = 1;
    
    #define mLED_1_Off()        mLED_1 = 0;
    #define mLED_2_Off()        mLED_2 = 0;
    #define mLED_3_Off()        mLED_3 = 0;
    #define mLED_4_Off()        mLED_4 = 0;
    
    #define mLED_1_Toggle()     mLED_1 = !mLED_1;
    #define mLED_2_Toggle()     mLED_2 = !mLED_2;
    #define mLED_3_Toggle()     mLED_3 = !mLED_3;
    #define mLED_4_Toggle()     mLED_4 = !mLED_4;
    
    /** SWITCH *********************************************************/
    #define mInitAllSwitches()  TRISBbits.TRISB4=1;TRISBbits.TRISB5=1;
    #define mInitSwitch2()      TRISBbits.TRISB4=1;
    #define mInitSwitch3()      TRISBbits.TRISB5=1;
    #define sw2                 PORTBbits.RB4
    #define sw3                 PORTBbits.RB5
    
    /** POT ************************************************************/
    #define mInitPOT()          {TRISAbits.TRISA0=1;ADCON0=0x01;ADCON2=0x3C;ADCON2bits.ADFM = 1;}
    
    /** SPI : Chip Select Lines ****************************************/
    #define tris_cs_temp_sensor TRISBbits.TRISB2    // Output
    #define cs_temp_sensor      LATBbits.LATB2
    
    #define tris_cs_sdmmc       TRISBbits.TRISB3    // Output
    #define cs_sdmmc            LATBbits.LATB3
    
    /** SD/MMC *********************************************************/
    #define TRIS_CARD_DETECT    TRISBbits.TRISB4    // Input
    #define CARD_DETECT         PORTBbits.RB4
    
    #define TRIS_WRITE_DETECT   TRISAbits.TRISA4    // Input
    #define WRITE_DETECT        PORTAbits.RA4

#endif

#endif  //HARDWARE_PROFILE_H
