
#ifndef MYSMOKE_H
#define	MYSMOKE_H

#include <xc.h> // include processor files - each processor file is guarded.  

extern uint16_t adcResult;
extern uint8_t alert;

void initADC();
void startADC();

#endif	MYSMOKE_H

