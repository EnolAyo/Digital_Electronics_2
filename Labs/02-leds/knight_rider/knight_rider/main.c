/*
 * knight_rider.c
 *
 * Created: 06/10/2020 18:26:18
 * Author : Enol Ayo Sando
 */ 
/* Defines -----------------------------------------------------------*/
#define LED_0		PB0     // AVR pin where green LED is connected
#define LED_1		PB1		//AVR pin where red LED is connected
#define LED_2		PB2		//AVR pin where red LED is connected
#define LED_3		PB3		//AVR pin where red LED is connected
#define LED_4		PB4		//AVR pin where red LED is connected
#define BTN			PD3		//AVR pin where the button is connected
#define BLINK_DELAY 500
#define F_CPU 16000000      // CPU frequency in Hz required for delay
/* Includes ----------------------------------------------------------*/
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>

void shift_led(){
	
		PORTB=PORTB<<1;
		_delay_ms(BLINK_DELAY);
	
}

int main(void)
{	//LEDS
	DDRB=DDRB|31; //Set five LED ports as output
	PORTB=PORTB & ~(1<<LED_4);// Turn off all the LEDs
	//Push button
	DDRD=DDRD & (0<<BTN); //Set pin as input in Data Direction Register
	PORTD=PORTD|(1<<BTN); //Enable internal pull-up resistor
	
	while (1) {
		if(bit_is_clear(PIND,BTN)){
			PORTB=1;
			_delay_ms(BLINK_DELAY);
			for(int i=1;i<=4;i++){
				shift_led();
			}
		}
    }
}

