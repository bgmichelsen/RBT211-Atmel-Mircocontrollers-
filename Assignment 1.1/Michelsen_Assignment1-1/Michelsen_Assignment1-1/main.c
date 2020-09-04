/*
 * Michelsen_Assignment1-1.c
 *
 * Created: 9/5/2018 11:13:00 AM
 * Author : brand
 */ 

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    /* Replace with your application code */
	DDRB = 0b00001000;
	
    while (1) 
    {
		PORTB = 0b00001000;
		_delay_ms(2000);
		PORTB = 0b00000000;
		_delay_ms(2000);
    }
	
	return 1;
}

