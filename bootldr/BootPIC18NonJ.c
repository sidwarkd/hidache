/*********************************************************************
 *
 *   Microchip USB HID Bootloader for PIC18F and PIC18LF versions of:
 *	 PIC18F4553/4458/2553/2458
 *	 PIC18F4550/4455/2550/2455
 *	 PIC18F4450/2450
 *
 *********************************************************************
 * FileName:        BootPIC18NonJ.c
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 3.20+
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the �Company�) for its PICmicro� Microcontroller is intended and
 * supplied to you, the Company�s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * File version         Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 1.0					06/19/08	Original Version.  Adapted from 
 *									MCHPFSUSB v2.1 HID Bootloader
 *									for PIC18F87J50 Family devices.
 * 2.0					07/14/08	Changed by HardlySoftware to account
 *									for size increase due to end of C18
 *									trial period.
 *********************************************************************/

/** C O N S T A N T S **********************************************************/

//Section defining the address range to erase for the erase device command, along with the valid programming range to be reported by the QUERY_DEVICE command.
#define ProgramMemStart					0x001000 //Beginning of application program memory (not occupied by bootloader).  **THIS VALUE MUST BE ALIGNED WITH 64 BYTE BLOCK BOUNDRY** Also, in order to work correctly, make sure the StartPageToErase is set to erase this section.

#if defined(__18F4550)||defined(__18F2550)
	#define StartPageToErase			67		 //The 4096 byte section from 0x000-0xFFF contains the bootloader and will not be erased
	#define MaxPageToErase				511		 //Last 64 byte page of flash on the PIC18F4550
	#define ProgramMemStop				0x008000 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If AddressToStopPopulating = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**	
	#define ConfigWordsStartAddress		0x300000 //0x300000 is CONFIG space for PIC18F4550/PIC18F4553/PIC18F4450 family devices
	#define ConfigWordsSectionLength	0x0C   	 //12 bytes worth of Configuration words on the PIC18F4550/PIC18F4553/PIC18F4450 family devices
	#define UserIDAddress				0x200000 //User ID is 8 bytes starting at 0x200000
	#define	UserIDSize					8
	#define	DEVICE_WITH_EEPROM
	#define	EEPROMSize					0x100	 //256 bytes of EEPROM on this device
	#define EEPROMEffectiveAddress		0xF00000
	#define	ProgramBlockSize			0x20	 //32 byte programming block size on the PIC18F4550/PIC18F4553 family devices
#elif defined(__18F4553)||defined(__18F2553)
	#define StartPageToErase			67		 //The 4096 byte section from 0x000-0xFFF contains the bootloader and will not be erased
	#define MaxPageToErase				511		 //Last 64 byte page of flash on the PIC18F4550
	#define ProgramMemStop				0x008000 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If AddressToStopPopulating = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**	
	#define ConfigWordsStartAddress		0x300000 //0x300000 is CONFIG space for PIC18F4550/PIC18F4553/PIC18F4450 family devices
	#define ConfigWordsSectionLength	0x0C   	 //12 bytes worth of Configuration words on the PIC18F4550/PIC18F4553/PIC18F4450 family devices
	#define UserIDAddress				0x200000 //User ID is 8 bytes starting at 0x200000
	#define	UserIDSize					8
	#define	DEVICE_WITH_EEPROM
	#define	EEPROMSize					0x100	 //256 bytes of EEPROM on this device
	#define EEPROMEffectiveAddress		0xF00000
	#define	ProgramBlockSize			0x20	 //32 byte programming block size on the PIC18F4550/PIC18F4553 family devices
