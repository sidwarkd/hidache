/********************************************************************
 FileName:		hidache.c
 Dependencies:	See INCLUDES section
 Processor:		PIC18 or PIC24 USB Microcontrollers
 Hardware:		The code is natively intended to be used on the 18F2455.
				The firmware may be
 				modified for use on other USB platforms by editing the
 				HardwareProfile.h file.
 Complier:  	Microchip C18 (for PIC18) or C30 (for PIC24)
 Company:		Microchip Technology, Inc.

 Original Microchip Software License Agreement:

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

#ifndef HIDACHE_C
#define HIDACHE_C

/** INCLUDES *******************************************************/
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "usb_config.h"
#include "usb_device.h"
#include "usb.h"
#include "HardwareProfile.h"

/** C O N F I G U R A T I O N ************************************************/
// Note: For a complete list of the available config pragmas and their values, 
// see the compiler documentation, and/or click "Help --> Topics..." and then 
// select "PIC18 Config Settings" in the Language Tools section.

        #pragma config PLLDIV   = 2         // (8 MHz crystal)
        #pragma config CPUDIV   = OSC2_PLL3	
        #pragma config USBDIV   = 2         // Clock source from 96MHz PLL/2
        #pragma config FOSC     = HSPLL_HS
        #pragma config FCMEN    = OFF
        #pragma config IESO     = OFF
        #pragma config PWRT     = OFF
        #pragma config BOR      = OFF
        #pragma config BORV     = 3
        #pragma config VREGEN   = ON		//USB Voltage Regulator
        #pragma config WDT      = OFF
        #pragma config WDTPS    = 32768		//Watchdog Postscalar
        #pragma config MCLRE    = OFF
        #pragma config LPT1OSC  = OFF
        #pragma config PBADEN   = OFF
        #pragma config STVREN   = ON
        #pragma config LVP      = OFF		// Low Voltage Programming
        #pragma config XINST    = OFF       // Extended Instruction Set
        #pragma config CP0      = OFF
        #pragma config CP1      = OFF
        #pragma config CP2      = OFF
        #pragma config CPB      = OFF
        #pragma config CPD      = OFF
        #pragma config WRT0     = OFF
        #pragma config WRT1     = OFF
        #pragma config WRT2     = OFF
        #pragma config WRTB     = OFF       // Boot Block Write Protection
        #pragma config WRTC     = OFF
        #pragma config WRTD     = OFF
        #pragma config EBTR0    = OFF
        #pragma config EBTR1    = OFF
        #pragma config EBTR2    = OFF
        #pragma config EBTRB    = OFF


/** VARIABLES ******************************************************/
// Set up the default EEPROM settings for the device including the device code
// This is used during programming to set up a default configuration during programming
#pragma romdata DEV_DATA
	rom char eedata_vals[21] = {0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 7, 'H', 'I', 'D', 'A', 'c', 'h', 'e'};
#pragma romdata

#define MAX_MOUSE_STEPS		240

#pragma udata
BYTE old_sw;
BOOL emulate_mode;
BYTE movement_length;
char buffer[9];
USB_HANDLE lastTransmission;
enum SEND_TYPE sendType;

// Set up an array to store the characters in
#pragma udata OBJ_DATA
char phraseArray[240];

#pragma idata
BYTE vector = 0; // used to rotate through direction arrays
short keysTyped = 0; 
short mouseSteps = 0;
short startupDelays = 0;
short seconds = 0; 
short minutes = 0;
BOOL IsMouseFinished = FALSE;
BOOL IsKeyboardFinished = FALSE;
BOOL continuousMode = FALSE;
BOOL startupDelayDone = FALSE;
char lastKey = 0; // stores last key typed in keyboard mode

//HIDACHE Variables
enum DEVICE_MODE deviceMode = MODE_MOUSE;
enum MOUSE_MODE mouseMode = MM_CIRCLE;
BYTE prankInterval = 0x00;
short phraseLength = 0;

//The following tables define the mouse movement for different settings
ROM signed char circle_table[]={-10,-10,-10, 0, 10, 10, 10, 0};
ROM signed char side_table[] = {20,-20};

/** PRIVATE PROTOTYPES *********************************************/
void WriteEEPROMData(BYTE address, char *data, short length);
void ReadEEPROMData(BYTE address, char *data, short length);
void PerformEmulation(void);
static void InitializeSystem(void);
void ProcessIO(void);
void UserInit(void);
void YourHighPriorityISRCode();
void YourLowPriorityISRCode();
void SimpleSleep(int ms);
void ResetTimer(void);
void FillMouseBuffer(void);
void FillKeyboardBuffer(void);
void TransmitBuffer(void);

/** VECTOR REMAPPING ***********************************************/
// In MPLAB, __18CXX is defined automatically for you so you will not
// find it in this code.  The following code sets up the correct
// vector remapping.  I use the PROGRAMMABLE_WITH_USB_HID_BOOTLOADER
// define but left the rest of the code in so you can use it on the
// MCHPUSB bootloader or program it without a bootloader.

