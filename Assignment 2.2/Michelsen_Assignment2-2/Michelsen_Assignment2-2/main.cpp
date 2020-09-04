/*
 * Michelsen_Assignment2-2.cpp
 *
 * Created: 9/12/2018 11:26:48 AM
 * Author : Brandon Michelsen
 */ 

#define F_CPU	1000000UL // 1 MHz

#define BLED	(1<<PB1) // Blue LED
#define RLED	(1<<PD5) // Red LED
#define GLED	(1<<PD6) // Green LED

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Function prototypes
void rgb_out(uint8_t r, uint8_t g, uint8_t b); // Function to write to the RGB LED
void color_pattern(char start, char middle, uint8_t value); // Function for controlling the color pattern

// Global variables
volatile int pattern_count = 0; // Variable for tracking the pattern mode

int main(void)
{
	// Set inputs and outputs
	DDRB |= BLED; // Set the blue pin to be an output
	
	DDRD |= (RLED | GLED); // Set the green and red pins to be outputs
	
	// Setup interrupts
	EICRA |= ((1<<ISC00) | (1<<ISC01)); // Trigger on rising edge
	EIMSK |= (1<<INT0); // Enable INT0
	
	// Turn on interrupts
	sei();
	
    // Run infinitely
    while (1) 
    {
		// Make sure the mode doesn't go over the maximum
		if (pattern_count > 3)
			pattern_count = 0;
		
		// Run the pattern
		if (pattern_count == 1)
		{
			// If count is 1, start with red, fade through blue, then green, then start over
			color_pattern('r', 'b', 255);
		}
		else if (pattern_count == 2)
		{
			// If the count is 2, start with blue, fade through red, then green, then start over
			color_pattern('b', 'r', 255);
		}
		else if (pattern_count == 3)
		{
			// If the count is 3, start with green, fade through blue, then red, then start over
			color_pattern('g', 'b', 255);
		}
		else
		{
			rgb_out(0, 0, 0);
		}
	}
}

// Function definitions
void rgb_out(uint8_t r, uint8_t g, uint8_t b)
{
	/* Set PD5 and PD6 PWM Output */
	TCCR0A |= 0xA3; // Set both OC0A and OC0B
	TCCR0B |= 0x05; // Rate = CLK/1024
	OCR0A = g; // Run OCR0A at r
	OCR0B = r; // Run OCR0B at g
	
	/* Set PB1 PWM Output */
	ICR1 = 0x00FF; // Set TOP to 8 bits
	TCCR1A |= 0xA1; // Set up fast PWM
	TCCR1B |= 0x0D; // Rate = CLK/1024
	OCR1A = b; // Run OCR1A at b
}

void color_pattern(char start, char middle, uint8_t value)
{
	// Declare local variables
	uint8_t start_color = value;
	uint8_t middle_color = 0;
	uint8_t end_color = 0;
	
	// First for-loop, decrements start color, increments middle color
	for (int i = 0; i < value; i++)
	{
		start_color--;
		middle_color++;
		
		// Select the proper pattern
		if (start == 'r' && middle == 'g')
			rgb_out(start_color, middle_color, end_color);
		else if (start == 'r' && middle == 'b')
			rgb_out(start_color, end_color, middle_color);
		else if (start == 'g' && middle == 'r')
			rgb_out(middle_color, start_color, end_color);
		else if (start == 'g' && middle == 'b')
			rgb_out(end_color, start_color, middle_color);
		else if (start == 'b' && middle == 'r')
			rgb_out(middle_color, end_color, start_color);
		else
			rgb_out(end_color, middle_color, start_color);
		
		// Delay
		_delay_ms(500);
	}
	
	// Second for-loop, decrement middle color, increment end color
	for (int i = 0; i < value; i++)
	{
		middle_color--;
		end_color++;
		
		// Select the proper pattern
		if (start == 'r' && middle == 'g')
			rgb_out(start_color, middle_color, end_color);
		else if (start == 'r' && middle == 'b')
			rgb_out(start_color, end_color, middle_color);
		else if (start == 'g' && middle == 'r')
			rgb_out(middle_color, start_color, end_color);
		else if (start == 'g' && middle == 'b')
			rgb_out(end_color, start_color, middle_color);
		else if (start == 'b' && middle == 'r')
			rgb_out(middle_color, end_color, start_color);
		else
			rgb_out(end_color, middle_color, start_color);
		
		// Delay
		_delay_ms(500);
	}
	
	// Final for-loop, decrement middle color, increment start color
	for (int i = 0; i < value; i++)
	{
		end_color--;
		start_color++;
		
		// Select the proper pattern
		if (start == 'r' && middle == 'g')
			rgb_out(start_color, middle_color, end_color);
		else if (start == 'r' && middle == 'b')
			rgb_out(start_color, end_color, middle_color);
		else if (start == 'g' && middle == 'r')
			rgb_out(middle_color, start_color, end_color);
		else if (start == 'g' && middle == 'b')
			rgb_out(end_color, start_color, middle_color);
		else if (start == 'b' && middle == 'r')
			rgb_out(middle_color, end_color, start_color);
		else
			rgb_out(end_color, middle_color, start_color);
		
		// Delay
		_delay_ms(500);
	}
}

// Interrupt subroutine
ISR (INT0_vect)
{
	// Delay (debounce)
	// Triggers on rising edge, so no need to check state change
	_delay_ms(500);
	
	// Increment the count
	pattern_count++;
	
	// Jump back to the main program
	asm volatile("jmp main");
}