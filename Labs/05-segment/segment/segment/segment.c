/*
 * segment.c
 *
 * Created: 21/10/2020 9:32:37
 *  Author: Enol Ayo Sando
 */ 

/* Includes ----------------------------------------------------------*/
#define F_CPU  16000000 
#include <util/delay.h>
#include "gpio.h"
#include "segment.h"

/* Variables ---------------------------------------------------------*/
// Active-low digit 0 to 9
uint8_t segment_value[] = {
	// abcdefgDP
	0b00000011,     // Digit 0
	0b10011111,          // Digit 1
	0b00100101,          // Digit 2
	0b00001101,     // Digit 3
	0b10011001,          //  ...
	0b01001001,
	0b01000001,
	0b00011111,
	0b00000001,
	0b00011001
};
uint8_t segment_value_dp[] = {
	// abcdefgDP
	0b00000010,     // Digit 0
	0b10011110,          // Digit 1
	0b00100100,          // Digit 2
	0b00001100,     // Digit 3
	0b10011000,          //  ...
	0b01001000,
	0b01000000,
	0b00011110,
	0b00000000,
	0b00011000
};

// Active-high position 0 to 3
uint8_t segment_position[] = {
	// p3p2p1p0....
	0b00010000,     // Position 0
	0b00100000,     // Position 1
	0b01000000,          // ...
	0b10000000
};

/* Function definitions ----------------------------------------------*/
void SEG_init(void)
{
	/* Configuration of SSD signals */
	GPIO_config_output(&DDRD, SEGMENT_LATCH);
	GPIO_config_output(&DDRD, SEGMENT_CLK);
	GPIO_config_output(&DDRB, SEGMENT_DATA);
}

/*--------------------------------------------------------------------*/
void SEG_update_shift_regs(uint8_t segments, uint8_t position, uint8_t dp)
{
	uint8_t bit_number;
	if(dp==0){
		segments = segment_value[segments];
	}
	else{
		segments = segment_value_dp[segments];
	}
	    
	position = segment_position[position];
	// Pull LATCH, CLK, and DATA low
	GPIO_write_low(&PORTD,SEGMENT_LATCH);
	GPIO_write_low(&PORTD,SEGMENT_CLK);
	GPIO_write_low(&PORTB,SEGMENT_DATA);
	// Wait 1 us
	_delay_us(1);

	// Loop through the 1st byte (segments)
	// a b c d e f g DP (active low values)
	for (bit_number = 0; bit_number < 8; bit_number++)
	{
		// Output DATA value (bit 0 of "segments")
		if((segments % 2)==1){
			GPIO_write_high(&PORTB,SEGMENT_DATA);
		}
		else{
			GPIO_write_low(&PORTB,SEGMENT_DATA);
		}
		// Wait 1 us
		_delay_us(1);

		// Pull CLK high
		GPIO_write_high(&PORTD,SEGMENT_CLK);

		// Wait 1 us
		_delay_us(1);
		// Pull CLK low
		GPIO_write_low(&PORTD,SEGMENT_CLK);
		// Shift "segments"
		segments = segments >> 1;
	}

	// Loop through the 2nd byte (position)
	// p3 p2 p1 p0 . . . . (active high values)
	for (bit_number = 0; bit_number < 8; bit_number++)
	{
		// Output DATA value (bit 0 of "position")
		if((position % 2)==1){
			GPIO_write_high(&PORTB,SEGMENT_DATA);
		}
		else{
			GPIO_write_low(&PORTB,SEGMENT_DATA);
		}
		// Wait 1 us
		_delay_us(1);
		// Pull CLK high
		GPIO_write_high(&PORTD,SEGMENT_CLK);
		// Wait 1 us
		_delay_us(1);
		// Pull CLK low
		GPIO_write_low(&PORTD,SEGMENT_CLK);
		// Shift "position"
		position = position >> 1;
	}

	// Pull LATCH high
	GPIO_write_high(&PORTD,SEGMENT_LATCH);

	// Wait 1 us
	_delay_ms(1);
}

/*--------------------------------------------------------------------*/
/* SEG_clear */

/*--------------------------------------------------------------------*/
/* SEG_clk_2us */