#elif defined(__18F4455)||defined(__18F2455)
	#define StartPageToErase			67		 //The 4096 byte section from 0x000-0xFFF contains the bootloader and will not be erased
	#define MaxPageToErase				383		 //Last 64 byte page of flash on the PIC18F4455
	#define ProgramMemStop				0x006000 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If AddressToStopPopulating = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**	
	#define ConfigWordsStartAddress		0x300000 //0x300000 is CONFIG space for PIC18F4550/PIC18F4553/PIC18F4450 family devices
	#define ConfigWordsSectionLength	0x0C   	 //12 bytes worth of Configuration words on the PIC18F4550/PIC18F4553/PIC18F4450 family devices
	#define UserIDAddress				0x200000 //User ID is 8 bytes starting at 0x200000
	#define	UserIDSize					8
	#define	DEVICE_WITH_EEPROM
	#define	EEPROMSize					0x100	 //256 bytes of EEPROM on this device
	#define EEPROMEffectiveAddress		0xF00000
	#define	ProgramBlockSize			0x20	 //32 byte programming block size on the PIC18F4550/PIC18F4553 family devices
#elif defined(__18F4458)||defined(__18F2458)
	#define StartPageToErase			67		 //The 4096 byte section from 0x000-0xFFF contains the bootloader and will not be erased
	#define MaxPageToErase				383		 //Last 64 byte page of flash on the PIC18F4455
	#define ProgramMemStop				0x006000 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If AddressToStopPopulating = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**	
	#define ConfigWordsStartAddress		0x300000 //0x300000 is CONFIG space for PIC18F4550/PIC18F4553/PIC18F4450 family devices
	#define ConfigWordsSectionLength	0x0C   	 //12 bytes worth of Configuration words on the PIC18F4550/PIC18F4553/PIC18F4450 family devices
	#define UserIDAddress				0x200000 //User ID is 8 bytes starting at 0x200000
	#define	UserIDSize					8
	#define	DEVICE_WITH_EEPROM
	#define	EEPROMSize					0x100	 //256 bytes of EEPROM on this device
	#define EEPROMEffectiveAddress		0xF00000
	#define	ProgramBlockSize			0x20	 //32 byte programming block size on the PIC18F4550/PIC18F4553 family devices

#elif defined(__18F4450)||defined(__18F2450)
	#define StartPageToErase			67		 //The 4096 byte section from 0x000-0xFFF contains the bootloader and will not be erased
	#define MaxPageToErase				255		 //Last 64 byte page of flash on the PIC18F4450
	#define ProgramMemStop				0x004000 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If AddressToStopPopulating = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**	
	#define ConfigWordsStartAddress		0x300000 //0x300000 is CONFIG space for PIC18F4550/PIC18F4553/PIC18F4450 family devices
	#define ConfigWordsSectionLength	0x0C   	 //12 bytes worth of Configuration words on the PIC18F4550/PIC18F4553/PIC18F4450 family devices
	#define UserIDAddress				0x200000 //User ID is 8 bytes starting at 0x200000
	#define	UserIDSize					8
	#define	ProgramBlockSize			0x10	 //16 byte programming block size on the PIC18F4450/2450 family devices

#elif defined(__18F14K50)
	#define StartPageToErase			67		 //The 4096 byte section from 0x000-0xFFF contains the bootloader and will not be erased
	#define MaxPageToErase				255		 //Last 64 byte page of flash on the PIC18F4455
	#define ProgramMemStop				0x004000 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If AddressToStopPopulating = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**	
	#define ConfigWordsStartAddress		0x300000 //0x300000 is CONFIG space for PIC18F4550/PIC18F4553/PIC18F4450 family devices
	#define ConfigWordsSectionLength	0x0C   	 //12 bytes worth of Configuration words on the PIC18F4550/PIC18F4553/PIC18F4450 family devices
	#define UserIDAddress				0x200000 //User ID is 8 bytes starting at 0x200000
	#define	UserIDSize					8
	#define	DEVICE_WITH_EEPROM
	#define	EEPROMSize					0x100	 //256 bytes of EEPROM on this device
	#define EEPROMEffectiveAddress		0xF00000
	#define	ProgramBlockSize			0x20	 //32 byte programming block size on the PIC18F14K50 family devices
