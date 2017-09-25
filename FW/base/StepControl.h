#ifndef _STEPCONTROL_H_
#define _STEPCONTROL_H_


void StepControl_Init(void);
void StepControl_Tasks(void);
void StepControl_Interrupt(void);

void StepControl_SetPeriod(unsigned int period);
unsigned int StepControl_GetPeriod();
void StepControl_SetBasePeriod(unsigned int period);
void StepControl_SetMinPeriod(unsigned int period);
void StepControl_SetAcc(unsigned int acc);

unsigned char StepControl_GetTask(void);
void StepControl_Abort(void);

void StepControl_SetupLine(signed int len_0, signed int len_1, signed int len_2, signed int len_3, unsigned int min_period, unsigned int end_period);
unsigned char StepControl_AddLine(signed int len_0, signed int len_1, signed int len_2, signed int len_3, unsigned int min_period, unsigned int end_period);
char StepControl_SetupManual(char motor, signed char dir);

#endif