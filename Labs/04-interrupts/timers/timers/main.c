/*
 * timers.c
 *
 * Created: 14/10/2020 9:26:02
 * Author : Enol Ayo Sando
 */ 

/* Defines -----------------------------------------------------------*/
#define LED_D1  PB1
#define LED_D2  PB2
#define LED_D3  PB3
#define LED_D4  PB4
#define LED_D5  PB5
#define BTN		PD0

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "gpio.h"           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Toggle three LEDs
 * on Multi-function shield with internal 8- and 16-bit timer modules.
 */
uint8_t cnt=1;//We use a global variable so that we can know the LED on in the interrupt routine
int main(void)
{
    /* Configuration of LEDs */
    GPIO_config_output(&DDRB, LED_D1);
    GPIO_write_high(&PORTB, LED_D1);
	
	GPIO_config_output(&DDRB, LED_D2);
	GPIO_write_low(&PORTB, LED_D2);
	
	GPIO_config_output(&DDRB, LED_D3);
	GPIO_write_low(&PORTB, LED_D3);
	
	GPIO_config_output(&DDRB, LED_D4);
	GPIO_write_low(&PORTB, LED_D4);
	
	GPIO_config_output(&DDRB, LED_D5);
	GPIO_write_low(&PORTB, LED_D5);
	
	/*Configuration of button*/
	GPIO_config_input_pullup(&DDRD,BTN);
	
    

    /* Configuration of 16-bit Timer/Counter1
     * Set prescaler and enable overflow interrupt */
    TIM1_overflow_1s();
    TIM1_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
		
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**
 * ISR starts when Timer/Counter1 overflows.
 * Multi-function shield. */
ISR(TIMER1_OVF_vect){
	if(cnt==5){
		PORTB=2;//Turn on the first LED
		cnt=0;
	}
	else{
		PORTB=PORTB<<1;
		cnt++;
	}
	if(GPIO_read(&PIND,BTN)==0){
		TIM1_overflow_262ms();
	}
	else{
		TIM1_overflow_1s();
	}
   
}

