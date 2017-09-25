#ifndef ANALOGIN_H
#define ANALOGIN_H

void Init_Analog(unsigned char n);
unsigned int AnalogRead(unsigned char n);
unsigned int AnalogAverage(unsigned char ch, unsigned char n);

#endif