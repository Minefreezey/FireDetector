#include <xc.h>
#include <avr/io.h>
#include <util/delay.h>
#include "myBuzzer.h"

void setupPWM() {
  DDRD |= (1 << DDD3);
  DDRD &= ~(1 << DDD3);
  _delay_ms(100);
  // Set Timer2 to Fast PWM mode, non-inverting on OC2B
  TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
  TCCR2B = (1 << CS21); 

  OCR2B = 32;  
}

void stopPWM() {
  // Disconnect OC2B (stop toggling pin)
  TCCR2A &= ~(1 << COM2B1);

}

void startPWM() {
  // Reconnect OC2B output
  TCCR2A |= (1 << COM2B1);
}