#elif defined(__18F13K50)
	#define StartPageToErase			67		 //The 4096 byte section from 0x000-0xFFF contains the bootloader and will not be erased
	#define MaxPageToErase				127		 //Last 64 byte page of flash on the PIC18F4455
	#define ProgramMemStop				0x002000 //**MUST BE WORD ALIGNED (EVEN) ADDRESS.  This address does not get updated, but the one just below it does: IE: If AddressToStopPopulating = 0x200, 0x1FF is the last programmed address (0x200 not programmed)**	
	#define ConfigWordsStartAddress		0x300000 //0x300000 is CONFIG space for PIC18F4550/PIC18F4553/PIC18F4450 family devices
	#define ConfigWordsSectionLength	0x0C   	 //12 bytes worth of Configuration words on the PIC18F4550/PIC18F4553/PIC18F4450 family devices
	#define UserIDAddress				0x200000 //User ID is 8 bytes starting at 0x200000
	#define	UserIDSize					8
	#define	DEVICE_WITH_EEPROM
	#define	EEPROMSize					0x100	 //256 bytes of EEPROM on this device
	#define EEPROMEffectiveAddress		0xF00000
	#define	ProgramBlockSize			0x20	 //32 byte programming block size on the PIC18F14K50 family devices

#endif

//Switch State Variable Choices
#define	QUERY_DEVICE				0x02	//Command that the host uses to learn about the device (what regions can be programmed, and what type of memory is the region)
#define	UNLOCK_CONFIG				0x03	//Note, this command is used for both locking and unlocking the config bits (see the "//Unlock Configs Command Definitions" below)
#define ERASE_DEVICE				0x04	//Host sends this command to start an erase operation.  Firmware controls which pages should be erased.
#define PROGRAM_DEVICE				0x05	//If host is going to send a full RequestDataBlockSize to be programmed, it uses this command.
#define	PROGRAM_COMPLETE			0x06	//If host send less than a RequestDataBlockSize to be programmed, or if it wished to program whatever was left in the buffer, it uses this command.
#define GET_DATA					0x07	//The host sends this command in order to read out memory from the device.  Used during verify (and read/export hex operations)
#define	RESET_DEVICE				0x08	//Resets the microcontroller, so it can update the config bits (if they were programmed, and so as to leave the bootloader (and potentially go back into the main application)

//Unlock Configs Command Definitions
#define UNLOCKCONFIG				0x00	//Sub-command for the ERASE_DEVICE command
#define LOCKCONFIG					0x01	//Sub-command for the ERASE_DEVICE command

//Query Device Response "Types" 
#define	TypeProgramMemory			0x01	//When the host sends a QUERY_DEVICE command, need to respond by populating a list of valid memory regions that exist in the device (and should be programmed)
#define TypeEEPROM					0x02
#define TypeConfigWords				0x03
#define	TypeEndOfTypeList			0xFF	//Sort of serves as a "null terminator" like number, which denotes the end of the memory region list has been reached.


//BootState Variable States
#define	Idle						0x00
#define NotIdle						0x01

//OtherConstants
#define InvalidAddress				0xFFFFFFFF

//Application and Microcontroller constants
#define BytesPerAddressPIC18		0x01		//One byte per address.  PIC24 uses 2 bytes for each address in the hex file.

#define	TotalPacketSize				0x40
#define WORDSIZE					0x02	//PIC18 uses 2 byte words, PIC24 uses 3 byte words.
#define RequestDataBlockSize 		0x3A	//Number of data bytes in a standard request to the PC.  Must be an even number from 2-58 (0x02-0x3A).  Larger numbers make better use of USB bandwidth and 
											//yeild shorter program/verify times, but require more micrcontroller RAM for buffer space.
#define BufferSize 					0x40

/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include "typedefs.h"
#include "usb.h"
#include "io_cfg.h"             // I/O pin mapping


