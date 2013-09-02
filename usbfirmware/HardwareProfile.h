/************************************************************************
	HardwareProfile.h

************************************************************************/

#ifndef HARDWARE_PROFILE_H
    #define HARDWARE_PROFILE_H

    // Defines required for the USB stack
    #define self_power                          1
    #define USE_USB_BUS_SENSE_IO
    #define USB_BUS_SENSE                       1
    #define DEMO_BOARD                          PICDEM_FS_USB
    #define PICDEM_FS_USB
    #define CLOCK_FREQ                          48000000

    // Fosc frequency (48 Mhz)
    #define _XTAL_FREQ                          48000000

    // Device Vendor Indentifier (VID)
    #define USB_VID                             0x04D8

    // Device Product Indentifier (PID)
    #define USB_PID                             0x0042

    // Manufacturer string descriptor
    #define MSDLENGTH                           14
    #define MSD                                 'M','i','c','k','e','y',' ','C','r','e','a','t','e','s'

    // Product String descriptor
    #define PSDLENGTH                           20
    #define PSD                                 'S','p','r','i','n','k','l','e','r',' ','C','o','n','t','r','o','l','l','e','r'
    // Device serial number string descriptor
    #define DSNLENGTH                           7
    #define DSN                                 'V','e','r','_','1','.','1'

    // Common useful definitions
    #define ON                                  1
    #define OFF                                 0
    #define WRITE                               0
    #define READ                                1

    // I/O pin definitions
    #define INPUT_PIN                           1
    #define OUTPUT_PIN                          0

    // Comment out the next define to disable the debug
    // functionality (saves some RAM and code space)
    //#define                                   DEBUGON

    // Defines for LCD firmware
    #define LCD_ATTACHED                        1
    #define LCD_EN                              RA1
    #define LCD_RS                              RA0
    #define LCD_DB4                             RA2
    #define LCD_DB5                             RA3
    #define LCD_DB6                             RA4
    #define LCD_DB7                             RA5

    //The Delay
    #define DELAY 6000

    //SerialDebug Flag
    #define DEBUG 0x1

    //Manual Override Button - Future
    #define TOGGLE                              RD7

#endif
