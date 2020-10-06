/*
 * push_leds.c
 *
 * Created: 05/10/2020 17:59:20
 * Author : Enol Ayo Sando
 */ 

#include <avr/io.h>


/* Defines -----------------------------------------------------------*/
#define LED_GREEN   PB5     // AVR pin where green LED is connected
#define LED_RED		PC0		//AVR pin where red LED is connected
#define BTN			PD3	//AVR pin where the button is connected 
#define BLINK_DELAY 500
#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/sfr_defs.h>

/* Functions ---------------------------------------------------------*/
void shift_led(){
	if(bit_is_clear(PIND,BTN){
		PORTB=PORTB<<1;
	}
	
}
/**
 * Main function where the program execution begins. Toggle two LEDs 
 * when a push button is pressed.
 */
int main(void)
{
    /* GREEN LED */
    // Set pin as output in Data Direction Register...
    DDRB = DDRB | (1<<LED_GREEN);
    // ...and turn LED off in Data Register
    PORTB = PORTB & ~(1<<LED_GREEN);

    /* second LED */
    // WRITE YOUR CODE HERE
	DDRC=DDRC|(1<<LED_RED); //Set pin as output in Data Direction Register
	PORTC=PORTC & ~(1<<LED_RED); //turn on led (It is active low)
	
	/*BUTTON*/
	DDRD=DDRD & (0<<BTN); //Set pin as input in Data Direction Register
	PORTD=PORTD|(1<<BTN); //Enable internal pull-up resistor

    // Infinite loop
    while (1)
    {
        if(bit_is_clear(PIND,BTN)){
			_delay_ms(BLINK_DELAY);
			PORTB=PORTB^(1<<LED_GREEN);
			PORTC=PORTC^(1<<LED_RED);
			
		}
		
        
    }

    // Will never reach this
    return 0;
}