typedef union 
{
		unsigned char Contents[64];

		//General command (with data in it) packet structure used by PROGRAM_DEVICE and GET_DATA commands 		
		struct{
			unsigned char Command;
			unsigned long Address;
			unsigned char Size;
//			unsigned char PadBytes[58-RequestDataBlockSize];	//Uncomment this if using a smaller than 0x3A RequestDataBlockSize.  Compiler doesn't like 0 byte array when using 58 byte data block size.
			unsigned char Data[RequestDataBlockSize];
		};
		
		//This struct used for responding to QUERY_DEVICE command (on a device with four programmable sections)
		struct{
			unsigned char Command;
			unsigned char PacketDataFieldSize;
			unsigned char BytesPerAddress;
			unsigned char Type1;
			unsigned long Address1;
			unsigned long Length1;
			unsigned char Type2;
			unsigned long Address2;
			unsigned long Length2;
			unsigned char Type3;
			unsigned long Address3;
			unsigned long Length3;
			unsigned char Type4;
			unsigned long Address4;
			unsigned long Length4;						
			unsigned char Type5;
			unsigned long Address5;
			unsigned long Length5;
			unsigned char Type6;
			unsigned long Address6;
			unsigned long Length6;			
			unsigned char ExtraPadBytes[7];
		};		
		
		struct{						//For UNLOCK_CONFIG command
			unsigned char Command;
			unsigned char LockValue;
		};
} PacketToFromPC;		
	

/** V A R I A B L E S ********************************************************/
#pragma udata SomeSectionName1
unsigned short long ProgramMemStopAddress;
unsigned char BootState;
unsigned int ErasePageTracker;
unsigned char BufferedDataIndex;
unsigned short long ProgrammedPointer;
unsigned char ConfigsLockValue;
unsigned char ProgrammingBuffer[BufferSize];

#pragma udata SomeSectionName2
PacketToFromPC PacketFromPC;
PacketToFromPC PacketToPC;

/** P R I V A T E  P R O T O T Y P E S ***************************************/
//void BlinkUSBStatus(void);
void UserInit(void);
void WriteFlashBlock(void);
void WriteConfigBits(void);
void WriteEEPROM(void);
void UnlockAndActivate(void);



/** D E C L A R A T I O N S **************************************************/
#pragma code
void UserInit(void)
{
    mInitAllLEDs();		//Init them off.

	//Initialize bootloader state variables
	BootState = Idle;
	ProgrammedPointer = InvalidAddress;	
	BufferedDataIndex = 0;
	ConfigsLockValue = TRUE;
}//end UserInit


/******************************************************************************
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
 * Overview:        This function is a place holder for other user routines.
 *                  It is a mixture of both USB and non-USB tasks.
 *
 * Note:            None
 *****************************************************************************/
