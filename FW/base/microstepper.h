#ifndef __MICROSTEPPER_H_
#define __MICROSTEPPER_H_

#define nmotors 3

void MicroStepper_Init(void);
void MicroStepper_Tasks(void);

void Stepper_Stop(char motor);
void Stepper_StepUp(char motor);
void Stepper_StepDown(char motor);

void Stepper_UntrackedStepUp(char motor);
void Stepper_UntrackedStepDown(char motor);

signed int Stepper_GetPos(char motor);
void Stepper_SetPos(char motor,signed int pos);

unsigned char Stepper_GetStep(char motor);
#endif