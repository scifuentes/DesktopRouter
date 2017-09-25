#include "StepControl.h"
#include "microstepper.h"
#include "cBuffer.h"
#include "auxMath.h"
#include <timers.h>
#include "debug.h"

//12e6/4==>@3Mhz beat; 6000=>500Hz 10000=>300Hz 12000=>250Hz; 15000=> 200Hz; 20000=>150Hz; 30000=>100Hz; 50000=>66Hz; ; 60000=>50Hz
//12e6/32==>@375Khz beat; 750=>500Hz 1250=>300Hz 1500=>250Hz; 1875=> 200Hz; 2500=>150Hz; 3750=>100Hz; 5000=>75Hz; ; 7500=>50Hz
unsigned int base_periodXY=3750;      //100Hz
unsigned int min_periodXY=1250;       //300Hz
unsigned int base_periodZ=7500;       //50Hz
unsigned int min_periodZ=3750;        //100Hz
unsigned int base_periodManual=2500;  //150Hz

unsigned int base_acc=10;               //in Hz/s

unsigned int task_base_period=2500;  //150Hz
unsigned int task_min_period=1500;   //250Hz
unsigned int task_end_period=2500;   //150Hz
unsigned int task_period=0;  

volatile enum{None=0, Idle=1, Manual, Line} currentTask;

unsigned int line_cnt, line_max, line_0,line_1,line_2,line_3,line_dir;
unsigned int idle_cnt;
signed char manual_dir[nmotors]={0};
//----------------------------------------------------------

void StepControl_LineTask(void);
void StepControl_ManualTask(void);
void StepControl_IdleTask(void);
void StepControl_PeriodControl(unsigned int current_steps,unsigned int steps_to_end);
void StepControl_Interrupt_Septup(void);
//----------------------------------------------------------

void StepControl_Init(void)
{
	currentTask=None;
	idle_cnt=0;
	cBuffer_Init();
    task_period = task_base_period;
    StepControl_Interrupt_Septup();
}

void StepControl_Interrupt_Septup(void)
{
	WriteTimer0(0); //3*500==>2KHz
	OpenTimer0( TIMER_INT_ON &  T0_16BIT  & T0_SOURCE_INT & T0_PS_1_32 );    
}

void StepControl_Interrupt(void)
{
    WriteTimer0(0xffff-task_period); 
    INTCONbits.T0IF=0;
    
    StepControl_Tasks();
}

void StepControl_Tasks(void)
{
    switch (currentTask)
    {
        case None:
            break;
        case Idle:
            StepControl_IdleTask();
            break;
        case Manual:
            StepControl_ManualTask();
            break;
        case Line:
            StepControl_LineTask();
            break;
        default :
            break;
    }
}
unsigned char StepControl_GetTask(void)
{
	return currentTask;
}
void StepControl_Abort(void)
{
    char i;

	currentTask=None;
    cBuffer_Reset();
    task_period=task_base_period;    
    
    for( i=0; i<nmotors; i++)
        manual_dir[i]=0;
    
	Stepper_Stop(-1);
}
void StepControl_SetPeriod(unsigned int period)
{
    if (period>0)
        task_period=period;
}
unsigned int StepControl_GetPeriod()
{
    return task_period;
}
void StepControl_SetBasePeriod(unsigned int period)
{
    if (period>0)
        base_periodXY=period;
}
unsigned int StepControl_GetBasePeriod()
{
    return base_periodXY;
}
void StepControl_SetMinPeriod(unsigned int period)
{
    if (period>0)
        min_periodXY=period;
}
unsigned int StepControl_GetMinPeriod()
{
    return min_periodXY;
}
void StepControl_SetAcc(unsigned int acc)
{
    base_acc=acc;
}
unsigned int StepControl_GetAcc()
{
    return base_acc;
}
//----------
void StepControl_IdleTask()
{
	if(cBuffer_GetNumEntries()>0)
	{
		char buff[20], size;

        idle_cnt=0;
        
		cBuffer_GetFirst(&buff,&size);
		if(buff[0]==Line)
        {
 			StepControl_SetupLine(*((signed int*)&buff[1]),
                                  *((signed int*)&buff[3]),
                                  *((signed int*)&buff[5]),
                                  *((signed int*)&buff[7]),
                                  *((unsigned int*)&buff[9]),
                                  *((unsigned int*)&buff[11]));
        }
	}
    
	idle_cnt++;
	if (idle_cnt>=500)
	{
		currentTask=None;
		Stepper_Stop(-1);
		idle_cnt=0;
	}    
}
unsigned char StepControl_AddLine(  signed int len_0, 
                                    signed int len_1, 
                                    signed int len_2, 
                                    signed int len_3, 
                                    unsigned int min_period, 
                                    unsigned int end_period)
{
    if(cBuffer_GetNumEntries()>16 || cBuffer_GetAvailableSpace()<14)
        return 0;

    cBuffer_Add1(Line);
    cBuffer_Append(&len_0,2);
    cBuffer_Append(&len_1,2);
    cBuffer_Append(&len_2,2);
    cBuffer_Append(&len_3,2);
    cBuffer_Append(&min_period,2);
    cBuffer_Append(&end_period,2);

    if( currentTask==None )
        currentTask=Idle;

    return 1;
}

