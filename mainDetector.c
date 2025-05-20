#define F_CPU 1000000UL
#include <xc.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "myLCD.h"
#include "myTemperature.h"
#include "myUtil.h"
#include "mySmoke.h"
#include "myBuzzer.h"


char bufferTemp[20];
char bufferSPI[20];
void setup(){
    cls();
    initLCD();
    _delay_ms(20);
    setupPWM();
    _delay_ms(20);
    initADC();
    _delay_ms(20);
    initSPI();
    
    
    LCDDisplayString("Project");
    _delay_ms(1000);
    cls();
    _delay_ms(100);
    sei();
}

void loop(){
    startADC();
    DDRD |= (1 << DDD3);
    startPWM();
    _delay_ms(150);
    if (alert) {
      cls();
      LCDDisplayString("ALERT!!");
      startPWM();  // Enable buzzer
    } else {
      stopPWM();   // Disable buzzer
    }
    PORTB &= ~(1 << PORTB2);
    
    DDRB |= (1 << DDB0);
    if(PIND & (1 << PIND0)){
        cls();
        LCDDisplayString("FIRE!");
        _delay_ms(100);
        DDRD |= (1 << DDD3);
        startPWM();
        
        PORTB |= (1 << PORTB0);
    }
    else{
        DDRD &= ~(1 << DDD3);
        PORTB &= ~(1 << PORTB0);
        stopPWM();
    }

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
    SPSR |= (1 << SPIF); //Clear the complete flag.
    
    PORTB |= (1 << PORTB2);
    
    uint16_t spiValue = (((highByte & 0x1F) << 8) | lowByte)>> 1;
    float temperature = (((spiValue /4095.0) * 5.0) - 0.5) * 100;
  
    myitoa(spiValue, bufferSPI);
    LCDDisplayString("SPI:");
    LCDDisplayString(bufferSPI);
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
