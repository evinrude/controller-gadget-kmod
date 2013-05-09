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

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <libcontroller-gadget.h>

static int fd = 0;

void __gadgetinit(void) __attribute__ ((constructor));
void __gadgetdestroy(void) __attribute__ ((destructor));

/*
 * open the gadget from sysfs
 * sets the fd
 */
void gadgetopen()
{
	struct stat gadgetstatus;
    stat(GADGET, &gadgetstatus);
	if(gadgetstatus.st_mode & S_IREAD && gadgetstatus.st_mode & S_IWRITE)
	{
		fprintf(stderr, "libcontroller-gadget: You do not have the permissions to open the gadget [%s]: %s\n", GADGET, strerror(errno));
		fd = -1;
		errno = EACCES;
	}
	else
	{
		fd = open(GADGET,O_RDWR);
    	if(fd <= 0)
    	{
        	fprintf(stderr,"libcontroller-gadget: Unable to open the gadget [%s]: %s\n", GADGET, strerror(errno));
    	}
	}
}

/*
 * close the gadget from sysfs
 * closes the fd
 */
void gadgetclose()
{
	close(fd);
}

/*
 * write a byte to the gadget
 * returns 0 on success, returns -1
 * on failure and sets errno
 */
int gadgetwrite(unsigned char byte)
{
	gadgetopen();

	if(fd <= 0)
	{
		return -1;
	}

	if(write(fd,(char*) &byte,1) != 1)
	{
		//write sets errno
		gadgetclose();
		return -1;
	}

	gadgetclose();
	return 0;
}

/*
 * get the status of the gadget
 * returns the status on success and 0xFF on failure
 * check errno on failure
 */
int gadgetstatus(int timeout, unsigned char *status)
{
	char temp = STATUS_CODE;
	unsigned char buff[2];
	float current_wait = 0.0;

	if(timeout == 0)
	{
		errno = EINVAL;
		return -1;
	}

	gadgetopen();

	if(fd <= 0)
	{
		gadgetclose();
		return -1;
	}

	if(write(fd,(char*) &temp,1) != 1)
	{
		//write sets errno
		gadgetclose();
		return -1;
	}

	while((read(fd,buff,2)) == 0)
	{
		usleep(500);
		current_wait++;
		if((current_wait / 2) == timeout)
		{
			errno = ETIME;
			gadgetclose();
			return -1;
		}
	}

	*status = buff[0];
	gadgetclose();
	return 0;
}

void gadgetrefresh(void)
{
	gadgetclose();
	gadgetopen();
}

void __gadgetinit(void);

void __gadgetdestroy(void)
{
	gadgetclose();
}