void ProcessIO(void)
{
	unsigned char i;

	if(BootState == Idle)
	{
		if(!mHIDRxIsBusy())	//Did we receive a command?
		{
			HIDRxReport((char *)&PacketFromPC, 64);
			BootState = NotIdle;
			
			for(i = 0; i < TotalPacketSize; i++)		//Prepare the next packet we will send to the host, by initializing the entire packet to 0x00.
				PacketToPC.Contents[i] = 0;				//This saves code space, since we don't have to do it independently in the QUERY_DEVICE and GET_DATA cases.
		}
	}
	else //(BootState must be NotIdle)
	{	
		switch(PacketFromPC.Command)
		{
			case QUERY_DEVICE:
			{
				//Prepare a response packet, which lets the PC software know about the memory ranges of this device.
				PacketToPC.Command = QUERY_DEVICE;
				PacketToPC.PacketDataFieldSize = RequestDataBlockSize;
				PacketToPC.BytesPerAddress = BytesPerAddressPIC18;
				PacketToPC.Type1 = TypeProgramMemory;
				PacketToPC.Address1 = (unsigned long)ProgramMemStart;
				PacketToPC.Length1 = (unsigned long)(ProgramMemStop - ProgramMemStart);	//Size of program memory area
				PacketToPC.Type2 = TypeConfigWords;
				PacketToPC.Address2 = (unsigned long)ConfigWordsStartAddress;
				PacketToPC.Length2 = (unsigned long)ConfigWordsSectionLength;
				PacketToPC.Type3 = TypeProgramMemory;		//Not really program memory (User ID), but may be treated as it it was as far as the host is concerned
				PacketToPC.Address3 = (unsigned long)UserIDAddress;
				PacketToPC.Length3 = (unsigned long)(UserIDSize);
				PacketToPC.Type4 = TypeEndOfTypeList;
				#if defined(DEVICE_WITH_EEPROM)
					PacketToPC.Type4 = TypeEEPROM;
					PacketToPC.Address4 = (unsigned long)EEPROMEffectiveAddress;
					PacketToPC.Length4 = (unsigned long)EEPROMSize;
					PacketToPC.Type5 = TypeEndOfTypeList;
				#endif
				//Init pad bytes to 0x00...  Already done after we received the QUERY_DEVICE command (just after calling HIDRxReport()).
	
				if(!mHIDTxIsBusy())
				{
					HIDTxReport((char *)&PacketToPC, 64);
					BootState = Idle;
				}
			}
				break;
			case UNLOCK_CONFIG:
			{
				ConfigsLockValue = TRUE;
				if(PacketFromPC.LockValue == UNLOCKCONFIG)
				{
					ConfigsLockValue = FALSE;
				}
				BootState = Idle;
			}
				break;
			case ERASE_DEVICE:
			{
				//First erase main program flash memory
				for(ErasePageTracker = StartPageToErase; ErasePageTracker < (unsigned int)(MaxPageToErase + 1); ErasePageTracker++)
				{
					ClrWdt();
					TBLPTR = ((unsigned short long)ErasePageTracker << 6);
					EECON1 = 0b10010100;	//Prepare for erasing flash memory
					UnlockAndActivate();
					USBDriverService(); 	//Call USBDriverService() periodically to prevent falling off the bus if any SETUP packets should happen to arrive.
				}
				
				#if defined(DEVICE_WITH_EEPROM)
				//Now erase EEPROM (if any is present on the device)
				i = 0x00;
				do{
					EEADR = i;
					EEDATA = 0xFF;
					EECON1 = 0b00000100;	//EEPROM Write mode
					USBDriverService(); 	//Call USBDriverService() periodically to prevent falling off the bus if any SETUP packets should happen to arrive.
					UnlockAndActivate();					
				}while(++i);
				#endif

				//Now erase the User ID space (0x200000 to 0x200007)
				TBLPTR = UserIDAddress;
				EECON1 = 0b10010100;	//Prepare for erasing flash memory
				UnlockAndActivate();

				BootState = Idle;				
			}
				break;
			case PROGRAM_DEVICE:
			{
				//Check if host is trying to program the config bits
				if(PacketFromPC.Contents[3] == 0x30) // 			//PacketFromPC.Contents[3] is bits 23:16 of the address.  
				{													//0x30 implies config bits
					if(ConfigsLockValue == FALSE)
					{
						WriteConfigBits();		//Doesn't get reprogrammed if the UNLOCK_CONFIG (LockValue = UNLOCKCONFIG) command hasn't previously been sent
					}
					BootState = Idle;
					break;
				}

				#if defined(DEVICE_WITH_EEPROM)
				//Check if host is trying to program the EEPROM
				if(PacketFromPC.Contents[3] == 0xF0)	//PacketFromPC.Contents[3] is bits 23:16 of the address.  
				{										//0xF0 implies EEPROM
					WriteEEPROM();
					BootState = Idle;
					break;
				}
				#endif

				if(ProgrammedPointer == (unsigned short long)InvalidAddress)
					ProgrammedPointer = PacketFromPC.Address;
				
				if(ProgrammedPointer == (unsigned short long)PacketFromPC.Address)
				{
					for(i = 0; i < PacketFromPC.Size; i++)
					{
						ProgrammingBuffer[BufferedDataIndex] = PacketFromPC.Data[i+(RequestDataBlockSize-PacketFromPC.Size)];	//Data field is right justified.  Need to put it in the buffer left justified.
						BufferedDataIndex++;
						ProgrammedPointer++;
						if(BufferedDataIndex == ProgramBlockSize)
						{
							WriteFlashBlock();
						}
					}
				}
				//else host sent us a non-contiguous packet address...  to make this firmware simpler, host should not do this without sending a PROGRAM_COMPLETE command in between program sections.
				BootState = Idle;
			}
				break;
			case PROGRAM_COMPLETE:
			{
				WriteFlashBlock();
				ProgrammedPointer = InvalidAddress;		//Reinitialize pointer to an invalid range, so we know the next PROGRAM_DEVICE will be the start address of a contiguous section.
				BootState = Idle;
			}
				break;
			case GET_DATA:
			{
				//Init pad bytes to 0x00...  Already done after we received the QUERY_DEVICE command (just after calling HIDRxReport()).
				PacketToPC.Command = GET_DATA;
				PacketToPC.Address = PacketFromPC.Address;
				PacketToPC.Size = PacketFromPC.Size;


				TBLPTR = (unsigned short long)PacketFromPC.Address;
				for(i = 0; i < PacketFromPC.Size; i++)
				{
					if(PacketFromPC.Contents[3] == 0xF0)	//PacketFromPC.Contents[3] is bits 23:16 of the address.  
					{										//0xF0 implies EEPROM, which doesn't use the table pointer to read from
						#if defined(DEVICE_WITH_EEPROM)
						EEADR = (((unsigned char)PacketFromPC.Address) + i);	//The bits 7:0 are 1:1 mapped to the EEPROM address space values
						EECON1 = 0b00000000;	//EEPROM read mode
						EECON1bits.RD = 1;
						PacketToPC.Data[i+((TotalPacketSize - 6) - PacketFromPC.Size)] = EEDATA;					
						#endif
					}
					else	//else must have been a normal program memory region, or one that can be read from with the table pointer
					{
						_asm
						tblrdpostinc
						_endasm
						PacketToPC.Data[i+((TotalPacketSize - 6) - PacketFromPC.Size)] = TABLAT;					
					}
				}

				if(!mHIDTxIsBusy())
				{
					HIDTxReport((char *)&PacketToPC, 64);
					BootState = Idle;
				}
			}
				break;
			case RESET_DEVICE:
			{
				UCONbits.SUSPND = 0;		//Disable USB module
				UCON = 0x00;				//Disable USB module
				//And wait awhile for the USB cable capacitance to discharge down to disconnected (SE0) state. 
				//Otherwise host might not realize we disconnected/reconnected when we do the reset.
				//A basic for() loop decrementing a 16 bit number would be simpler, but seems to take more code space for
				//a given delay.  So do this instead:
				for(i = 0; i < 0xFF; i++)
				{
					WREG = 0xFF;
					while(WREG)
					{
						WREG--;
						_asm
						bra	0	//Equivalent to bra $+2, which takes half as much code as 2 nop instructions
						bra	0	//Equivalent to bra $+2, which takes half as much code as 2 nop instructions
						_endasm	
					}
				}
				Reset();
			}
				break;
		}//End switch
	}//End if/else

}//End ProcessIO()