#if defined(__18CXX)
	//On PIC18 devices, addresses 0x00, 0x08, and 0x18 are used for
	//the reset, high priority interrupt, and low priority interrupt
	//vectors.  However, the current Microchip USB bootloader 
	//examples are intended to occupy addresses 0x00-0x7FF or
	//0x00-0xFFF depending on which bootloader is used.  Therefore,
	//the bootloader code remaps these vectors to new locations
	//as indicated below.  This remapping is only necessary if you
	//wish to program the hex file generated from this project with
	//the USB bootloader.  If no bootloader is used, edit the
	//usb_config.h file and comment out the following defines:
	//#define PROGRAMMABLE_WITH_USB_HID_BOOTLOADER
	//#define PROGRAMMABLE_WITH_USB_LEGACY_CUSTOM_CLASS_BOOTLOADER
	
	#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)
		#define REMAPPED_RESET_VECTOR_ADDRESS			0x10C0
		#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x10C8
		#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x10D8
	#elif defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)	
		#define REMAPPED_RESET_VECTOR_ADDRESS			0x800
		#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x808
		#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x818
	#else	
		#define REMAPPED_RESET_VECTOR_ADDRESS			0x00
		#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x08
		#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x18
	#endif
	
	#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)
	extern void _startup (void);        // See c018i.c in your C18 compiler dir
	#pragma code REMAPPED_RESET_VECTOR = REMAPPED_RESET_VECTOR_ADDRESS
	void _reset (void)
	{
	    _asm goto _startup _endasm
	}
	#endif
	#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS
	void Remapped_High_ISR (void)
	{
	     _asm goto YourHighPriorityISRCode _endasm
	}
	#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS
	void Remapped_Low_ISR (void)
	{
	     _asm goto YourLowPriorityISRCode _endasm
	}
	
	#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)
	//Note: If this project is built while one of the bootloaders has
	//been defined, but then the output hex file is not programmed with
	//the bootloader, addresses 0x08 and 0x18 would end up programmed with 0xFFFF.
	//As a result, if an actual interrupt was enabled and occured, the PC would jump
	//to 0x08 (or 0x18) and would begin executing "0xFFFF" (unprogrammed space).  This
	//executes as nop instructions, but the PC would eventually reach the REMAPPED_RESET_VECTOR_ADDRESS
	//(0x1000 or 0x800, depending upon bootloader), and would execute the "goto _startup".  This
	//would effective reset the application.
	
	//To fix this situation, we should always deliberately place a 
	//"goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS" at address 0x08, and a
	//"goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS" at address 0x18.  When the output
	//hex file of this project is programmed with the bootloader, these sections do not
	//get bootloaded (as they overlap the bootloader space).  If the output hex file is not
	//programmed using the bootloader, then the below goto instructions do get programmed,
	//and the hex file still works like normal.  The below section is only required to fix this
	//scenario.
	#pragma code HIGH_INTERRUPT_VECTOR = 0x08
	void High_ISR (void)
	{
	     _asm goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS _endasm
	}
	#pragma code LOW_INTERRUPT_VECTOR = 0x18
	void Low_ISR (void)
	{
	     _asm goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS _endasm
	}
	#endif	//end of "#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_LEGACY_CUSTOM_CLASS_BOOTLOADER)"

	#pragma code
	
	
	//These are your actual interrupt handling routines.
	#pragma interrupt YourHighPriorityISRCode
	void YourHighPriorityISRCode()
	{
		//Check which interrupt flag caused the interrupt.
		//Service the interrupt
		//Clear the interrupt flag
		//Etc.


	
	}	//This return will be a "retfie fast", since this is in a #pragma interrupt section 
	#pragma interruptlow YourHighPriorityISRCode
	void YourLowPriorityISRCode()
	{
		//Check which interrupt flag caused the interrupt.
		//Service the interrupt
		//Clear the interrupt flag
		//Etc.
	
	}	//This return will be a "retfie", since this is in a #pragma interruptlow section 

#endif //of "#if defined(__18CXX)"




/** DECLARATIONS ***************************************************/
#pragma code

/********************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Main program entry point.
 *
 * Note:            None
 *******************************************************************/
