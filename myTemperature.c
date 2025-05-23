#include <xc.h>
#include "myTemperature.h"
#include <avr/io.h>

void initSPI(){
    SPCR |= (1 << SPE); //enable the SPI interface
    SPCR |= (1 << MSTR); // declare the AVR as the Master
    
    //use the prescaler of /8 (The MCP3201 can have maximum clock rate of 1.6 MHz))
    SPCR |= (1 << SPR0); 
    SPSR |= (1 << SPI2X);
    DDRB |= (1 << DDB2)| (1 << DDB5);
    PORTB |= (1 << PORTB2);
}