void WriteFlashBlock(void)		//Use to write blocks of data to flash.
{
	unsigned char i;
	unsigned char BytesTakenFromBuffer = 0;
	unsigned char CorrectionFactor;

	TBLPTR = (ProgrammedPointer - BufferedDataIndex);

	//Check the lower 5 bits of the TBLPTR to verify it is pointing to a 32 byte aligned block (5 LSb = 00000).
	//If it isn't, need to somehow make it so before doing the actual loading of the programming latches.
	//In order to maximize programming speed, the PC application meant to be used with this firmware will not send 
	//large blocks of 0xFF bytes.  If the PC application
	//detects a large block of unprogrammed space in the hex file (effectively = 0xFF), it will skip over that
	//section and will not send it to the firmware.  This works, because the firmware will have already done an
	//erase on that section of memory when it received the ERASE_DEVICE command from the PC.  Therefore, the section
	//can be left unprogrammed (after an erase the flash ends up = 0xFF).
	//This can result in a problem however, in that the next genuine non-0xFF section in the hex file may not start 
	//on a 32 byte aligned block boundary.  This needs to be handled with care since the microcontroller can only 
	//program 32 byte blocks that are aligned with 32 byte boundaries.
	//So, use the below code to avoid this potential issue.

	#if(ProgramBlockSize == 0x20)
		CorrectionFactor = (TBLPTRL & 0b00011111);	//Correctionfactor = number of bytes tblptr must go back to find the immediate preceeding 32 byte boundary
		TBLPTRL &= 0b11100000;						//Move the table pointer back to the immediately preceeding 32 byte boundary
	#elif(ProgramBlockSize == 0x10)
		CorrectionFactor = (TBLPTRL & 0b00001111);	//Correctionfactor = number of bytes tblptr must go back to find the immediate preceeding 16 byte boundary
		TBLPTRL &= 0b11110000;						//Move the table pointer back to the immediately preceeding 16 byte boundary
	#else
		#error Double click this error message and fix this section for your microcontroller type.
	#endif

	for(i = 0; i < ProgramBlockSize; i++)	//Load the programming latches
	{
		if(CorrectionFactor == 0)
		{
			if(BufferedDataIndex != 0)	//If the buffer isn't empty
			{
				TABLAT = ProgrammingBuffer[BytesTakenFromBuffer];
				_asm tblwtpostinc _endasm
				BytesTakenFromBuffer++;
				BufferedDataIndex--;	//Used up a byte from the buffer.
			}
			else	//No more data in buffer, need to write 0xFF to fill the rest of the programming latch locations
			{
				TABLAT = 0xFF;
				_asm tblwtpostinc _endasm				
			}
		}
		else
		{
			TABLAT = 0xFF;
			_asm tblwtpostinc _endasm
			CorrectionFactor--;
		}
	}

//	TBLPTR--;		//Need to make table pointer point to the region which will be programmed before initiating the programming operation
	_asm tblrdpostdec _endasm	//Do this instead of TBLPTR--; since it takes less code space.
		
	EECON1 = 0b10100100;	//flash programming mode
	UnlockAndActivate();

	//Now need to fix the ProgrammingBuffer[].  We may not have taken a full 32 bytes out of the buffer.  In this case,
	//the data is no longer justified correctly.
	for(i = 0; i < BufferedDataIndex; i++)	//Need to rejustify the remaining data to the "left" of the buffer (if there is any left)
	{
		ProgrammingBuffer[i] = ProgrammingBuffer[BytesTakenFromBuffer+i];
	}
}