#if defined(__18CXX)
void main(void)
#else
int main(void)
#endif
{   
	// Set up everything first
    InitializeSystem();

	// Infinite loop to service USB and do prank tasks
    while(1)
    {
		// Check bus status and service USB interrupts.
        USBDeviceTasks(); // Interrupt or polling method.  If using polling, must call
        				  // this function periodically.  This function will take care
        				  // of processing and responding to SETUP transactions 
        				  // (such as during the enumeration process when you first
        				  // plug in).  USB hosts require that USB devices should accept
        				  // and process SETUP packets in a timely fashion.  Therefore,
        				  // when using polling, this function should be called 
        				  // frequently (such as once about every 100 microseconds) at any
        				  // time that a SETUP packet might reasonably be expected to
        				  // be sent by the host to your device.  In most cases, the
        				  // USBDeviceTasks() function does not take very long to
        				  // execute (~50 instruction cycles) before it returns.
    				  

		// Application-specific tasks.
		// Application related code may be added here, or in the ProcessIO() function.
		// We need to have an intial delay after enumeration.  In testing I found that
		// initial enumeration and driver setup can take several seconds so in order to
		// handle that initial connection I've added a 20 second initial delay after
		// which the normal prank interval will apply.  The reason it is in an 'if'
		// statement instead of a 'while' is because we need to continuously service
		// the bus through USBDeviceTasks or our device will be disconnected

		if((INTCONbits.TMR0IF == 1) && (startupDelays < 20) && !startupDelayDone)
		{
			ResetTimer();
			startupDelays++;
			if(startupDelays >= 20)
			{
				  startupDelayDone = TRUE;  
			}
		}
		
		if(startupDelayDone)
			ProcessIO(); 
    }//end while
}//end main

void MySleep(int ms)
{
	// This is fine for our PIC runing on 32 Mhz
	// It is a very simple sleep and only approximates a millisecond delay
	// You can fine tune it in MPLAB to acheive a more accurate timing but
	// for our purposes this will do.
	int x,z;
	for(z=0;z<ms;z++)
		for(x=0;x<500;x++);
}

/********************************************************************
 * Function:        static void InitializeSystem(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        InitializeSystem is a centralize initialization
 *                  routine. All required USB initialization routines
 *                  are called from here.
 *
 *                  User application initialization routine should
 *                  also be called from here.                  
 *
 * Note:            None
 *******************************************************************/
static void InitializeSystem(void)
{
	// This sets up I/O
    ADCON1 |= 0x0F;     // Default all pins to digital

    USBDeviceInit();	//usb_device.c.  Initializes USB module SFRs and firmware
    					//variables to known states.
    
	UserInit();			// This is where user initialization occurs

}//end InitializeSystem



/******************************************************************************
 * Function:        void UserInit(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine should take care of all of the user code
 *                  initialization that is required.
 *
 * Note:            
 *
 *****************************************************************************/
void UserInit(void)
{
	// To keep power consumption down and to prevent floating
	// pins we want to set all unused pins to digital output
	// and pull them low.
	InitializePins();

    // Initialize all of the data to 0 (no movement and no keystrokes)
	// For some reason I don't see the correct behavior with memset
    buffer[0] = buffer[1] = buffer[2] = buffer[3] = buffer[4] = buffer[5] = buffer[6] = buffer[7] = buffer[8] = 0;

	// Read in the hidache device info
	ReadEEPROMData(INTERVAL_LOC, (char*)&prankInterval, 1);		// Read in the prank interval
	ReadEEPROMData(DEV_MODE_LOC, (char*)&deviceMode, 1);		// Read in the device mode
	ReadEEPROMData(MOUSE_MODE_LOC, (char*)&mouseMode, 1);		// Read in the mouse mode
	ReadEEPROMData(KEYS_LENGTH_LOC, (char*)&phraseLength, 1);	// Read in the number of keystrokes for keyboard mode

	// Now read in the phrase just once so we don't have to read it every time the prank interval hits
	// This will allow us to extend the life of the EEPROM by reading it as infrequently as possible
	ReadEEPROMData(PHRASE_START_LOC, phraseArray, phraseLength);

    // Enable emulation mode.  This means that the mouse data
    // and or keyboard data will be sent to the PC.  Otherwise
	// no movement or keys will be sent.  We'll default it to 
	// true but then only set it when the prank interval has
	// expired
    emulate_mode = TRUE;

	// Set up continuous mode if the prankInterval is zero
	if(prankInterval == 0)
	{
		continuousMode = TRUE;
	}

	// We initialize sendType here just in case the mode is BOTH.
	// Otherwise it will be set appropriately in the FillBuffer 
	// function
	sendType = MOUSE;

	// Set up the timer for one second intervals regardless of the prank interval so 
	// that we can implement the initial startup delay.
	T0CONbits.T0CS = 0;
	T0CON &= 0x06; // prescalar to 128
	T0CONbits.PSA = 0; // assign prescalar
	T0CONbits.T08BIT = 0;
	TMR0H = 0x0B; // The values for TMR0H and TMR0L are set this way to acheive almost exactly
	TMR0L = 0xD0; // a one second interval between each interrupt.  Tuned in MPLAB.
	T0CONbits.TMR0ON = 1; // Turn the timer on

	// The following two lines are for testing timer times in MPLAB
	// Only uncomment this code if you are tweaking the timer values
	// You can set a breakpoint before this block and then after and
	// see what the actual delay is in MPLAB.  I've pre-tuned the values
	// for a 32MHZ clock cycle to be very close to 1 second.
	//while(INTCONbits.TMR0IF == 0);
	//T0CONbits.TMR0ON = 0;

    
    // Initialize the variable holding the handle for the last
    // transmission
    lastTransmission = 0;

}//end UserInit

