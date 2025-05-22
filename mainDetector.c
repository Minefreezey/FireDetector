#define F_CPU 1000000UL
#include <xc.h>
//#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "myLCD.h"
#include "myTemperature.h"
#include "myUtil.h"
#include "mySmoke.h"
#include "myBuzzer.h"

#define HOT 0
#define SMOKE 1
#define FIRE 2

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
void UART_init()
{
    UCSR0A |= (1 << U2X0);
    UBRR0H = (unsigned char)(BAUD_PRESCALE >> 8);
    UBRR0L = (unsigned char)BAUD_PRESCALE;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void USART_Transmit(unsigned char data)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void USART_SendString(const char *str){
    while (str)
    {
        USART_Transmit(*str++);
    }
    USART_Transmit('\n');
}

uint8_t globalFire = 0;

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
    
    for (int i =1;i<=10;i++){
        PORTB ^= (1 << PORTB0);
        _delay_ms(200);
    }
    PORTB &= ~(1 << PORTB0);
    
    _delay_ms(100);
    LCDDisplayString("Project13+");
    _delay_ms(500);
    cls();
    _delay_ms(100);
    sei();
}

void loop(){
    stopPWM();
    startADC();
    
    PORTB &= ~(1 << PORTB2);
    
    DDRB |= (1 << DDB0);
    

    SPDR = 0;
    while (!(SPSR & (1 << SPIF))){
//        LCDDisplayString("Wait");
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
//    spiValue -= 3200;
    float temperature = (((spiValue /4095.0) * 3.63) - 0.5) * 100;
    
    
    myitoa(spiValue, bufferSPI);
//    LCDDisplayString("SPI:");
//    LCDDisplayString(bufferSPI);
    sendLCDCommand(0xc0);
//    sprintf(bufferTemp, "Temp = %.3f", temperature);
//    USART_SendString(bufferSPI);
//    LCDDisplayString(bufferTemp);
    
    _delay_ms(200);
    cls();
    _delay_ms(8);
    
    if(PIND & (1 << PIND0)){
        cls();
        LCDDisplayString("FIRE!");
        globalFire |= (1 << FIRE);
    }
    else{
        globalFire &= ~(1 << FIRE);
    }
    
    if (alert) {
      cls();
      LCDDisplayString("SMOKE!!");
      globalFire |= (1 << SMOKE);
    } else {
        globalFire &= ~(1 << SMOKE);
    }
    
    if(spiValue >= 900){
        cls();
        LCDDisplayString("HOT");
        globalFire |= (1 << HOT);
//        _delay_ms(250);
    }
    else{
        globalFire &= ~(1 << HOT);
    }
    
    if (globalFire & ((1 << FIRE) | (1 << SMOKE) | (1 << HOT))){
        PORTB |= (1 << PORTB0); 
        DDRD |= (1 << DDD3);
        startPWM();
        _delay_ms(500);
        DDRD &= ~(1 << DDD3);
        _delay_ms(50);
    }
    else{
        PORTB &= ~(1 << PORTB0);
        stopPWM();
    }
    
    
}

void main(void) {
    setup();
//    DDRB |= (1 << PORTB0);
//    DDRD |= (1 << DDD3);
//    sei();
//    setupPWM();
//    initADC();
    while(1){       
        loop();     
        
    }
    return;
}
