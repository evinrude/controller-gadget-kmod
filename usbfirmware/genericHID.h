/************************************************************************
	genericHID.h

    usbGenericHidCommunication reference firmware 2_0_0_0
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

#include "memorymap.h"

#ifndef GENERICHID_H
#define GENERICHID_H

unsigned char ReceivedDataBuffer[64]    @ USBRAM_RECEIVEBUFFER;
unsigned char ToSendDataBuffer[64]      @ USBRAM_SENDBUFFER;

USB_HANDLE USBOutHandle = 0;
USB_HANDLE USBInHandle = 0;
BOOL blinkStatusValid = TRUE;

#endif