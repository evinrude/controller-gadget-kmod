/************************************************************************
	memorymap.h

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

#ifndef MEMORYMAP_H
#define MEMORYMAP_H

// Since Hi Tech C doesn't have the pragma functionality for moving things
// around in memory we have to manually define the locations for the 
// USBRAM variables here.  The defines are used at the appropriate places
// in the code.

#define USBRAM_PINGPONGBUFFER	0x400
#define USBRAM_RECEIVEBUFFER	0x480
#define USBRAM_SENDBUFFER		0x4C0
#define USBRAM_EP0_SETUPPKT		0x500
#define USBRAM_EP0_BUFFER		0x520
#define USBRAM_HIDOUT			0x530
#define USBRAM_HIDIN			0x570

#endif