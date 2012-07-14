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
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main()
{
	int fd=0;
	char input[2];
	char buff[80]="";
	
	
	fd=open("/dev/gadget",O_RDWR);
	
	for(;;)
	{
			printf("gadget> ");
			scanf("%s",input);
			
			switch(input[0])
			{
				case '0':
					write(fd,"0",1);
					break;
				case '1':
					write(fd,"1",1);
					break;
				case '2':
					write(fd,"2",1);
					break;
				case '3':
					write(fd,"3",1);
					break;
				case '4':
					write(fd,"4",1);
					break;
				case '5':
					write(fd,"5",1);
					break;
				case '6':
					write(fd,"6",1);
					break;
				case 'q':
					goto quit;
				case 's':
					write(fd,"S",1);
					while((read(fd,buff,10)) == 0)
					{
						usleep(500);
					}
					printf("Status [%c]\n", buff[0]);
					break;
				default:
					continue;
			}
	}
	
quit:
	close(fd);
	return 0;	
}
