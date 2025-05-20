#include <xc.h>
#include <util/delay.h>
#include <avr/io.h>
#include "myLCD.h"

void commitData(){
    PORTD |= (1 << PORTD4);
    _delay_us(30);
    PORTD &= ~(1 << PORTD4);
    _delay_us(30);
}

void sendLCDCommand(uint8_t command){
    PORTD &= ~(1 << PORTD2);
    PORTC &= 0xF0;
    PORTC |= command >> 4;
    commitData();
    PORTC &= 0xF0;
    PORTC |= (command & 0x0F);
    commitData();  
}

void sendLCDData(uint8_t data){
    PORTD |= (1 << PORTD2);
    PORTC &= 0xF0;
    PORTC |= data >> 4;
    commitData();
    PORTC &= 0xF0;
    PORTC |= (data & 0x0F);
    commitData();  
}

void LCDDisplayString(const char* str){  
    while(*str){
        _delay_ms(2);
        sendLCDData(*str++);
    }
}

void cls(){
    sendLCDCommand(0x01);
    sendLCDCommand(0x80);
}

void initLCD(){
    DDRC |= 0x0F;
    PORTC &= 0xF0;
    DDRD |= (1 << DDD2) | (1 << DDD4);
    PORTD &= ~(1 << PORTD2) & ~(1 << PORTD4);

    sendLCDCommand(0x33);
    sendLCDCommand(0x32);
    sendLCDCommand(0x28);
    sendLCDCommand(0x0E);
    sendLCDCommand(0x01);
    sendLCDCommand(0x80);
    _delay_ms(100);
}
