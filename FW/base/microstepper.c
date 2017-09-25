#include <p18cxxx.h>
#include "microstepper.h"

#define ms_nsteps 4
#if ms_nsteps == 4
unsigned char seqV[ms_nsteps]={0b00000001,
                               0b00000100,
                               0b00000010,
                               0b00001000};
#elif ms_nsteps == 8
unsigned char seqV[ms_nsteps]={0b00000001,
                               0b00000101,
                               0b00000100,
                               0b00000110,
                               0b00000010,
                               0b00001010,
                               0b00001000,
                               0b00001001};
#endif


char ms_maxcnt[nmotors]={4,4,3};
char ms_cnt[nmotors]={0};

signed char step_cnt[nmotors]={0};
unsigned char stepV[nmotors]={0};

signed int pos_cnt[nmotors]={0};
///--------------------------------------------------------

void MicroStepper_SetStepV(char step,char motor);
///--------------------------------------------------------

void MicroStepper_Init(void)
{
	LATB=0;
	PORTB=0;
	TRISB=0;
	LATD=0;
	PORTD=0;
	TRISD=0;
	Stepper_Stop(-1);
}

void MicroStepper_Tasks(void)
{
#if nmotors>=1
    LATB =0;
    if(ms_cnt[0]==0)
    {
        LATB |= stepV[0];
        ms_cnt[0] = ms_maxcnt[0];
    }
    else
        ms_cnt[0]--;
#endif

#if nmotors>=2
    if(ms_cnt[1]==0)
    {
        LATB |= stepV[1];
        ms_cnt[1] = ms_maxcnt[1];
    }
    else
        ms_cnt[1]--;
#endif      

#if nmotors>=3
    LATD &= 0xF0;
    if(ms_cnt[2]==0)
    {
        LATD |= stepV[2];
        ms_cnt[2] = ms_maxcnt[2];
    }
    else
        ms_cnt[2]--;
#endif      

#if nmotors>=4
    if(ms_cnt[3]==0)
    {
        LATD |= stepV[3];
        ms_cnt[3] = ms_maxcnt[3];
    }
    else
        ms_cnt[3]--;
#endif      

}

void _Stepper_Stop(char motor)
{
	if (motor>=nmotors)
		return;

	stepV[motor]=0;
}
void Stepper_Stop(char motor)
{
	int i;
	if(motor==-1)
		for(i=0;i<nmotors;i++)
			_Stepper_Stop(i);
	else
		_Stepper_Stop(motor);
}
void Stepper_StepUp(char motor)
{
	if (motor>=nmotors)
		return;

    Stepper_UntrackedStepUp(motor);
	pos_cnt[motor]++;
}

void Stepper_UntrackedStepUp(char motor)
{
	step_cnt[motor]++;
	if(step_cnt[motor]>=ms_nsteps)
		step_cnt[motor]=0;

	MicroStepper_SetStepV(step_cnt[motor],motor);   
}
void Stepper_StepDown(char motor)
{
	if (motor>=nmotors)
		return;

    Stepper_UntrackedStepDown(motor);
            
	pos_cnt[motor]--;
}

void Stepper_UntrackedStepDown(char motor)
{
	step_cnt[motor]--;
	if(step_cnt[motor]<0)
		step_cnt[motor]=ms_nsteps-1;

	MicroStepper_SetStepV(step_cnt[motor],motor);   
}

void MicroStepper_SetStepV(char step,char motor)
{
    if( motor%2 == 0)
        stepV[motor]=seqV[step];
    else
        stepV[motor]=seqV[step]<<4;
}



signed int Stepper_GetPos(char motor)
{
	return pos_cnt[motor];
}
void Stepper_SetPos(char motor,signed int pos)
{
	pos_cnt[motor]=pos;
}

//---- debug aux ----
unsigned char Stepper_GetStep(char motor)
{
	return step_cnt[motor];
}
