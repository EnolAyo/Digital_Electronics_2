#include <avr/io.h>
#include "timer.h"
#include "gpio.h"
#include "keypad.h"
#include "lcd.h"
#include "lcd_definitions.h"
#include "uart.h"
#include <util/delay.h> 
#include <avr/interrupt.h> 
#include <stdlib.h>

#define F_CPU	16000000
#define LED_GREEN PD2
#define LED_RED PD3

//Global variables: used by both interrupt routines
uint8_t open=0;//The door is open (1) or closed (0)
uint8_t wait_clear=0;//Indicates if we have to wait until screen cleared

int main(void)
{	//Configure LEDs
	GPIO_config_output(&DDRD,LED_GREEN);
	GPIO_write_low(&PORTD,LED_GREEN); //ACTIVE HIGH
	
	GPIO_config_output(&DDRD,LED_RED);
	GPIO_write_low(&PORTD,LED_RED); //ACTIVE HIGH
	
	//Configure LCD display
	lcd_init(LCD_DISP_ON);
	lcd_gotoxy(1,0);
	lcd_puts("CODE: ");
	
	//Configure keypad ports
	GPIO_config_output(&DDRC,0);
	GPIO_config_output(&DDRC,1);
	GPIO_config_output(&DDRC,2);
	GPIO_config_input_pullup(&DDRC,3);
	GPIO_config_input_pullup(&DDRC,4);
	GPIO_config_input_pullup(&DDRC,5);
	GPIO_config_input_pullup(&DDRC,6);
	
	//Configure relay
	GPIO_config_output(&DDRB,3);
	
	//Timer1 for scanning keypad
	TIM1_overflow_33ms();//scan every 33 ms
	TIM1_overflow_interrupt_enable();
	//Timer0 for closing door
	TIM0_overflow_16ms();
	
	//Set interrupts
	sei();
	
	//Configure UART
	
	uart_init(UART_BAUD_SELECT(9600,F_CPU))	;
	uart_puts("System initialized\r\n");
   
    while (1);//forever loop
    
}
ISR(TIMER1_OVF_vect){
	//Declaration of 3 correct codes
	static char correct_code1[4]={'1','2','3','4'};
	static char correct_code2[4]={'5','6','7','8'};
	static char correct_code3[4]={'6','3','8','7'};
	static uint8_t cnt=0;//Count the number of digits entered
	static uint8_t time_limit_on=0;//Time limit goes on when first digit is pressed
	static uint16_t time_limit_cnt=0;//Count the time to enter the code (16 s approx.)
	static char code[4]={'X','X','X','X'};//Code entered
	char readed=read_digits();//If none button is pressed the output is 'X'
	
	if((readed!='X')&&(wait_clear==0)){//We accept the digit only if the screen is cleared from the previous code
		switch(cnt){
			case 0:
				uart_puts("First digit inserted\r\n");
				break;
			case 1:
				uart_puts("Second digit inserted\r\n");
				break;
			case 2:
				uart_puts("Third digit inserted\r\n");
				break;
			case 3:
				uart_puts("Fourth digit inserted\r\n");
				break;
			default:
				break;
		}
		
		code[cnt]=readed;
		lcd_gotoxy(cnt+7,0);
		lcd_putc(readed);
		cnt++;
		if(cnt==1){
			time_limit_cnt=0;//Set time 
			time_limit_on=1;//Activation of time limit
		}
		if(cnt==4){
			cnt=0;
			uart_puts("The code inserted is: ");
			uart_putc(code[0]);uart_putc(code[1]);uart_putc(code[2]);uart_putc(code[3]);
			uart_puts("\r\n");
			wait_clear=1;//Wait until the screen is cleared
			time_limit_on=0;//Time limit off
			TIM0_overflow_interrupt_enable();//Start counting to close the door
			if(code_analyzer(code,correct_code1,correct_code2,correct_code3)==1){
				uart_puts("The code is correct. Door open");
				open=1;//door open
				GPIO_write_high(&PORTB,3);//open door
				GPIO_write_high(&PORTD,LED_GREEN);
				GPIO_write_low(&PORTD,LED_RED);
				lcd_gotoxy(1,1);
				lcd_puts("CORRECT CODE");
				
				
			}
			else
			{	
				uart_puts("The code is wrong.");
				open=0;//door closed
				lcd_gotoxy(1,1);
				lcd_puts("WRONG CODE");
				GPIO_write_high(&PORTD,LED_RED);
				GPIO_write_low(&PORTD,LED_GREEN);
			}
			uart_puts("\r\n");
		}
	}
	
	time_limit_cnt++;
	if((time_limit_cnt==500)&&(time_limit_on==1)){
		time_limit_cnt=0;
		cnt=0;
		lcd_gotoxy(7,0);
		lcd_puts("    ");
		uart_puts("Time for introducing code exceeded. \r\n");
	}
}
ISR(TIMER0_OVF_vect){
	//Interrupt used to close the relay
	static uint8_t cnt=0;
	if(cnt==188){
		cnt=0;
		if(open==1){
			GPIO_write_low(&PORTB,3);//close door
			GPIO_write_low(&PORTD,LED_GREEN);
			open=0;
			uart_puts("Closing door...\r\n");
		}
		else{//if the door is closed we clear the screen for a new code
			GPIO_write_low(&PORTD,LED_RED);
		}
		TIM0_overflow_interrupt_disable();
		lcd_gotoxy(7,0);
		lcd_puts("    ");//Clear screen
		lcd_gotoxy(1,1);
		lcd_puts("            ");
		wait_clear=0;//the screen is cleared now
			
	}
	cnt++;
	
}

