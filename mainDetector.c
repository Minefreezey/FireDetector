#define F_CPU 1000000UL
#include <xc.h>
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
    initLCD();
    _delay_ms(20);
    initSPI();
    _delay_ms(20);
    cls();
    
    setupPWM();
    _delay_ms(20);
    initADC();
    _delay_ms(20);
    DDRB |= (1 << DDB0);
    
//    Blink LED for 10 times (Starting the program)
    
    for (int i =1;i<=10;i++){
        PORTB ^= (1 << PORTB0);
        _delay_ms(200);
    }
    PORTB &= ~(1 << PORTB0);
    sei();
    //FOR DEBUG
    
//    _delay_ms(100);
//    LCDDisplayString("Project13+");
//    _delay_ms(500);
//    cls();
//    _delay_ms(100);
    
    
}

void loop(){
    stopPWM();
    startADC();
    
    PORTB &= ~(1 << PORTB2);
    
    DDRB |= (1 << DDB0);
    

    SPDR = 0;
    while (!(SPSR & (1 << SPIF))){
    }
    uint8_t highByte = SPDR;
    SPSR |= (1 << SPIF); //Clear the complete flag.
    
    SPDR = 0; 
    while (!(SPSR & (1 << SPIF))){
    }

    uint8_t lowByte = SPDR;
    SPSR |= (1 << SPIF); //Clear the complete flag.
    
    PORTB |= (1 << PORTB2);
    
    uint16_t spiValue = (((highByte & 0x1F) << 8) | lowByte)>> 1;
    
    
//    FOR DEBUG
    
//    myitoa(spiValue, bufferSPI);
//    LCDDisplayString("SPI:");
//    LCDDisplayString(bufferSPI);
//    sendLCDCommand(0xc0);
//    sprintf(bufferTemp, "Temp = %.3f", temperature);
//    LCDDisplayString(bufferTemp);
    
    _delay_ms(200);
    cls();
    _delay_ms(8);
    
    if(PIND & (1 << PIND0)){
        cls();
//        LCDDisplayString("FIRE!");
        DDRD |= (1 << DDD3);
        for (int i =1;i<=3;i++){
            PORTB |= (1 << PORTB0);
            startPWM();
            _delay_ms(40);
            PORTB &= ~(1 << PORTB0);
            stopPWM();
            _delay_ms(40);
        }
        DDRD &= ~(1 << DDD3);
        _delay_ms(25);
    }
    
    if (alert) {
      cls();
//      LCDDisplayString("SMOKE!!");
      PORTB |= (1 << PORTB0);
      DDRD |= (1 << DDD3);
      startPWM();
      _delay_ms(700);
      stopPWM();
      DDRD &= ~(1 << DDD3);
      PORTB &= ~(1<< PORTB0);
      _delay_ms(25);
    } 
    
    if(spiValue >= 950){
        cls();
//        LCDDisplayString("HOT");
        DDRD |= (1 << DDD3);
        for (int i =1;i<=5;i++){
            PORTB |= (1 << PORTB0);
            startPWM();
            _delay_ms(100);
            PORTB &= ~(1 << PORTB0);
            stopPWM();
            _delay_ms(100);
        }
        DDRD &= ~(1 << DDD3);
        _delay_ms(25);
    }

}

void main(void) {
    setup();
    while(1){       
        loop();     
    }
    return;
}