void WriteConfigBits(void)	//Also used to write the Device ID
{
	unsigned char i;

	TBLPTR = (unsigned short long)PacketFromPC.Address;

	for(i = 0; i < PacketFromPC.Size; i++)
	{
		TABLAT = PacketFromPC.Data[i+(RequestDataBlockSize-PacketFromPC.Size)];
		_asm
		tblwt
		_endasm

		EECON1 = 0b11000100;	//Config bits programming mode
		UnlockAndActivate();

		_asm
		tblrdpostinc
		_endasm
	}
}

#if defined(DEVICE_WITH_EEPROM)
void WriteEEPROM(void)
{
	unsigned char i;
	
	for(i = 0; i < PacketFromPC.Size; i++)
	{
		EEADR = (((unsigned char)PacketFromPC.Address) + i);
		EEDATA = PacketFromPC.Data[i+(RequestDataBlockSize-PacketFromPC.Size)];

		EECON1 = 0b00000100;	//EEPROM Write mode
		UnlockAndActivate();
	}

}
#endif

void UnlockAndActivate(void)
{
	INTCONbits.GIE = 0;		//Make certain interrupts disabled for unlock process.
	_asm
	//Now unlock sequence to set WR (make sure interrupts are disabled before executing this)
	MOVLW 0x55
	MOVWF EECON2, 0
	MOVLW 0xAA
	MOVWF EECON2, 0
	BSF EECON1, 1, 0		//Performs write
	_endasm	
	while(EECON1bits.WR);	//Wait until complete (relevant when programming EEPROM, not important when programming flash since processor stalls during flash program)	
	EECON1bits.WREN = 0;  	//Good practice now to clear the WREN bit, as further protection against any accidental activation of self write/erase operations.
}	

/** EOF Boot4450Family.c *********************************************************/