/********************************************************************
 * Function:        void ProcessIO(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is a place holder for other user
 *                  routines. It is a mixture of both USB and
 *                  non-USB tasks.
 *
 * Note:            None
 *******************************************************************/
void ProcessIO(void)
{   
    // User Application USB tasks
	// If we're not in a configured state yet then there is no reason to 
	// continue
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;

	// This 'if' block keeps track of how much time has passed since the last
	// delay.
	if(INTCONbits.TMR0IF == 1) // We've hit the prank interval
	{
		seconds++;
		if(seconds >= 60)
		{
			seconds = 0;
			minutes++;
			if(minutes == prankInterval)
			{
				// Reset everything and get ready to go again
				emulate_mode = TRUE;
				IsKeyboardFinished = FALSE;
				IsMouseFinished = FALSE;
				minutes = 0;
				seconds = 0;
			}
		}
		
		ResetTimer();
	}

	if(continuousMode)
	{
		T0CONbits.TMR0ON = 0;  // We don't need the timer anymore so shut it off.
		emulate_mode = TRUE;

		// Only set them both back to FALSE if they have both finished
		if(IsKeyboardFinished && IsMouseFinished)
		{
			IsKeyboardFinished = FALSE;
			IsMouseFinished = FALSE;
		}
	}
		
	// If we've hit the prank interval or are in continuous
	// mode then perform the mouse/keyboard emulations
	if(emulate_mode)
	{
		PerformEmulation();
	}
	//}
    
}//end ProcessIO


/******************************************************************************
 * Function:        void PerformEmulation(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    The ownership of the USB buffers will change according
 *                  to the required operation
 *
 * Overview:        This routine will emulate the function of the mouse/keyboard.  It
 *					does this by sending IN packets of data to the host.
 *					
 *					The generic function HIDTxPacket() is used to send HID
 *					IN packets over USB to the host.
 *
 * Note:            
 *
 *****************************************************************************/
void PerformEmulation()
{   
	switch(deviceMode)
	{
		case MODE_MOUSE:
			// For mouse only mode we can go ahead and set the keyboard finished variable
			// up front.
			IsKeyboardFinished = TRUE;

			// Load the next value in the buffer
			FillMouseBuffer();
			break;
		case MODE_KEYBOARD:
			// For keyboard only mode we can go ahead and set the mouse finished variable
			// up front;
			IsMouseFinished = TRUE;
			
			// Load the next value in the buffer
			FillKeyboardBuffer();
			break;
		case MODE_BOTH:
			// Switch back and forth between the two modes until they both complete
			if(sendType == KEYSTROKE)
				FillKeyboardBuffer();
			else
				FillMouseBuffer();
			break;
	}
    
	// Send the buffer to the PC
	TransmitBuffer();

	if(deviceMode == MODE_BOTH)
	{
		// Switch back and forth between the two modes
		if(sendType == KEYSTROKE)
			sendType = MOUSE;
		else
			sendType = KEYSTROKE;
	}

	if(IsKeyboardFinished && IsMouseFinished)
	{
		//Send a clearing packet for both mouse and keyboard so that nothing is stuck on the OS side
		buffer[1] = buffer[2] = buffer[3] = 0;

		// Send the mouse clearing packet
		sendType = MOUSE;
		buffer[0] = 0x4D;  // Report ID for mouse
		while(HIDTxHandleBusy(lastTransmission)); // Wait for the last transmission to complete
		TransmitBuffer();

		// Send the keyboard clearing packet
		sendType = KEYSTROKE;
		buffer[0] = 0x4B;
		while(HIDTxHandleBusy(lastTransmission)); // Wait for the last transmission to complete
		TransmitBuffer();

		mouseSteps = 0;
		keysTyped = 0;
		emulate_mode = FALSE;
		buffer[3] = 0;
	}

}//end PerformEmulation

/********************************************************************
 * Function:        void FillKeyboardBuffer(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function prepares the transmission buffer
 *					for sending the next phrase character
 *
 * Note:            None
 *******************************************************************/
