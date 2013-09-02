/************************************************************************
 * Author: Mickey Malone
 * Purpose: Firmware for Linux based sprinkler controller
 * Requirements: Hi-Tech C Compiler, Pic 18F4550
************************************************************************/

/***********************************************************************
 * Header includes
 ***********************************************************************/
#include <htc.h>
#include <stdio.h>
#include "usb.h"
#include "delay.h"
#include "HardwareProfile.h"
#include "usb_function_hid.h"
#include "genericHID.h"
#include "serial.h"
#include <string.h>
#include "lcd.h"

/***********************************************************************
 * Configuration bits
 ***********************************************************************/
// Config word 1 (Oscillator configuration)
__CONFIG(1, USBPLL & IESODIS & FCMDIS & HSPLL & CPUDIV1 & PLLDIV5);
// Config word 2
__CONFIG(2, VREGEN & PWRTDIS & BOREN & BORV20 & WDTDIS & WDTPS32K);
// Config word 3
__CONFIG(3, PBDIGITAL & LPT1DIS & MCLREN);
// Config word 4
__CONFIG(4, XINSTDIS & STVREN & LVPDIS & ICPORTDIS & DEBUGDIS);
// Config word 5, 6 and 7 (protection configuration)
__CONFIG(5, UNPROTECT);
__CONFIG(6, UNPROTECT);
__CONFIG(7, UNPROTECT);


int looper = 0;

/***********************************************************************
 * Static LCD Items
 ***********************************************************************/
struct lcdm_s
{
	char init1[15];
	char init2[15];
	char zone1[15];
	char zone2[15];
	char zone3[15];
	char zone4[15];
	char zone5[15];
	char zone6[15];
        char zonestat[15];
	int linea;
	int lineb;
	int columna;

}lcm={"Controller","Ready","Zone 1","Zone 2","Zone 3","Zone 4","Zone 5","Zone 6","Active",0,1,0};

/***********************************************************************
 * Static initial LCD message
 ***********************************************************************/
void greeting(void)
{
    lcdClearDisplay();
    lcdGoto(lcm.columna,lcm.linea);
    lcdPuts(lcm.init1);
    lcdGoto(lcm.columna,lcm.lineb);
    lcdPuts(lcm.init2);
}

/***********************************************************************
 * Sprinkler toggle function: returns 0x04 on success
 *
 * 5/25/2012 -  updated the case statements to clear the display on toggle off
 ***********************************************************************/
char toggleSprinklerZone(int zone)
{
    switch (zone)
    {
        case 0:
            PORTB = 0x00;
            lcdClearDisplay();
            greeting();
            break;
        case 1:
            PORTB = (0b00000001 ^ PORTB & 0b00000001);
            if((PORTB & 0b00000001) != 0)
            {
                lcdClearDisplay();
                lcdGoto(lcm.columna,lcm.linea);
                lcdPuts(lcm.zone1);
                lcdGoto(lcm.columna,lcm.lineb);
                lcdPuts(lcm.zonestat);
            }
            else
            {
                lcdClearDisplay();
                greeting();
            }
            break;
        case 2:
            PORTB = (0b00000010 ^ PORTB & 0b00000010);
            if((PORTB & 0b00000010) != 0)
            {
                lcdClearDisplay();
                lcdGoto(lcm.columna,lcm.linea);
                lcdPuts(lcm.zone2);
                lcdGoto(lcm.columna,lcm.lineb);
                lcdPuts(lcm.zonestat);
            }
            else
            {
                lcdClearDisplay();
                greeting();
            }
            break;
        case 3:
            PORTB = (0b00000100 ^ PORTB & 0b00000100);
            if((PORTB & 0b00000100) != 0)
            {
                lcdClearDisplay();
                lcdGoto(lcm.columna,lcm.linea);
                lcdPuts(lcm.zone3);
                lcdGoto(lcm.columna,lcm.lineb);
                lcdPuts(lcm.zonestat);
            }
            else
            {
                lcdClearDisplay();
                greeting();
            }
            break;
        case 4:
            PORTB = (0b00001000 ^ PORTB & 0b00001000);
            if((PORTB & 0b00001000) != 0)
            {
                lcdClearDisplay();
                lcdGoto(lcm.columna,lcm.linea);
                lcdPuts(lcm.zone4);
                lcdGoto(lcm.columna,lcm.lineb);
                lcdPuts(lcm.zonestat);
            }
            else
            {
                lcdClearDisplay();
                greeting();
            }
            break;
        case 5:
            PORTB = (0b00010000 ^ PORTB & 0b00010000);
            if((PORTB & 0b00010000) != 0)
            {
                lcdClearDisplay();
                lcdGoto(lcm.columna,lcm.linea);
                lcdPuts(lcm.zone5);
                lcdGoto(lcm.columna,lcm.lineb);
                lcdPuts(lcm.zonestat);
            }
            else
            {
                lcdClearDisplay();
                greeting();
            }
            break;
        case 6:
            PORTB = (0b00100000 ^ PORTB & 0b00100000);
            if((PORTB & 0b00100000) != 0)
            {
                lcdClearDisplay();
                lcdGoto(lcm.columna,lcm.linea);
                lcdPuts(lcm.zone6);
                lcdGoto(lcm.columna,lcm.lineb);
                lcdPuts(lcm.zonestat);
            }
            else
            {
                lcdClearDisplay();
                greeting();
            }
            break;
        default:
            return 0x01;
    }

    return 0x04;
}

