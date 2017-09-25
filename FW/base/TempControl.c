#include "TempControl.h"
#include "AnalogIn.h"
#include <p18cxxx.h>
#include "auxMath.h"
unsigned int ref_adc[2]={0xffff, 0xffff};
unsigned int last_adc[2];

unsigned char t_pwm_cnt=0;
unsigned char t_pwm_ref[2]={8,8};
#define t_pwm_max 10

#define HOut0 LATCbits.LATC6
#define HOut1 LATCbits.LATC7
#define TrisH0 TRISCbits.TRISC6
#define TrisH1 TRISCbits.TRISC7

void TempControl_Init()
{
	HOut0=0;
	HOut1=0;
	TrisH0=0;
	TrisH1=0;
}

unsigned int TempControl_ReadT(unsigned char ch)
{
    last_adc[ch]=AnalogAverage(ch,4);
   
	return last_adc[ch];
}
void TempControl_Tasks()
{
	unsigned int t_adc;

	/* 100K thermistor, 4K7 divider , 10bit adc, 0--th--!--div--5
	/* aprox...
		th @25ºC 100K => 100/104.7 *1024 = 978
	    th @100ºC 7K  => 7/11.7 *1024 = 612
		th @250ºC 300 => .3/5 *1024 = 61

       test...* T ext, with multimeter
		@21ºC  : 981 adc
        @180ºC : 100
        @195ºC : 80
		@205ºC : 60
		@210ºC : 55
     */    

	t_adc=TempControl_ReadT(0);
	HOut0 = (t_adc>ref_adc[0]) && ProportionalStep(t_pwm_ref[0], t_pwm_max, t_pwm_cnt);

	t_adc=TempControl_ReadT(1);
	HOut1 = (t_adc>ref_adc[1]) && ProportionalStep(t_pwm_ref[1], t_pwm_max, t_pwm_cnt);

	t_pwm_cnt++;
	if (t_pwm_cnt>=t_pwm_max)
		t_pwm_cnt=0;

}

void TempControl_SetRef(unsigned int v_adc,unsigned char ch)
{
	ref_adc[ch]=v_adc;
}
void TempControl_SetPwmRef(unsigned char pwm,unsigned char ch)
{
	t_pwm_ref[ch]=pwm;
}
unsigned int TempControl_GetLastT(unsigned char ch)
{
	return last_adc[ch];
}
unsigned int TempControl_GetRef(unsigned char ch)
{
	return ref_adc[ch];
}
