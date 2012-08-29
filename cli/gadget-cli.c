/**
 *
 * USB Controller Gadget CLI
 * Copyright (C) 2012 Mickey Malone     <mickey.malone at gmail.com>
 *
 * This is the CLI to the kernel driver.
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <libcontroller-gadget.h>


void werror(void)
{
	perror("Write failed\n");
	exit(errno);
}

int main()
{
	char input[32768];
	char status;
	int timeout = 5;

	for(;;)
	{
		printf("gadget> ");
		if(scanf("%s",input) == 0)
		{
			perror("Unable to get input\n");
			exit(errno);
		}

		switch(input[0])
		{
		case '0':
			if(gadgetwrite((unsigned char) 0x10) == -1)
			{
				werror();
			}
			break;
		case '1':
			if(gadgetwrite((unsigned char) 0x11) == -1)
			{
				werror();
			}
			break;
		case '2':
			if(gadgetwrite((unsigned char) 0x12) == -1)
			{
				werror();
			}
			break;
		case '3':
			if(gadgetwrite((unsigned char) 0x13) == -1)
			{
				werror();
			}
			break;
		case '4':
			if(gadgetwrite((unsigned char) 0x14) == -1)
			{
				werror();
			}
			break;
		case '5':
			if(gadgetwrite((unsigned char) 0x15) == -1)
			{
				werror();
			}
			break;
		case '6':
			if(gadgetwrite((unsigned char) 0x16) == -1)
			{
				werror();
			}
			break;
		case 'q':
			goto quit;
		case 's':
			status = gadgetstatus(timeout);
			if(status == ERROR_CODE)
			{
				perror("Status: ERROR");
			}
			else
			{
				printf("Status: %c\n",status);
			}
			break;
		default:
			printf("Unrecognized command [%c]. ",input[0]);
			printf("Expected commands are 0, 1, 2, 3, 4, 5, 6, s, q\n");
			continue;
		}
	}

quit:
	return 0;
}
