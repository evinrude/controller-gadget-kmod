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

/************************************************************************
 * Holds a few config items
************************************************************************/
#define _LCD_C

/************************************************************************
 * Header includes
************************************************************************/
#include	<htc.h>
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	"lcd.h"
#include	"HardwareProfile.h"

/************************************************************************
 * Static variable
************************************************************************/
#define SMALL_DELAY	40

/************************************************************************
 * lcdWriteNybble function
 * This function will turn the appropriate pins on
 * Please ensure the LCD is hooked up correctly
 * The HardwareProfile.h will have the pin designations defined
************************************************************************/
void lcdWriteNybble(unsigned char nybble)
{
	if (nybble & 0b00000001) LCD_DB4 = 1; else LCD_DB4 = 0;
	if (nybble & 0b00000010) LCD_DB5 = 1; else LCD_DB5 = 0;
	if (nybble & 0b00000100) LCD_DB6 = 1; else LCD_DB6 = 0;
	if (nybble & 0b00001000) LCD_DB7 = 1; else LCD_DB7 = 0;
}

/************************************************************************
 * lcdWrite function
************************************************************************/
void lcdWrite(unsigned char address, unsigned char c)
{
	LCD_RS = address;
	__delay_us(SMALL_DELAY);

	// Write the 4 bits non-distructively to the port
	lcdWriteNybble((c >> 4) & 0x0F); 

	// Toggle the clock bit
	LCD_EN = 1;
	__delay_us(SMALL_DELAY);
	LCD_EN = 0;
	__delay_us(SMALL_DELAY);

	// Write the 4 bits non-distructively to the port
	lcdWriteNybble(c & 0x0F);

	// Toggle the clock bit
	LCD_EN = 1;
	__delay_us(SMALL_DELAY);
	LCD_EN = 0;
	__delay_us(SMALL_DELAY);
}

/************************************************************************
 * lcdPutch function
************************************************************************/
void lcdPutch(char c)
{
	// This routine needs updating so it monitors the cursor posiiton
	LCD_RS = 1;	// write characters
	lcdWrite(DATA_REG, c);
}

/************************************************************************
 * lcdClearDisplay function
************************************************************************/
void lcdClearDisplay(void)
{
	lcdWrite(CMD_REG, LCD_CLR);

	// Reset our internal cursor position
	lcdX = 0;
	lcdY = 0;

	// Delay
	for (int counter = 0; counter < 200; counter++) __delay_us(100);
}

/************************************************************************
 * lcdInit fuction
 * This needs to be called first!
************************************************************************/
void lcdInit(void)
{
	int counter;
	
	// Power up delay
	for (counter = 0; counter < 2500; counter++) __delay_us(100);

	LCD_EN = 0; // Set clock low
	LCD_RS = CMD_REG; // Set RS to command register

	// Set up the interface
	lcdWriteNybble(LCD_WAKE);

	LCD_EN = 1;
	LCD_EN = 0;
	for (counter = 0; counter < 1500; counter++) __delay_us(100);
	LCD_EN = 1;
	LCD_EN = 0;
	__delay_us(200);
	LCD_EN = 1;
	LCD_EN = 0;
	__delay_us(200);

	// Set the interfact to 4 bits
	lcdWriteNybble(LCD_4BIT);
	__delay_us(SMALL_DELAY);
	LCD_EN = 1;
	__delay_us(SMALL_DELAY);
	LCD_EN = 0;

	// Don't know what this does...
	lcdWrite(CMD_REG, 0x28);

	// Enable display / hide cursor
	lcdWrite(CMD_REG, LCD_ENDISP);

	// Clear the display and home the cursor
	lcdClearDisplay();

	// Set the cursor movement direction to right
	lcdWrite(CMD_REG, LCD_CURRIGHT);
	
	// Turn on display
	lcdWrite(CMD_REG, LCD_DISPON);

	// Reset our internal cursor position
	lcdX = 0;
	lcdY = 0;
}

/************************************************************************
 * lcdDisplayState function
************************************************************************/
void lcdDisplayState(int state)
{
	if (state == 1) lcdWrite(CMD_REG, LCD_DISPON);
		else lcdWrite(CMD_REG, LCD_DISPOFF);
}

/************************************************************************
 * lcdGoto function
************************************************************************/
void lcdGoto(int x, int y)
{
	int offset;

	// Select the correct line of the display
	switch (y)
	{
		case 0: offset = 0x00;
				break;
		case 1: offset = 0x40;
				break;
		case 2: offset = 0x14;
				break;
		case 3: offset = 0x54;
				break;
	}

	// Select the correct character of the line
	offset += x;

	// Send the command to the LCD
	lcdWrite(CMD_REG, LCD_DDSET | offset);

	// Reset our internal cursor position
	lcdX = x;
	lcdY = y;
}

/************************************************************************
 * lcdPuts function
************************************************************************/
void lcdPuts(const char *string)
{
	// Since the 4 line display is in the order line 1, line 3, line 2, line 4
	// we need to read the current cursor position and control how the text is
	// output, adjusting the cursor position as we go
	int loop;

	for (loop = 0; loop < strlen(string); loop++)
	{
		// Write the character to the LCD
		lcdWrite(DATA_REG, string[loop]);
		lcdX++;

		// Have we reached the end of the line?
		if (lcdX == 20)
		{
			// Move to the start of the next line
			lcdX = 0;
			lcdY++;

			// If we are off the bottom of the screen
			// go back to the top line
			if (lcdY == 4) lcdY = 0;

			// Adjust the cursor position on the LCD
			lcdGoto(lcdX, lcdY);
		}
	}
}