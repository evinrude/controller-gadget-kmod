/**
 * 
 * USB Controller Gadget Shared Library
 * Copyright (C) 2012 Mickey Malone     <mickey.malone at gmail.com>
 * 
 * This is the Shared Library to the kernel driver. It provides basic read
 * and write functionality in a thread safe manner.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 * 
 **/

#define GADGET              "/dev/gadget"
#define STATUS_CODE         0x09
#define ERROR_CODE          0xFF

/*  
 * write a byte to the gadget
 * returns 0 on success, returns -1 
 * on failure and sets errno
 */
extern int gadgetwrite(unsigned char byte);

/*  
 * get the status of the gadget
 * returns the status on success and 0xFF on failure
 * check errno on failure
 */
extern unsigned char gadgetstatus(int timeout);
