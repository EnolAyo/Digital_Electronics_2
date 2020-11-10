/*
 * lcd.c
 *
 * Created: 04/11/2020 9:22:31
 * Author : 
 */ 

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Update stopwatch
 * value on LCD display when 8-bit Timer/Counter2 overflows.
 */
int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);
    // Configure 8-bit Timer/Counter2 for Stopwatch
    // Set prescaler and enable overflow interrupt every 16 ms
	TIM2_overflow_interrupt_enable();
	TIM2_overflow_16ms();


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
 * ISR starts when Timer/Counter2 overflows. Update the stopwatch on
 * LCD display every sixth overflow, ie approximately every 100 ms
 * (6 x 16 ms = 100 ms).
 */
/* Interrupt service routines ----------------------------------------*/
/**
 * ISR starts when Timer/Counter2 overflows. Update the stopwatch on
 * LCD display every sixth overflow, ie approximately every 100 ms
 * (6 x 16 ms = 100 ms).
 */
ISR(TIMER2_OVF_vect)
{
    static uint8_t number_of_overflows = 0;
    static uint8_t tens = 0;        // Tenths of a second
    static uint8_t secs = 0;        // Seconds
	static uint8_t mins = 0;		//Minutes
	uint16_t square=0;				//Square value of seconds
    char lcd_string[2] = "  ";      // String for converting numbers by itoa()

    number_of_overflows++;
    if (number_of_overflows >= 6)
    {
        // Do this every 6 x 16 ms = 100 ms
        number_of_overflows = 0;
		//Compute the tens, seconds and minutes
        tens++;
		if(tens>9){
			tens=0;
			secs++;
		}
		if(secs>59){
			mins++;
			secs=0;
		}
		if(mins>59){
			mins=0;
		}
		
		//Display the clock
		
		//Minutes
		itoa(mins, lcd_string, 10);
		if(mins<10){
			lcd_gotoxy(1, 0);
			lcd_putc('0');
			lcd_gotoxy(2, 0);
		}
		else{
			lcd_gotoxy(1, 0);
		}
		lcd_puts(lcd_string);
		lcd_gotoxy(3, 0);
		lcd_putc(':');
		
		//Seconds
		itoa(secs, lcd_string, 10);     // Convert decimal value to string
		if(secs<10){
			lcd_gotoxy(4, 0);
			lcd_putc('0');
			lcd_gotoxy(5, 0);
		}
		else{
			lcd_gotoxy(4, 0);
		}
		lcd_puts(lcd_string);
		lcd_putc('.');
		
		//Tens
		itoa(tens, lcd_string, 10);
		lcd_gotoxy(7, 0);
		lcd_puts(lcd_string);
		
		//Compute and display the square of the seconds
		square=secs*secs;
		if(secs==0){
			lcd_gotoxy(11, 0);
			lcd_puts("0000");
		}
		
		itoa(square, lcd_string, 10);
		if(square<10){
			lcd_gotoxy(14, 0);
		}
		else if(square<100){
			lcd_gotoxy(13, 0);
		}
		else if(square<1000){
			lcd_gotoxy(12, 0);
		}
		else{
			lcd_gotoxy(11, 0);
		}
		
		lcd_puts(lcd_string);
    }
}
