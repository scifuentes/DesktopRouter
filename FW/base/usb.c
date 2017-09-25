/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include "typedefs.h"	// Required

#include "usb\usbH.h"
#include "io_cfg.h"             // I/O pin mapping
#include "usb.h"			//dataPacket Enums

#include "debug.h"
#include "SoftT.h"
#include "microstepper.h"
#include "StepControl.h"
#include "TempControl.h"
#include "AnalogIn.h"

/** V A R I A B L E S ********************************************************/
#pragma udata
byte outSize;
DATA_PACKET msg;


/** P R I V A T E  P R O T O T Y P E S ***************************************/

/** I N I C I A L I Z A T I O N  **************************************************/

//** F U N C T I O N S **********************************************

// USBTasks --> Mantenimiento de rutina para el usb 
#pragma code
void USBTasks(void)
{
    /*
     * Servicing Hardware
     */
    USBCheckBusStatus();                    // Must use polling method
    if(UCFGbits.UTEYE!=1)
        USBDriverService();                 // Interrupt or polling method

}// end USBTasks


// USBRequests --> Funcion de atencion a las peticiones llegadas por Usb
// 					Aqui es donde se gestionan las comunicaciones del usuario, 
//					añadiendo nuevos casos.

#pragma code
void USBRequests(void)
{
	byte index;
   	unsigned char i;
	unsigned char tmp;

	//Usb state check
    if((usb_device_state < CONFIGURED_STATE)||(UCONbits.SUSPND==1)) 
		return;
 

	//Usb input handle
    if(USBGenRead((byte*)&msg,sizeof(msg)))
    {

        outSize = 0;
        switch(msg.CMD) //_byte[0]
        {
            case READ_VERSION:
                //msg._byte[1] is len
                msg._byte[2] = MINOR_VERSION;
                msg._byte[3] = MAJOR_VERSION;
                outSize=0x04;
               break;
			
			case READ_REG:
				msg.reg_value=*msg.reg_dir;
				outSize=0x04;
				break;
				
			case WRITE_REG:
				(*msg.reg_dir)=msg.reg_value;
				break;
			
			case WRITE_REG_BIT:
				tmp=1<<msg.reg_bit;	//pone un 1 en el bit correspondiente
				if(msg.reg_value==0)
					(*msg.reg_dir)&=~tmp;	//graba el 1 como 0
				else
					(*msg.reg_dir)|=tmp;		//graba el 1
				break;

			case PIC_PING:	
				tmp=msg.uint8[1];
				msg.uint8[1]=msg.uint8[0];
				msg.uint8[0]=tmp;
				outSize=0x03;
				break;

            case RESET:
                Reset();
                break;

            case 254: //DEBUG
                for(i=0; i<DEBUGLEN; i++)
                    msg.uint16[i] = debug[i];
                outSize=1+2*DEBUGLEN;
                break;

		//---  P R O G R A M   S P E C I F I C   C O D E S ---------------------

            case 250:
                msg.uint16[0]=GetCyclesCnt();
                outSize=3;
                break;

            case 60:
                msg.uint16[0]= AnalogRead(msg.ch);
                outSize=3;
                break;
            case 61:
                msg.uint16[0]= AnalogAverage(msg.ch,msg.uchar[1]);
                outSize=3;
                break;

            case 80:
                break;

            case 81:
                Stepper_Stop(msg.uchar[0]);
                break;

            case 82:
                Stepper_StepUp(msg.uchar[0]);
                break;

            case 83:
                Stepper_StepDown(msg.uchar[0]);
                break;

            case 84:
                Stepper_UntrackedStepUp(msg.uchar[0]);
                break;

            case 85:
                Stepper_UntrackedStepDown(msg.uchar[0]);
                break;	

            case 87:	
                Stepper_SetPos(msg.ch,msg.value);
                break;	

            case 88:	
                msg.sint16[0]=Stepper_GetPos(msg.uchar[0]);
                outSize=3;
                break;

            case 89:
                msg.uchar[0]=Stepper_GetStep(msg.uchar[0]);
                outSize=2;
                break;


            case 100:
                msg.uchar[0]=StepControl_GetTask();
                outSize=2;
                break;

            case 101:
                StepControl_Abort();
                break;

            case 102:
                msg.uchar[0]=StepControl_AddLine(msg.sint16[0],msg.sint16[1],msg.sint16[2],msg.sint16[3],msg.uint16[4],msg.uint16[5]);
                outSize=2;
                break;

            case 103:
                msg.uchar[0]=StepControl_SetupManual(msg.uchar[0],msg.schar[1]);
                outSize=2;
                break;

            case 104:
                StepControl_SetPeriod(msg.uint16[0]);
                break;        

            case 105:
                msg.uint16[0]=StepControl_GetPeriod();
                outSize=3;
                break;

            case 106:
                StepControl_SetBasePeriod(msg.uint16[0]);
                break;        

            case 107:
                StepControl_SetMinPeriod(msg.uint16[0]);
                break;        

            case 108:
                StepControl_SetAcc(msg.uint16[0]);
                break;        

            case 109:
               msg.uint16[0] = StepControl_GetBasePeriod();
               msg.uint16[1] = StepControl_GetMinPeriod();
               msg.uint16[2] = StepControl_GetAcc();
               outSize=7;   
               break;

            case 110:
                msg.uint16[0]= TempControl_GetLastT(msg.uint8[0]);
                outSize=3;
                break;

            case 111:
                TempControl_SetRef( msg.value, msg.ch);
                break;

            case 112:
                TempControl_SetPwmRef(msg.uint8[1],msg.ch);
                break;

            case 113:
                msg.uint16[0]= TempControl_GetRef(msg.uint8[0]);
                outSize=3;
                break;

            default:
                break;
        }//end switch()

        
		if(outSize != 0)
        {
			if(!mUSBGenTxIsBusy())
                USBGenWrite((byte*)&msg,outSize);
        }//end if
        
    
	}//end if

}//end UsbRequests

/** EOF usb.c ***************************************************************/
