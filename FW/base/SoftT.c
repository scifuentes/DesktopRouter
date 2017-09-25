#include <timers.h>
#include "SoftT.h"
#include "StepControl.h"

unsigned int st_cnt;
unsigned char st_cnt10;
unsigned int st_cnt500;
unsigned int cycle_cnt, cycle_500;


#define TFlag PIR1bits.TMR2IF
void SoftT_Init(void)
{
	//Timer2 incrementa TMR2 desde 0 hasta que coincide con PR2, luego se resetea a 0 y pone su flag a 1
	//Ref: @48Mhz, PS_1_8 & POST_1_15 & PR2=100 ==> 1ms
	WriteTimer2(0);	
	OpenTimer2( TIMER_INT_OFF & T2_PS_1_16 & T2_POST_1_15);		  
	PR2=50;		// ==> 1ms	
	TFlag=0;

	st_cnt500=0;
	cycle_cnt=0;
}

void SoftT_Tasks(void)
{
	cycle_cnt++;
	if(TFlag==0)
		return;
	TFlag=0;
    
	//---Tasks---
	if(st_cnt500 ==0)	//1000* 1ms => 1Hz
	{
		cycle_500=cycle_cnt;	
		cycle_cnt=0;
        st_cnt500 = 500;
	}
    else
        st_cnt500--;
}
unsigned int GetCyclesCnt(void)
{
	return cycle_500;
} 

