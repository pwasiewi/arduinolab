#include <Narcoleptic.h>
#include <avr/power.h> 

/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
  Narcoleptic Library is found here: https://code.google.com/p/narcoleptic/downloads/list
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);  
  power_adc_disable();
  power_spi_disable();
  power_usart0_disable();
  power_timer0_disable();
  power_timer1_disable();
  power_timer2_disable();
  power_twi_disable(); 
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  Narcoleptic.delay(2000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  Narcoleptic.delay(2000);               // wait for a second
}
