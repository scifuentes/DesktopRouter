#include "auxMath.h"

unsigned char ProportionalStep(unsigned int v, unsigned int vmax, unsigned int cnt)
{
    return  ( (((cnt+1)*(long unsigned int)v)/vmax) != ((cnt*(long unsigned int)v)/vmax) );
}