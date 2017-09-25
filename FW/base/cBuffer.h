#ifndef MSGBUFFER_H
#define MSGBUFFER_H

void cBuffer_Init(void);
void cBuffer_Reset(void);

char cBuffer_Add(char* data,char size);
char cBuffer_Add1(char data);
char cBuffer_Append(char* data,char size);

char cBuffer_GetFirstSize(void);
void cBuffer_GetFirst(char* data, char* size);
void cBuffer_CheckFirst(char* data, char pos, char len);
void cBuffer_RemoveFirst(void);

unsigned char cBuffer_GetAvailableSpace(void);
unsigned char cBuffer_GetNumEntries(void);

#endif