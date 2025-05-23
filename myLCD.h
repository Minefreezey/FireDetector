// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MYLCD_H
#define	MYLCD_H

#include <xc.h> // include processor files - each processor file is guarded.  

void commitData();
void sendLCDCommand(uint8_t command);
void sendLCDData(uint8_t data);
void LCDDisplayString(const char* str);
void cls();
void initLCD();

#endif	MYLCD_H

