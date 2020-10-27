/*
 * segment.c
 *
 * Created: 21/10/2020 9:29:13
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
uint8_t units_0=0;//Second units
uint8_t tens_0=0;//Second tens
uint8_t units_1=0;//Minutes units
uint8_t tens_1=0;//Minutes tens

int main(void)
{
    // Configure SSD signals
    SEG_init();
	SEG_update_shift_regs(0,0,0);
	
    /* Configure 16-bit Timer/Counter1 and 8-bit Timer/Counter0
     * Set pre-scaler and enable overflow interrupt */
	TIM1_overflow_1s();
	TIM1_overflow_interrupt_enable();
	TIM0_overflow_4ms();
	TIM0_overflow_interrupt_enable();

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
	units_0++;
	if(units_0>9)
	{
		units_0=0;
		tens_0++;
	}
	if(tens_0>5)
	{
		units_1++;
		units_0=0;
		tens_0=0;
	}
	if(units_1>9)
	{
		tens_1++;
		units_1=0;
	}
	if(tens_1>5)
	{
		tens_1=0;
		units_1=0;
		units_0=0;
		tens_0=0;
	}
}
ISR(TIMER0_OVF_vect)
{
	static uint8_t pos=0;
	if(pos==0){
		SEG_update_shift_regs(units_0,pos,0);
		
	}
	else if(pos==1)
	{
		SEG_update_shift_regs(tens_0,pos,0);
		
	}
	else if(pos==2)
	{
		SEG_update_shift_regs(units_1,pos,1);
	}
	else
	{
		SEG_update_shift_regs(tens_1,pos,0);
	}
	pos++;
	if(pos==4) pos=0;
}