void StepControl_SetupLine( signed int len_0, 
                            signed int len_1, 
                            signed int len_2, 
                            signed int len_3, 
                            unsigned int min_period, 
                            unsigned int end_period)
{
	line_0=abs(len_0);
	line_1=abs(len_1);
	line_2=abs(len_2);
	line_3=abs(len_3);
	line_max=max(max(max(line_0,line_1),line_2),line_3);
	line_cnt=0;
 
	line_dir=0x00;
	if(len_0<0)
		line_dir|=0x01;
	if(len_1<0)
		line_dir|=0x02;
	if(len_2<0)
		line_dir|=0x04;
	if(len_3<0)
		line_dir|=0x08;

    if(min_period==0)
        min_period=min_periodXY;

    if(end_period==0)
        end_period=base_periodXY;
    
    if(line_2>0 && 5*line_2>=max(line_0,line_1))
    {
        task_min_period = max(min_periodZ, min_period);
        task_base_period = base_periodZ;
        task_end_period = base_periodZ;
    }   
    else
    {
        task_min_period = max(min_periodXY, min_period);
        task_base_period = min(base_periodXY,task_period);
        task_end_period = min(base_periodXY,end_period);
    }
    
    task_period=task_base_period;
    
	currentTask=Line;
}
void StepControl_LineTask()
{
	unsigned char moveIn0 = ProportionalStep(line_0, line_max, line_cnt);
	unsigned char moveIn1 = ProportionalStep(line_1, line_max, line_cnt);
	unsigned char moveIn2 = ProportionalStep(line_2, line_max, line_cnt);
	unsigned char moveIn3 = ProportionalStep(line_3, line_max, line_cnt);
	
	if(moveIn0)
	{
		if((line_dir&0x01)==0)
			Stepper_StepUp(0);
		else
			Stepper_StepDown(0);
	}

	if(moveIn1)
	{
		if((line_dir&0x02)==0)
			Stepper_StepUp(1);
		else
			Stepper_StepDown(1);
	}
    
	if(moveIn2)
	{
		if((line_dir&0x04)==0)
			Stepper_StepUp(2);
		else
			Stepper_StepDown(2);
	}
    
    if(moveIn3)
	{
		if((line_dir&0x08)==0)
			Stepper_StepUp(3);
		else
			Stepper_StepDown(3);
	}
    
	line_cnt++;   
    
    StepControl_PeriodControl(line_cnt,line_max-line_cnt);

	if(line_cnt>=line_max)
    {
		currentTask=Idle;
        task_period=task_end_period;
    }
}

unsigned int StepControl_PeriodAcc()
{
    unsigned int period_delta;
    if( task_period>24000 )
    {
        unsigned int pf = task_period / 10000;
        period_delta = (((base_acc * pf) * pf) * pf);
    }
    else if ( task_period>17000 )
    {
        unsigned int pf = task_period / 1000;
        period_delta = (((base_acc * pf)/10 * pf)/10 * pf)/10;
    }
    else if ( task_period>13000 )
    {
        unsigned int pf = task_period / 250;
        period_delta = (((base_acc * pf)/40 * pf)/40 * pf)/40;
    }
    else
    {
        unsigned int pf = task_period / 100;
        period_delta = (((base_acc * pf)/100 * pf)/100 * pf)/100;
    }
    if( period_delta == 0 && base_acc > 0 )
        period_delta = 1;
    
    return period_delta;
}
void StepControl_PeriodControl(unsigned int current_steps,unsigned int steps_to_end)
{
    if(base_acc>0)
    {
        unsigned int period_delta = StepControl_PeriodAcc();
 
        unsigned int curr_speed=30000/(task_period/(unsigned int)100);
        unsigned int end_speed=30000/(task_end_period/(unsigned int)100);
        unsigned int max_speed=30000/(task_min_period/(unsigned int)100);
        
        unsigned int decel_steps=0;
        if( curr_speed > end_speed )
        {
            decel_steps=((curr_speed/3)*(curr_speed/6)-(end_speed/3)*(end_speed/6))/base_acc;
        }
        
        if( decel_steps >= steps_to_end )
        {
            task_period += period_delta;
        } else if( curr_speed < max_speed )
        {
            task_period -= period_delta;
        }

        /*
        debug[0]=curr_speed;
        debug[1]=end_speed;
        debug[2]=max_speed;
        debug[3]=steps_to_end;
        debug[4]=dcc_steps;
        debug[5]=period_delta;
        */

    }
}
char StepControl_SetupManual(char motor,signed char dir)
{
	if (currentTask>Manual)
		return 0;
		
	manual_dir[motor]=dir;
    task_period=base_periodManual;
	currentTask=Manual;
	return 1;
}
void StepControl_ManualTask()
{
	char i;
	for (i =0;i<nmotors;i++)
	{
		if(manual_dir[i]==1)
			Stepper_StepUp(i);
		if(manual_dir[i]==-1)
			Stepper_StepDown(i);
	}
	
}		