void FillKeyboardBuffer(void)
{
	char key;
	char hidKey;
	BOOL useShift = FALSE;

	// Load the report ID for keyboard transmission
	// This ID is semi-arbitrary and is set in the descriptor.
	// See usb_descriptors.c.
	buffer[0] = 0x4B;

	// Control bytes set to 0
	buffer[1] = 0;
	buffer[2] = 0;

	if(keysTyped < phraseLength) // There is still more to type
	{
		// The following code translates the ASCII keys stored in EEPROM
		// to HID keycodes.  I found it was easiest to store the text in
		// EEPROM as ASCII so the software could simply write it and read
		// it for display.  The translation is simple enough that I 
		// decided to handle it in firmware.
		
		if(!HIDTxHandleBusy(lastTransmission)) // If we're ready to send another character
		{
			key = phraseArray[keysTyped];
			if(TranslateAsciiToHID(key, &hidKey, &useShift))
			{
				if(useShift == TRUE)
					buffer[1] = 2;
				else
					buffer[1] = 0;
	
				buffer[3] = hidKey;
			}
			else //Failure in translate
			{
				// In the event of a failure in TranslateAsciiToHID I've
				// chosen to send a 'z' since the character shouldn't 
				// occur much.  If you see random z's being typed then it
				// indicates that something has gone wrong in the translate
				buffer[3] = 29;
			}
		}
	}
	else  // We've finished typing the phrase
	{
		buffer[1] = buffer[2] = buffer[3] = buffer[4] = buffer[5] = buffer[6] = buffer[7] = buffer[8] = 0;
		IsKeyboardFinished = TRUE;
	}
	sendType = KEYSTROKE;
}

/********************************************************************
 * Function:        void FillMouseBuffer(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function prepares the transmission buffer
 *					for mouse movement based on the mouse mode
 *
 * Note:            None
 *******************************************************************/
void FillMouseBuffer(void)
{
	// Load the report ID for mouse transmissions
	// This ID is semi-arbitrary and is set in the descriptor.
	// See usb_descriptors.c.
	buffer[0] = 0x4D;

	// No buttons will be pressed
	buffer[1] = 0;

	if(!IsMouseFinished)
	{
		switch(mouseMode)
		{
			case MM_CIRCLE:
				// Change direction every 20 steps or load the starting direction
				// if we haven't taken any steps yet.
				if(movement_length == 20)
				{
					//go to the next direction in the table
					vector++;

					buffer[2] = circle_table[vector & 0x07];           // X-Vector
					buffer[3] = circle_table[(vector+2) & 0x07];       // Y-Vector
										
					movement_length = 0;
				}
				else // This is mainly for BOTH mode where buffer[2] and [3] get destroyed in between mouse sends
				{
					buffer[2] = circle_table[vector & 0x07];           // X-Vector
					buffer[3] = circle_table[(vector+2) & 0x07];       // Y-Vector
				}

				break;
			case MM_BOTTOM_LEFT:
				buffer[2] = -5;	// X-Vector
				buffer[3] = 5;	// Y-Vector
				break;
			case MM_TOP_RIGHT:
				buffer[2] = 5;	// X-Vector
				buffer[3] = -5;	// Y-Vector
				break;
			case MM_LEFT:
				buffer[2] = -5;	// X-Vector
				buffer[3] = 0;	// Y-Vector
				break;
			case MM_UP:
				buffer[2] = 0;	// X-Vector
				buffer[3] = -5;	// Y-Vector
				break;
			case MM_RIGHT:
				buffer[2] = 5;	// X-Vector
				buffer[3] = 0;	// Y-Vector
				break;
			case MM_DOWN:
				buffer[2] = 0;	// X-Vector
				buffer[3] = 5;	// Y-Vector
				break;
			case MM_RANDOM:
				// I found that % did not work as I expected in firmware but it
				// turns out it creates a pretty random looking motion to use
				// it in the condition statement and then use the same values
				// as the circle table
				if(mouseSteps % 10 == 0)
				{
					buffer[2] = circle_table[vector & 0x07];           // X-Vector
					buffer[3] = circle_table[(vector+2) & 0x07];       // Y-Vector

					//go to the next direction in the table
					vector++;
				}				
				break;
			case MM_SIDE_TO_SIDE:
				// Change direction after 10 steps or load the initial direction
				if((movement_length > 10) || (mouseSteps == 0))
				{
					buffer[2] = side_table[vector & 0x01];  // X-Vector
					buffer[3] = 0;							// Y-Vector
					
					//go to the next direction in the table
					vector++;
					movement_length = 0;
				}
				break;
		}
	}
	else // Mouse is finished so set the buffer to 0.
	{
		buffer[2] = 0;
		buffer[3] = 0;
	}

	// Once we've moved the mouse 240 times we'll be done until
	// the next prank interval is hit
	if(mouseSteps > MAX_MOUSE_STEPS)
		IsMouseFinished = TRUE;

	// Set the sendType so we send the correct number of bits in the transmission
	sendType = MOUSE;
}

/******************************************************************************
 * Function:        void TransmitBuffer(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function sends a packet to the PC for mouse movement
 *					or a keyboard character based on the sendType.
 *
 * Note:            
 *
 *****************************************************************************/
