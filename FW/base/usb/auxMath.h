#ifndef AUXMATH_H
#define	AUXMATH_H

#define max(x,y) ((x)>=(y)?(x):(y))
#define min(x,y) ((x)>=(y)?(y):(x))
#define abs(x) 	 ((x)>=0?(x):(-(x)))

//Provides 'steps' spreaded following proportion of v to max
unsigned char ProportionalStep(unsigned int v, unsigned int vmax, unsigned int cnt);

#endif	/* AUXMATH_H */

