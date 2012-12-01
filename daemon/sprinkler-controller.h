//
//  controller.h
//  sprinklerdaemon
//
//  Created by Mickey Malone on 4/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include <libcontroller-gadget.h>

#ifndef sprinklerdaemon_sprinkler_controller_h
#define sprinklerdaemon_sprinkler_controller_h

typedef enum
{
	CMD_TOGGLE_ZONEOFF      = 0,
	CMD_TOGGLE_ZONE1        = 1,
	CMD_TOGGLE_ZONE2        = 2,
	CMD_TOGGLE_ZONE3        = 3,
	CMD_TOGGLE_ZONE4        = 4,
	CMD_TOGGLE_ZONE5        = 5,
	CMD_TOGGLE_ZONE6        = 6,
	CMD_STATUS              = 115
} controller_command_code;


//---------------------------------------------------------
//
// Prototype Functions
//
//---------------------------------------------------------
int sendSprinklerZoneCommand(controller_command_code command);
unsigned char get_status_response(void);

//---------------------------------------------------------
//
// Static constants
//
// VENDOR_ID                        =   Vendor id of the usb device
// PRODUCT_ID                       =   Product id of the usb device
// TIMEOUT                       	=   Timeout used when querying the controller library for status in seconds
// INTERFACE_NUMBER                 =   The interface we want to use
// INTERRUPT_IN_ENDPOINT            =   USB device IN endpoint set in firmware
// INTERRUPT_OUT_ENDPOINT           =   USB device OUT endpoint set in firmware
// MAX_INTERRUPT_IN_TRANSFER_SIZE   =   IN buffer - must be supported in firmware
// MAX_INTERRUPT_OUT_TRANSFER_SIZE  =   OUT buffer - must be supported in firmware
// STATUS                           =   Status command
// OFF                              =   Off command
// SPRINKLER_ZONE1                  =   Zone1 command
// SPRINKLER_ZONE2                  =   Zone2 command
// SPRINKLER_ZONE3                  =   Zone3 command
// SPRINKLER_ZONE4                  =   Zone4 command
// SPRINKLER_ZONE5                  =   Zone5 command
// SPRINKLER_ZONE6                  =   Zone6 command
//
//---------------------------------------------------------
static const int VENDOR_ID = 0x04d8;
static const int PRODUCT_ID = 0x0042;
static const int TIMEOUT = 5000;
static const int INTERFACE_NUMBER = 0;
static const int INTERRUPT_IN_ENDPOINT = 0x81;
static const int INTERRUPT_OUT_ENDPOINT = 0x01;
static const int MAX_INTERRUPT_IN_TRANSFER_SIZE = 64;
static const int MAX_INTERRUPT_OUT_TRANSFER_SIZE = 64;
static const char STATUS = 0x09;
static const char OFF = 0x10;
static const char SPRINKLER_ZONE1 = 0x11;
static const char SPRINKLER_ZONE2 = 0x12;
static const char SPRINKLER_ZONE3 = 0x13;
static const char SPRINKLER_ZONE4 = 0x14;
static const char SPRINKLER_ZONE5 = 0x15;
static const char SPRINKLER_ZONE6 = 0x16;
#endif
