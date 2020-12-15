
/* Includes ----------------------------------------------------------*/
#include "gpio.h"
#include <avr/delay.h>

/* Function definitions ----------------------------------------------*/

/*************************************************************************
 * Function: code_analyzer()
 * Purpose:  checks if the code introduced matches one of the correct ones
 * Input:    the code introduced and the correct ones
 * Returns:  1 if the code is correct, 0 if not
 **************************************************************************/
uint8_t code_analyzer(char code[4],char correct_code1[4],char correct_code2[4],char correct_code3[4])
{
	
		uint8_t cnt1=0;
		uint8_t cnt2=0;
		uint8_t cnt3=0;
		uint8_t equal=0;
		for (uint8_t i=0;i<4;i++)
		{
			if((code[i]==correct_code1[i])){
				cnt1++;
			}
			
			if((code[i]==correct_code2[i])){
				cnt2++;
			}
			
			if((code[i]==correct_code3[i])){
				cnt3++;
			}
			
		}
		
		if((cnt1==4)|(cnt2==4)|(cnt3==4))//the code is correct if matches one of the correct codes
		{
			equal=1;//CORRECT CODE
		}
		
		return equal;
		
	
}


/*************************************************************************
 * Function: read_digits()
 * Purpose:  scan of the keypad and reading the pressed button
 * Input:    none
 * Returns:  the button pressed or 'X' if none button is pressed
 **************************************************************************/
char read_digits(void){
	
	GPIO_write_low(&PORTC,0);
	GPIO_write_high(&PORTC,1);
	GPIO_write_high(&PORTC,2);
	
	if(GPIO_read(&PINC,3)==0){
		_delay_ms(10);
		while(GPIO_read(&PINC,3)==0);
		return '1';
	}
	if(GPIO_read(&PINC,4)==0){
		_delay_ms(10);
		while(GPIO_read(&PINC,4)==0);
		return '4';
	}
	if(GPIO_read(&PINC,5)==0){
		_delay_ms(10);
		while(GPIO_read(&PINC,5)==0);
		return '7';
	}
	if(GPIO_read(&PINC,6)==0){
		_delay_ms(10);
		while(GPIO_read(&PINC,6)==0);
		return '*';
	}
	
	
	GPIO_write_high(&PORTC,0);
	GPIO_write_low(&PORTC,1);
	GPIO_write_high(&PORTC,2);
	
	if(GPIO_read(&PINC,3)==0){
		_delay_ms(10);
		while(GPIO_read(&PINC,3)==0);
		return '2';
	}
	if(GPIO_read(&PINC,4)==0){
		_delay_ms(10);
		while(GPIO_read(&PINC,4)==0);
		return '5';
	}
	if(GPIO_read(&PINC,5)==0){
		_delay_ms(10);
		while(GPIO_read(&PINC,5)==0);
		return '8';
	}
	if(GPIO_read(&PINC,6)==0){
		_delay_ms(10);
		while(GPIO_read(&PINC,6)==0);
		return '0';
	}
	
	
	GPIO_write_high(&PORTC,0);
	GPIO_write_high(&PORTC,1);
	GPIO_write_low(&PORTC,2);
	
	if(GPIO_read(&PINC,3)==0){
		_delay_ms(10);
		while(GPIO_read(&PINC,3)==0);
		return '3';
	}
	if(GPIO_read(&PINC,4)==0){
		_delay_ms(10);
		while(GPIO_read(&PINC,4)==0);
		return '6';
	}
	if(GPIO_read(&PINC,5)==0){
		_delay_ms(10);
		while(GPIO_read(&PINC,5)==0);
		return '9';
	}
	if(GPIO_read(&PINC,6)==0){
		_delay_ms(10);
		while(GPIO_read(&PINC,6)==0);
		return '#';
	}
	
	return 'X';
	
}