/*********************************************************************
 *
 *      Program to be used with resident Bootloader
 *
 *********************************************************************
#define OSC_42

/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include "typedefs.h" 
#include "io_cfg.h" 
#include "usb.h" 

#include "SoftT.h"
#include "microstepper.h"
#include "StepControl.h"
#include "TempControl.h"
/** V A R I A B L E S ********************************************************/
#pragma udata

/** P R I V A T E  P R O T O T Y P E S ***************************************/
#pragma code
void HighInt();
void LowInt();
void main_Blink(void);

/** I N I T I A L I T A T I O N ***************************************/
#define any_interruption_enabled (INTCON&0b00111000 || INTCON3&0b00011000 || PIE1 || PIE2)
#define enable_Interrupts(){INTCONbits.GIE=1;	INTCONbits.PEIE=1;}
#pragma code
static void Init(void)
{
	LATB=0;
	LATD=0;
	TRISB=0;
	TRISD=0;

	InitMainLed();
	MainLed=1;

	MicroStepper_Init();
	StepControl_Init();
	
    mInitializeUSBDriver();         // See usbdrv.h

	TempControl_Init();

	Init_Analog(3);
    
	SoftT_Init();
    
    if (any_interruption_enabled)
        enable_Interrupts();
}//end InitializeSystem


/** M A I N **********************************************************/
#pragma code
void main(void)
{
    Init();

    while(1)
    {
        USBTasks();         
        USBRequests();       // See usb.c & .h

		SoftT_Tasks();
		MicroStepper_Tasks(); 
   	
    	main_Blink();
    }//end while

}//end main
void main_Blink(void)
{
    static int i=0;
    if(i++ ==0)
        MainLed=!MainLed;	
}


/** I N T E R R U P T   H A N D L E R S**********************************************/

#pragma interrupt HighInt
void HighInt()
{
	if(INTCONbits.T0IF)
        StepControl_Interrupt();
}

#pragma code
void LowInt()
{
	;
}


/** V E C T O R   R E M A P P I N G *******************************************/

extern void _startup (void);        // See c018i.c in your C18 compiler dir
#pragma code _RESET_INTERRUPT_VECTOR = 0x000800
void _reset (void)
{
    _asm goto _startup _endasm
}
#pragma code

#pragma code _HIGH_INTERRUPT_VECTOR = 0x000808
void _high_ISR (void)
{
	_asm goto HighInt _endasm
}
#pragma code

#pragma code _LOW_INTERRUPT_VECTOR = 0x000818
void _low_ISR (void)
{
	_asm goto LowInt _endasm
}

//--------------------------------------------------
//--------------------------------------------------


/** EOF *************************************************************/
