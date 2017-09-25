/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.0
 *
 *********************************************************************
 * FileName:        user.h
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 2.30.01+
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Rawin Rojvanit       11/19/04     Original.
 ********************************************************************/

#ifndef PICDEM_FS_DEMO_H
#define PICDEM_FS_DEMO_H

/** I N C L U D E S **********************************************************/
#include "typedefs.h"
#include "usb\usbH.h"                         // Required
//#include "usb\usb_compile_time_validation.h" // Optional

/** U S B & P O W E R***********************************************************/
#define usb_bus_sense       1	// Siempre se considera conectado
#define self_power         1	//No esta conectada a alimentacion externa

/** D E F I N I T I O N S ****************************************************/
/* PICDEM FS USB Demo Version */
#define MINOR_VERSION   0x00    //Demo Version 1.00
#define MAJOR_VERSION   0x01


/** S T R U C T U R E S ******************************************************/
typedef union DATA_PACKET
{
    byte _byte[USBGEN_EP_SIZE];  //For byte access
    word _word[USBGEN_EP_SIZE/2];//For word access(USBGEN_EP_SIZE msut be even)
    struct
    {
        enum
        {
            READ_VERSION    = 0x00,
            /*
            READ_FLASH      = 0x01,	//estos son los del bootloader
            WRITE_FLASH     = 0x02,
            ERASE_FLASH     = 0x03,
            READ_EEDATA     = 0x04,
            WRITE_EEDATA    = 0x05,
            READ_CONFIG     = 0x06,
            WRITE_CONFIG    = 0x07,
            */

			READ_REG		= 0x0E,	//14	//lee el contenido de una direccion de registro dada(y lo envia)	
			WRITE_REG		= 0x0F,	//15	//escribe el contenido enviado en una direcciond de registro dada
			WRITE_REG_BIT	= 0x10,	//16	//escribe un bit en la direccion de registro dada

            PIC_PING      	= 0x30,	//48	//comprueba comunicaicones con el pic
						
            RESET           = 0xFF	
        }CMD;
        byte len;
    };
	struct	//Reg_Read/Write	//14R,15W
	{
		unsigned :8;
		word *reg_dir;
		byte reg_value;
		byte reg_bit;
	};
	struct 
	{
		unsigned :8;
		unsigned char uchar[USBGEN_EP_SIZE-1];
	};
	struct 
	{
		unsigned :8;
		unsigned char schar[USBGEN_EP_SIZE-1];
	};
	struct 
	{
		unsigned :8;
		unsigned char uint8[USBGEN_EP_SIZE-1];
	};
	struct 
	{
		unsigned :8;
		signed char sint8[USBGEN_EP_SIZE-1];
	};
	struct 
	{
		unsigned :8;
		unsigned int uint16[(USBGEN_EP_SIZE-1)/2];
	};
	struct 
	{
		unsigned :8;
		signed int sint16[(USBGEN_EP_SIZE-1)/2];
	};
	struct 
	{
		unsigned :8;
		unsigned long uint32[(USBGEN_EP_SIZE-1)/4];
	};
	struct 
	{
		unsigned :8;
		signed long sint32[(USBGEN_EP_SIZE-1)/4];
	};
	struct 
	{
		unsigned :8;
		float float16[(USBGEN_EP_SIZE-1)/4];
	};
	struct 
	{
		unsigned :8;
		double float32[(USBGEN_EP_SIZE-1)/4];
	};
    struct
    {
        unsigned :8;
        unsigned char ch;
        signed int value;
    };

} DATA_PACKET;

/** P U B L I C  P R O T O T Y P E S *****************************************/
void USBRequests(void);
void USBTasks(void);
#endif //PICDEM_FS_DEMO_H
