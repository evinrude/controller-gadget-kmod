/**
 *
 * USB Controller Gadget Daemon (sprinkler-controller.c)
 * Copyright (C) 2012 Mickey Malone     <mickey.malone at gmail.com>
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
 **/

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include "sprinkler-controller.h"
#include "sprinkler-logger.h"

static unsigned char status = '0';

//---------------------------------------------------------
//
// Function:    get_status_response
// Description  returns the current status of the controller
// Arguments:   void
//
// Returns:     controller status
//
//---------------------------------------------------------
unsigned char get_status_response(void)
{
	return status;
}

//---------------------------------------------------------
//
// Function:    fire
// Description  Program entry point
// Arguments:   int zone
//
// Returns:     exit status
//
//---------------------------------------------------------
int sendSprinklerZoneCommand(controller_command_code command)
{
	int retval = 0;

	switch (command) {
	case CMD_TOGGLE_ZONEOFF:
		if (gadgetwrite((unsigned char)OFF) == -1) {
			logerror("Unable to send zoneoff [%s]", strerror(errno));
			retval = -1;
		}
		break;
	case CMD_TOGGLE_ZONE1:
		if (gadgetwrite((unsigned char)SPRINKLER_ZONE1) == -1) {
			logerror("Unable to send zone1 [%s]", strerror(errno));
			retval = -1;
		}
		break;
	case CMD_TOGGLE_ZONE2:
		if (gadgetwrite((unsigned char)SPRINKLER_ZONE2) == -1) {
			logerror("Unable to send zone2 [%s]", strerror(errno));
			retval = -1;
		}
		break;
	case CMD_TOGGLE_ZONE3:
		if (gadgetwrite((unsigned char)SPRINKLER_ZONE3) == -1) {
			logerror("Unable to send zone3 [%s]", strerror(errno));
			retval = -1;
		}
		break;
	case CMD_TOGGLE_ZONE4:
		if (gadgetwrite((unsigned char)SPRINKLER_ZONE4) == -1) {
			logerror("Unable to send zone4 [%s]", strerror(errno));
			retval = -1;
		}
		break;
	case CMD_TOGGLE_ZONE5:
		if (gadgetwrite((unsigned char)SPRINKLER_ZONE5) == -1) {
			logerror("Unable to send zone5 [%s]", strerror(errno));
			retval = -1;
		}
		break;
	case CMD_TOGGLE_ZONE6:
		if (gadgetwrite((unsigned char)SPRINKLER_ZONE6) == -1) {
			logerror("Unable to send zone6 [%s]", strerror(errno));
			retval = -1;
		}
		break;
	case CMD_STATUS:
		if (gadgetstatus(TIMEOUT, &status)  == -1) {
			logerror("Status error [%s]", strerror(errno));
			retval = -1;
		}
		break;
	}

	return retval;
}
