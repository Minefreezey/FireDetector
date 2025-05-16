#define F_CPU 1000000UL
#include <xc.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "myLCD.h"
#include "myTemperature.h"
#include "myUtil.h"

char bufferTemp[20];
char bufferADC[20];
void setup(){
    cls();
    initLCD();
    initSPI();
    DDRB |= (1 << DDD2);
    LCDDisplayString("Project");
    _delay_ms(1000);
    cls();
    _delay_ms(100);
}

void loop(){
    PORTB &= ~(1 << PORTB2);

    SPDR = 0;
    while (!(SPSR & (1 << SPIF))){
    }
    uint8_t highByte = SPDR;
    SPSR |= (1 << SPIF); //Clear the complete flag.
    
    SPDR = 0; 
    while (!(SPSR & (1 << SPIF))){
        //spin-wait polling
    }

    uint8_t lowByte = SPDR;
    myitoa(lowByte, bufferTemp);
    SPSR |= (1 << SPIF); //Clear the complete flag.
    
    PORTB |= (1 << PORTB2);
    
    uint16_t adcValue = (((highByte & 0x1F) << 8) | lowByte)>> 1;
    float temperature = (((adcValue /4095.0) * 5.0) - 0.5) * 100;
   
    myitoa(adcValue, bufferADC);
    LCDDisplayString("ADC = ");
    LCDDisplayString(bufferADC);
    sendLCDCommand(0xc0);
    sprintf(bufferTemp, "Temp = %.3f", temperature);
    LCDDisplayString(bufferTemp);
    
    _delay_ms(200);
    cls();
    _delay_ms(8);
}

void main(void) {
    setup();
    while(1){
        loop();
    }
    return;
}
