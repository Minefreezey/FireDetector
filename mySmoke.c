#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "mySmoke.h"
#include "myLCD.h"
#include "myUtil.h"

uint16_t adcResult =0;
uint8_t alert =0;
char bufferADC[20];

ISR(ADC_vect){
    adcResult = ADC;
    myitoa(adcResult, bufferADC);
    LCDDisplayString("ADC:");
    LCDDisplayString(bufferADC);
    _delay_ms(200);
    if (adcResult > 200){
        alert = 1;
    }
    else{
        alert = 0;
    }
}


void initADC(){
    //Setup C5 as an input for smoke sensor (For clearity)
    DDRC &= ~(1 << DDC5); 

    // Clear content in ADMUX (For clearity)
    ADMUX &= 0x00;
    ADMUX |= (1 << MUX2) | (1 << MUX0);  // ADC5
    
    // AVcc reference
    ADMUX |= (1 << REFS0);              
    ADMUX &= ~(1 << REFS1);
    
    
    // Set prescaler to 128

    ADCSRA |= (1 << ADEN) | (1 << ADIE) |
              (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}


void startADC() {
  ADCSRA |= (1 << ADSC);  // Start ADC conversion
}
