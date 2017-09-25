#ifndef IO_CFG_H
#define IO_CFG_H

/** IO Pins definitions for easy reading*****************************/

/** L E D ***********************************************************/
#define InitMainLed() TRISCbits.TRISC2=0
#define MainLed PORTCbits.RC2


//==================================================
#endif //IO_CFG_H
