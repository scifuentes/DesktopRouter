#include "AnalogIn.h"
#include <p18cxxx.h>

/***********************

Add to Init Section: Init_Analog(x);

***********************/

void Init_Analog(unsigned char n)
{
	ADCON0=0b00000001;	//channel 0, ADC idle, ADC off	
	ADCON1=0x0F&(15-n);	//ref:gnd&vcc, n analog channels
	ADCON2=0b10111110;	//RightJustified, 20TAD,Fosc/64,	
}

unsigned int AnalogRead(unsigned char ch)
{
	union{unsigned int full;struct{unsigned char b0,b1;};}data;
	
	ADCON0=1|(ch<<2);
	for (data.b0=0;data.b0<20;data.b0++){;}	//acquisition time

	ADCON0bits.GO=1;
	while(ADCON0bits.GO){;}
	data.b0=ADRESL;
	data.b1=ADRESH;
	return data.full;
}

unsigned int AnalogAverage(unsigned char ch, unsigned char n)
{
    unsigned int adc_read=0;
    unsigned char i;
    for(i=0;i<n;i++)
        adc_read += AnalogRead(ch);
    adc_read=adc_read/n;
    return adc_read;
}