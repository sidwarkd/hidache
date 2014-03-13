#ifndef __GENERIC_TYPE_DEFS_H_
#include "GenericTypeDefs.h"
#endif

// This table assists in the conversion of ASCII chars to HID commands
// Each row contains 3 pieces of data in the following order:
// [ASCII Code][HID Code][needShiftKey]
// Add to the table as needed to support new keys
#pragma idata
ram const char KeyTable[36][3] =   { 
{10,0,0},   //Line Feed
{13,40,0}, //Enter
{27,41,0}, //Escape
{33,30,1}, // !
{34,52,1}, // "
{35,32,1}, // #
{36,33,1}, // $
{37,34,1}, // %
{38,36,1}, // &
{40,38,1}, // (
{41,39,1}, // )
{42,37,1}, // *
{43,46,1}, // +
{44,54,0}, // , (comma)
{39,52,0}, // ' (single quote)
{45,45,0}, // -
{46,55,0}, // .
{47,56,0}, // /
{48,39,0}, // zero
{58,51,1}, // :
{59,51,0}, // ;
{60,54,1}, // <
{61,46,0}, // =
{62,55,1}, // >
{63,56,1}, // ?
{64,31,1}, // @
{91,47,0}, // [
{92,49,0}, // backslash
{93,48,0}, // ]
{94,35,1}, // ^
{95,45,1}, // _ (underscore)
{123,47,1},// {
{124,49,1},// |
{125,48,1},// }
{126,53,1},// ~
{127,42,0},// Delete
};

// If you add to the table above make sure you change this value to be
// the new length of the array
#define TABLE_LEN	36

// This function translates the ASCII characters and special characters
// to their equivalent HID usage to be sent to the PC
#pragma code
BOOL TranslateAsciiToHID(char asciiKey, char *hidKey, BOOL *needShiftKey)
{
	short i = 0;

	// Lets initialize this to false and then just set it if we need it
	*needShiftKey = FALSE;
	
	// Lowercase ASCII characters
	if((asciiKey >= 97) && (asciiKey <= 122))
	{
		*hidKey = asciiKey - 93;
		return TRUE;
	}
	// Uppercase ASCII characters
	else if((asciiKey >= 65) && (asciiKey <= 90))
	{
		*needShiftKey = TRUE;
		*hidKey = asciiKey - 61;
		return TRUE;
	}
	// Numbers
	else if((asciiKey >= 49) && (asciiKey <= 57))
	{
		*hidKey = asciiKey - 19;
		return TRUE;
	}
	// Spacebar.  I decided not to include this in the table because it could be
	// a regular occurance and looking for it in the table will take more time
	else if(asciiKey == 32)
	{
		*hidKey = 44;
		return TRUE;
	}
	// Look for the character in the key table defined above
	else
	{
		// Look through the table to see if we can find it there
		for(i = 0; i < TABLE_LEN; i++)
		{
			if(KeyTable[i][0] == asciiKey) // We've found the entry
			{
				*hidKey = KeyTable[i][1];
				*needShiftKey = KeyTable[i][2];
				return TRUE;
			}
		}

		// Send a z to indicate an error
		*hidKey = 29;
		return FALSE;
	}
}