void TransmitBuffer(void)
{
	if(!HIDTxHandleBusy(lastTransmission)) // Ready for another transmission
    {
		// Copy over the data to the HID buffer
		// We do this because we pass control of
		// hid_report_in over to the SIE which
		// allows us to continue manipulating buffer[]
		hid_report_in[0] = buffer[0];
		hid_report_in[1] = buffer[1];
		hid_report_in[2] = buffer[2];
		hid_report_in[3] = buffer[3];
		hid_report_in[4] = buffer[4];
		hid_report_in[5] = buffer[5];
		hid_report_in[6] = buffer[6];
		hid_report_in[7] = buffer[7];
		hid_report_in[8] = buffer[8];
     
        //Send the correct packet length to the PC depending on what type of data we are sending
		switch(sendType)
		{
			case MOUSE:
				mouseSteps++; // We increment this here instead of in the fill buffer routine because we know it is being sent here
				movement_length++;
				lastTransmission = HIDTxPacket(HID_EP, (BYTE*)hid_report_in, 0x04);		
				break;
			case KEYSTROKE:
				// I found that I had problems with keys being skipped if I tried to send them back to back in order.
				// It may be a speed issue but I found that it works nicely to send a blank packet in between chars.
				// This if-else handles that.
				if(hid_report_in[3] == lastKey)
				{
					hid_report_in[3] = 0;
					lastKey = 0;
				}
				else
				{
					lastKey = hid_report_in[3];
					keysTyped++;
				}
				
				lastTransmission = HIDTxPacket(HID_EP, (BYTE*)hid_report_in, 0x09);
				break;
		}
    }
	else
	{
		// This delay ensures that we don't send the keys too fast
		// for the computer
		MySleep(5);
	}
}

/******************************************************************************
 * Function:        void ResetTimer(void)
 *
 * PreCondition:    TIMER0 interrupt has fired
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine resets TIMER0.  It reloads the precalculated
 *					values to acheive 1 second delays and restarts it.
 *
 * Note:            
 *
 *****************************************************************************/
void ResetTimer(void)
{
	INTCONbits.TMR0IF = 0; // Clear the interrupt flag
	T0CONbits.TMR0ON = 0;  // Turn the timer off while we reset values
	TMR0H = 0x0B;		   // Reset the timer values to those previously calculated
	TMR0L = 0xD0;
	T0CONbits.TMR0ON = 1;  // Turn the timer back on
}


/******************************************************************************
 * Function:        void WriteEEPROMData(WORD address, char *data, short length)
 *
 * PreCondition:    None
 *
 * Input:           address - location in EEPROM to write to
 *
 * Input:			data - pointer to location where data is
 *
 * Input:			length - number of bytes to write
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Writes data to EEPROM starting at a specific address
 *
 * Note:            None
 *****************************************************************************/
void WriteEEPROMData(BYTE address, char *data, short length)
{
	short i = 0;
	EEADR = address;
	EECON1bits.EEPGD = 0;	// Point to Data Memory
	EECON1bits.CFGS = 0;	// Access EEPROM
	EECON1bits.WREN = 1;	// Enable writes

	for(i = 0; i < length; i++)
	{
		EEDATA = *data;
		EECON2 = 0x55;
		EECON2 = 0x0AA;
		EECON1bits.WR = 1;
		while(EECON1bits.WR == 1);	// Wait for write to finish
		data++;
		EEADR = EEADR + 0x01;	// Increment the write address
	}
	EECON1bits.WREN = 0;		// Disable EEPROM write capability
}

/******************************************************************************
 * Function:        char* ReadEEPROMData(WORD address, char *data, short length)
 *
 * PreCondition:    None
 *
 * Input:           address - location in EEPROM to read from
 *
 * Input:			data - pointer to location where data should be stored once read
 *
 * Input:			length - number of bytes to read into data
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Reads data from the EEPROM at a specific address
 *
 * Note:            None
 *****************************************************************************/
void ReadEEPROMData(BYTE address, char *data, short length)
{
	int i = 0;
	EEADR = address;
	EECON1bits.EEPGD = 0;
	EECON1bits.CFGS = 0;
	for(i = 0; i < length; i++)
	{
		EECON1bits.RD = 1;
		*data = EEDATA;
		data++;
		EEADR = EEADR + 0x01;
	}
}



// ******************************************************************************************************
// ************** USB Callback Functions ****************************************************************
// ******************************************************************************************************
// The USB firmware stack will call the callback functions USBCBxxx() in response to certain USB related
// events.  For example, if the host PC is powering down, it will stop sending out Start of Frame (SOF)
// packets to your device.  In response to this, all USB devices are supposed to decrease their power
// consumption from the USB Vbus to <2.5mA each.  The USB module detects this condition (which according
// to the USB specifications is 3+ms of no bus activity/SOF packets) and then calls the USBCBSuspend()
// function.  You should modify these callback functions to take appropriate actions for each of these
// conditions.  For example, in the USBCBSuspend(), you may wish to add code that will decrease power
// consumption from Vbus to <2.5mA (such as by clock switching, turning off LEDs, putting the
// microcontroller to sleep, etc.).  Then, in the USBCBWakeFromSuspend() function, you may then wish to
// add code that undoes the power saving things done in the USBCBSuspend() function.

