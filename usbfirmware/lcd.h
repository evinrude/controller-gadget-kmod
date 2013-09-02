/************************************************************************
	lcd.h

    usbGenericHidCommunication reference LCD example
    Copyright (C) 2010 Simon Inns

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

	Email: simon.inns@gmail.com

************************************************************************/

#ifndef LCD_H
#define LCD_H

// LCD command definitions
#define LCD_WAKE 		0x03 	// Wake up the display
#define LCD_8BIT 		0x38	// 8 bit / 2/4 lines
#define LCD_4BIT 		0x02	// 4 bit / 2/4 lines
#define LCD_ENDISP 		0x08	// Enable the display (no cursor)
#define LCD_CLR 		0x01	// Clear display and home cursor
#define LCD_CURRIGHT 	0x06	// Move cursor right on character write
#define LCD_DISPON 		0x0c	// Turn on the display
#define LCD_DISPOFF		0x00	// Turn off the display

#define LCD_DDSET		0x80	// Set the DDRAM

#define CMD_REG 		0		// Command register selection
#define DATA_REG 		1		// Data register selection

#ifdef _LCD_C
unsigned char lcdX;
unsigned char lcdY;
#endif

// Protypes...

// Write a byte to the LCD in 4 bit mode
void lcdWrite(unsigned char, unsigned char);

// Clear and home the LCD
void lcdClearDisplay(void);

// Write a string of characters to the LCD
void lcdPuts(const char * string);

// Go to the specified position
void lcdGoto(int x, int y);
	
// Intialise the LCD - call before anything else
void lcdInit(void);

// Output a single character
void lcdPutch(char);

// Set the display state (on or off)
void lcdDisplayState(int state);

#endif