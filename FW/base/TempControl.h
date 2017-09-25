#ifndef TEMPCONTROL_H_
#define TEMPCONTROL_H_

void TempControl_Init(void);
unsigned int TempControl_ReadT(unsigned char ch);
void TempControl_Tasks(void);
void TempControl_SetRef(unsigned int v_adc, unsigned char ch);
void TempControl_SetPwmRef(unsigned char pwm, unsigned char ch);
unsigned int TempControl_GetLastT(unsigned char ch);

#endif