// The USBCBSendResume() function is special, in that the USB stack will not automatically call this
// function.  This function is meant to be called from the application firmware instead.  See the
// additional comments near the function.

/******************************************************************************
 * Function:        void USBCBSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Call back that is invoked when a USB suspend is detected
 *
 * Note:            None
 *****************************************************************************/
void USBCBSuspend(void)
{
	//Example power saving code.  Insert appropriate code here for the desired
	//application behavior.  If the microcontroller will be put to sleep, a
	//process similar to that shown below may be used:
	
	//ConfigureIOPinsForLowPower();
	//SaveStateOfAllInterruptEnableBits();
	//DisableAllInterruptEnableBits();
	//EnableOnlyTheInterruptsWhichWillBeUsedToWakeTheMicro();	//should enable at least USBActivityIF as a wake source
	//Sleep();
	//RestoreStateOfAllPreviouslySavedInterruptEnableBits();	//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.
	//RestoreIOPinsToNormal();									//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.

	//IMPORTANT NOTE: Do not clear the USBActivityIF (ACTVIF) bit here.  This bit is 
	//cleared inside the usb_device.c file.  Clearing USBActivityIF here will cause 
	//things to not work as intended.	
	

    #if defined(__C30__)
    #if 0
        U1EIR = 0xFFFF;
        U1IR = 0xFFFF;
        U1OTGIR = 0xFFFF;
        IFS5bits.USB1IF = 0;
        IEC5bits.USB1IE = 1;
        U1OTGIEbits.ACTVIE = 1;
        U1OTGIRbits.ACTVIF = 1;
        TRISA &= 0xFF3F;
        LATAbits.LATA6 = 1;
        Sleep();
        LATAbits.LATA6 = 0;
    #endif
    #endif
}


/******************************************************************************
 * Function:        void _USB1Interrupt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the USB interrupt bit is set
 *					In this example the interrupt is only used when the device
 *					goes to sleep when it receives a USB suspend command
 *
 * Note:            None
 *****************************************************************************/
#if 0
void __attribute__ ((interrupt)) _USB1Interrupt(void)
{
    #if !defined(self_powered)
        if(U1OTGIRbits.ACTVIF)
        {
            LATAbits.LATA7 = 1;
        
            IEC5bits.USB1IE = 0;
            U1OTGIEbits.ACTVIE = 0;
            IFS5bits.USB1IF = 0;
        
            //USBClearInterruptFlag(USBActivityIFReg,USBActivityIFBitNum);
            USBClearInterruptFlag(USBIdleIFReg,USBIdleIFBitNum);
            //USBSuspendControl = 0;
            LATAbits.LATA7 = 0;
        }
    #endif
}
#endif

/******************************************************************************
 * Function:        void USBCBWakeFromSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The host may put USB peripheral devices in low power
 *					suspend mode (by "sending" 3+ms of idle).  Once in suspend
 *					mode, the host may wake the device back up by sending non-
 *					idle state signalling.
 *					
 *					This call back is invoked when a wakeup from USB suspend 
 *					is detected.
 *
 * Note:            None
 *****************************************************************************/
void USBCBWakeFromSuspend(void)
{
	// If clock switching or other power savings measures were taken when
	// executing the USBCBSuspend() function, now would be a good time to
	// switch back to normal full power run mode conditions.  The host allows
	// a few milliseconds of wakeup time, after which the device must be 
	// fully back to normal, and capable of receiving and processing USB
	// packets.  In order to do this, the USB module must receive proper
	// clocking (IE: 48MHz clock must be available to SIE for full speed USB
	// operation).
}

/********************************************************************
 * Function:        void USBCB_SOF_Handler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB host sends out a SOF packet to full-speed
 *                  devices every 1 ms. This interrupt may be useful
 *                  for isochronous pipes. End designers should
 *                  implement callback routine as necessary.
 *
 * Note:            None
 *******************************************************************/
void USBCB_SOF_Handler(void)
{
    // No need to clear UIRbits.SOFIF to 0 here.
    // Callback caller is already doing that.
}

/*******************************************************************
 * Function:        void USBCBErrorHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The purpose of this callback is mainly for
 *                  debugging during development. Check UEIR to see
 *                  which error causes the interrupt.
 *
 * Note:            None
 *******************************************************************/
void USBCBErrorHandler(void)
{
    // No need to clear UEIR to 0 here.
    // Callback caller is already doing that.

	// Typically, user firmware does not need to do anything special
	// if a USB error occurs.  For example, if the host sends an OUT
	// packet to your device, but the packet gets corrupted (ex:
	// because of a bad connection, or the user unplugs the
	// USB cable during the transmission) this will typically set
	// one or more USB error interrupt flags.  Nothing specific
	// needs to be done however, since the SIE will automatically
	// send a "NAK" packet to the host.  In response to this, the
	// host will normally retry to send the packet again, and no
	// data loss occurs.  The system will typically recover
	// automatically, without the need for application firmware
	// intervention.
	
	// Nevertheless, this callback function is provided, such as
	// for debugging purposes.
}


