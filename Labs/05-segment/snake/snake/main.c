/*
 * snake.c
 *
 * Created: 26/10/2020 20:14:45
 * Author : Enol Ayo Sando
 */ 

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "segment.h"        // Seven-segment display library for AVR-GCC
#include <util/delay.h>


/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Display decimal 
 * counter values on SSD (Seven-segment display) when 16-bit 
 * Timer/Counter1 overflows.
 */

int main(void)
{
    // Configure SSD signals
    SEG_init();
	SEG_update_shift_regs(0,0,0);
	
    /* Configure 16-bit Timer/Counter1 and 8-bit Timer/Counter0
     * Set pre-scaler and enable overflow interrupt */
	TIM1_overflow_262ms();
	TIM1_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
	sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**
 * ISR starts when Timer/Counter1 overflows. Increment decimal counter
 * value and display it on SSD.
 */
ISR(TIMER1_OVF_vect)
{
	static uint8_t snake=0;
	static uint8_t pos=0;
	if(snake<=3){
		pos=0;
	}
	else
	{
		pos=1;
	}
	SEG_snake(snake,pos);
	if(snake>=7){
		snake=0;
	}
	else{
		snake++;
	}
}

