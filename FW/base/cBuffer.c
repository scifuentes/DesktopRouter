#include "cBuffer.h"

#define cBufferSize 251
unsigned char cBuffer[cBufferSize];
unsigned char pos0;	//position in the buffer of the beginning of the first entry: contains the size of the first entry
unsigned char pos1;  //position in the buffer next to the last byte of the last entry: it is the first availabel position
unsigned char pos10;	//position in the buffer of the beginning of the last entry: contains the size of the last entry
unsigned char usedSpace,numEntries;

void cBuffer_Init(void)
{
    cBuffer_Reset();
}

void cBuffer_Reset(void)
{
    pos0=0;
	pos1=0;	
	pos10=0;
	cBuffer[pos0]=0;
	usedSpace=0;
	numEntries=0;
}

char cBuffer_Add(char* data, char size)
{
	unsigned char i;

	if(size+1>cBufferSize)
		return 255;	//entry too big 
	if((int)usedSpace+(int)size+(int)1>=cBufferSize)
		return 0;	//not enough space

	if(usedSpace==0)
	{
		pos0=0;
		pos1=0;	
		pos10=0;
		cBuffer[pos0]=0;
	}
	
	//message storage
	pos10=pos1;
	cBuffer[pos10]=size;
	pos1=(pos1+1)%cBufferSize;
	for(i=0;i<size;i++)
	{
		cBuffer[pos1]=data[i];
		pos1=(pos1+1)%cBufferSize;
	}
	usedSpace+=size+1;
	numEntries++;
	return 1;
}
char cBuffer_Add1(char data)
{
	cBuffer_Add(&data,1);
}
char cBuffer_Append(char* data,char size)
{
	unsigned char i;

	if(size+1>cBufferSize)
		return 255;	//entry too big 
	if((int)usedSpace+(int)size>=cBufferSize)
		return 0;	//not enough space

	for(i=0;i<size;i++)
	{
		cBuffer[pos1]=data[i];
		pos1=(pos1+1)%cBufferSize;
	}
	usedSpace+=size;
	cBuffer[pos10]+=size;

	return 1;
}

char cBuffer_GetFirstSize(void)
{
	return cBuffer[pos0];
}

void cBuffer_GetFirst(char* data, char* size)
{
	unsigned char i,len,posR;

	if(usedSpace==0)
	{
		*size=0;
		return;
	}
	
	len=cBuffer[pos0];
	posR=pos0;
	posR=(posR+1)%cBufferSize;
	for(i=0;i<len;i++)
	{
		data[i]=cBuffer[posR];
		posR=(posR+1)%cBufferSize;
	}
	*size=len;

	cBuffer_RemoveFirst();
  
}

void cBuffer_RemoveFirst()
{
    unsigned char len=cBuffer[pos0]+1;
	pos0=((int)pos0+(int)len)%cBufferSize;
	usedSpace-=len;
	numEntries--;
}

unsigned char cBuffer_GetAvailableSpace(void)
{	
	return cBufferSize-usedSpace;
}
unsigned char cBuffer_GetNumEntries(void)
{	
	return numEntries;
}
