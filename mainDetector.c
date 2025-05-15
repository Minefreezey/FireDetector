#define F_CPU 8000000UL
#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>

void setup(){
    DDRB |= (1 << DDD2);
}

void loop(){
    PORTB |= (1 << PORTB2);
    _delay_ms(1000);
    PORTB &= ~(1 << PORTB2);
    _delay_ms(1000);
}

void main(void) {
    setup();
    while(1){
        loop();
    }
    return;
}