/***********************************************************************
 * IO Processing
 * This firmware uses the USB polling method
 * The method will be called about every 30 micro seconds or so
 * DO NOT! impedede the USB communication, do your business and get out!
 *
 * Here are the firmware codes
 * 0x09: Status of the zones (PORTB Register)
 * 0x10: Turn off all sprinkler zones
 * 0x11: Toggler sprinkler zone #1
 * 0x12: Toggler sprinkler zone #2
 * 0x13: Toggler sprinkler zone #3
 * 0x14: Toggler sprinkler zone #4
 * 0x15: Toggler sprinkler zone #5
 * 0x16: Toggler sprinkler zone #6
 * 0x20: Clear the LCD contents
 * 0x21: Display greeting on LCD
 *
 * On success, the value 0xC0 will be trasmitted back to the host unless the host is expecting a query response
 ***********************************************************************/
void ProcessIO(void)
{   
    if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)){return; }

    //Check for Manual Override
    if(TOGGLE)
    {
        switch(PORTB)
        {
            case 0x00:
                toggleSprinklerZone(1);
                lcdGoto(lcm.columna,lcm.lineb);
                lcdPuts("Manual Mode");
                break;
            case 0x01:
                toggleSprinklerZone(2);
                lcdGoto(lcm.columna,lcm.lineb);
                lcdPuts("Manual Mode");
                break;
            case 0x02:
                toggleSprinklerZone(3);
                lcdGoto(lcm.columna,lcm.lineb);
                lcdPuts("Manual Mode");
                break;
            case 0x04:
                toggleSprinklerZone(4);
                lcdGoto(lcm.columna,lcm.lineb);
                lcdPuts("Manual Mode");
                break;
            case 0x08:
                toggleSprinklerZone(5);
                lcdGoto(lcm.columna,lcm.lineb);
                lcdPuts("Manual Mode");
                break;
            case 0x10:
                toggleSprinklerZone(6);
                lcdGoto(lcm.columna,lcm.lineb);
                lcdPuts("Manual Mode");
                break;
            default:
                toggleSprinklerZone(0);
                break;
        }

        for(looper=0;looper <= 100; looper++)
        {
            DelayMs(10);
        }
    }

    if(!HIDRxHandleBusy(USBOutHandle))
    {
        switch (ReceivedDataBuffer[0])
        {
            case 0x09:
                ToSendDataBuffer[0] = PORTB;
                //Transmit result back to host if it is a status request
                if(!HIDTxHandleBusy(USBInHandle))
                {
                    USBInHandle = HIDTxPacket(HID_EP,(BYTE*)&ToSendDataBuffer,64);
                }
                break;
            case 0x10:
                toggleSprinklerZone(0);
                break;
            case 0x11:
                toggleSprinklerZone(1);
                break;
            case 0x12:
                toggleSprinklerZone(2);
                break;
            case 0x13:
                toggleSprinklerZone(3);
                break;
            case 0x14:
                toggleSprinklerZone(4);
                break;
            case 0x15:
                toggleSprinklerZone(5);
                break;
            case 0x16:
                toggleSprinklerZone(6);
                break;
            case 0x20:
                lcdClearDisplay();
                break;
            case 0x21:
                greeting();
                break;
            default:
                break;
        }

        //Rearm the USBOutHandle
        USBOutHandle = HIDRxPacket(HID_EP,(BYTE*)&ReceivedDataBuffer,64);
    }  
}

/***********************************************************************
 * PIC18F4550 Setup
 ***********************************************************************/
void setup()
{
    // Default all pins to digital
    ADCON1 = 0x0F;

    // Configure ports as inputs (1) or outputs(0)
    TRISA = 0b00000000;
    TRISB = 0b00000000;
    TRISC = 0b00000000;
    TRISD = 0b10000000; //RD7 as INPUT
    TRISE = 0b00000000;

    // Clear all ports
    PORTA = 0b00000000;
    PORTB = 0b00000000;
    PORTC = 0b00000000;
    PORTD = 0b00000000;
    PORTE = 0b00000000;

    // USB stack and parameter initialisation
    #if defined(USE_SELF_POWER_SENSE_IO)
        tris_self_power = INPUT_PIN;
    #endif

    //Initialise LCD
    lcdInit();
    greeting();

    //Initialize handles
    USBOutHandle = 0;
    USBInHandle = 0;

    //Start USB
    USBDeviceInit();

}

/***********************************************************************
 * Main Method
 ***********************************************************************/
void main(void)
{
    setup();
    while(1)
    {
        USBDeviceTasks();
        ProcessIO();
    }
    
}