/*******************************************************************
 * Function:        void USBCBCheckOtherReq(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        When SETUP packets arrive from the host, some
 * 					firmware must process the request and respond
 *					appropriately to fulfill the request.  Some of
 *					the SETUP packets will be for standard
 *					USB "chapter 9" (as in, fulfilling chapter 9 of
 *					the official USB specifications) requests, while
 *					others may be specific to the USB device class
 *					that is being implemented.  For example, a HID
 *					class device needs to be able to respond to
 *					"GET REPORT" type of requests.  This
 *					is not a standard USB chapter 9 request, and 
 *					therefore not handled by usb_device.c.  Instead
 *					this request should be handled by class specific 
 *					firmware, such as that contained in usb_function_hid.c.
 *
 * Note:            None
 *******************************************************************/
void USBCBCheckOtherReq(void)
{
    USBCheckHIDRequest();
}//end


/*******************************************************************
 * Function:        void USBCBStdSetDscHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USBCBStdSetDscHandler() callback function is
 *					called when a SETUP, bRequest: SET_DESCRIPTOR request
 *					arrives.  Typically SET_DESCRIPTOR requests are
 *					not used in most applications, and it is
 *					optional to support this type of request.
 *
 * Note:            None
 *******************************************************************/
void USBCBStdSetDscHandler(void)
{
    // Must claim session ownership if supporting this request
}//end


/*******************************************************************
 * Function:        void USBCBInitEP(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the device becomes
 *                  initialized, which occurs after the host sends a
 * 					SET_CONFIGURATION (wValue not = 0) request.  This 
 *					callback function should initialize the endpoints 
 *					for the device's usage according to the current 
 *					configuration.
 *
 * Note:            None
 *******************************************************************/
void USBCBInitEP(void)
{
    //enable the HID endpoint
    USBEnableEndpoint(HID_EP,USB_IN_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);
}

/********************************************************************
 * Function:        void USBCBSendResume(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB specifications allow some types of USB
 * 					peripheral devices to wake up a host PC (such
 *					as if it is in a low power suspend to RAM state).
 *					This can be a very useful feature in some
 *					USB applications, such as an Infrared remote
 *					control	receiver.  If a user presses the "power"
 *					button on a remote control, it is nice that the
 *					IR receiver can detect this signalling, and then
 *					send a USB "command" to the PC to wake up.
 *					
 *					The USBCBSendResume() "callback" function is used
 *					to send this special USB signalling which wakes 
 *					up the PC.  This function may be called by
 *					application firmware to wake up the PC.  This
 *					function should only be called when:
 *					
 *					1.  The USB driver used on the host PC supports
 *						the remote wakeup capability.
 *					2.  The USB configuration descriptor indicates
 *						the device is remote wakeup capable in the
 *						bmAttributes field.
 *					3.  The USB host PC is currently sleeping,
 *						and has previously sent your device a SET 
 *						FEATURE setup packet which "armed" the
 *						remote wakeup capability.   
 *
 *					This callback should send a RESUME signal that
 *                  has the period of 1-15ms.
 *
 * Note:            Interrupt vs. Polling
 *                  -Primary clock
 *                  -Secondary clock ***** MAKE NOTES ABOUT THIS *******
 *                   > Can switch to primary first by calling USBCBWakeFromSuspend()
 
 *                  The modifiable section in this routine should be changed
 *                  to meet the application needs. Current implementation
 *                  temporary blocks other functions from executing for a
 *                  period of 1-13 ms depending on the core frequency.
 *
 *                  According to USB 2.0 specification section 7.1.7.7,
 *                  "The remote wakeup device must hold the resume signaling
 *                  for at lest 1 ms but for no more than 15 ms."
 *                  The idea here is to use a delay counter loop, using a
 *                  common value that would work over a wide range of core
 *                  frequencies.
 *                  That value selected is 1800. See table below:
 *                  ==========================================================
 *                  Core Freq(MHz)      MIP         RESUME Signal Period (ms)
 *                  ==========================================================
 *                      48              12          1.05
 *                       4              1           12.6
 *                  ==========================================================
 *                  * These timing could be incorrect when using code
 *                    optimization or extended instruction mode,
 *                    or when having other interrupts enabled.
 *                    Make sure to verify using the MPLAB SIM's Stopwatch
 *                    and verify the actual signal on an oscilloscope.
 *******************************************************************/
void USBCBSendResume(void)
{
    static WORD delay_count;
    
    USBResumeControl = 1;                // Start RESUME signaling
    
    delay_count = 1800U;                // Set RESUME line for 1-13 ms
    do
    {
        delay_count--;
    }while(delay_count);
    USBResumeControl = 0;
}


/** EOF hidache.c *************************************************/
#endif
