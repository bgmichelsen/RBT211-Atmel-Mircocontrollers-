/*
 * Michelsen_Assignment5-2.cpp
 *
 * Created: 10/5/2018 2:59:06 PM
 * Author : Brandon Michelsen
 */ 

#define F_CPU	16000000UL // 16 MHz clock

#include <avr/io.h>
#include <util/delay.h>
#include "headers/Notes.h"

// Function prototypes
void tone(int frequency, int duration); // Function to play a tone on Timer 0
void delay(int time); // Function for variable delay
void calc_ocr(int frequency, int* audio_ocr, int* prescaler_val); // Function to calculate the OCRnX value based on a given frequency
bool debounce(bool last); // Function for debouncing PD7 pin

int main(void)
{
	// Setup inputs and outputs
	DDRD |= (1<<DDD6); // Set PD6 as an output (tone pin)
	DDRD &= ~((1<<DDD0) | (1<<DDD1) | (1<<DDD2) | (1<<DDD3) | (1<<DDD4) | (1<<DDD7)); // Set input button pins
	
	// Declare local variables
	bool current_state = 0; // Variable for the current state of the button
	bool last_state = 0; // Variable for the last state of the button
	bool key_played = false; // Variable for tracking if a key is pressed in lowest octave mode
	int octave_count = 0; // Variable for tracking what octave we are on
	
	// Run infinitely
	while (1)
	{
		/* Play music using the keyboard */
		
		// Short delay to ensure there is no noise
		//_delay_ms(5);
		
		// Check if the button has been toggled
		current_state = debounce(last_state);
		if (current_state == 1 && last_state == 0)
		{
			// While the OCTAVE button is being held down, step all the notes down to the lowest available octave (octave 1)
			while (PIND & (1<<PIND7))
			{
				while (PIND & (1<<PIND0))
				{
					tone(NOTE_C1, 0);
					key_played = true;
				}
				while (PIND & (1<<PIND1))
				{
					tone(NOTE_D1, 0);
					key_played = true;
				}
				while (PIND & (1<<PIND2))
				{
					tone(NOTE_E1, 0);
					key_played = true;
				}
				while (PIND & (1<<PIND3))
				{
					tone(NOTE_G1, 0);
					key_played = true;
				}
				while (PIND & (1<<PIND4))
				{
					tone(NOTE_A1, 0);
					key_played = true;
				}
				tone(0, 0);
			}
			// If no button was pressed in lowest-octave mode, increase the octave count
			// Otherwise, reset the key_pressed flag
			if (key_played == true)
				key_played = false;
			else
				octave_count++;
		}
		last_state = current_state;
		
		// If the octave count is above 5, reset it
		if (octave_count > 3)
			octave_count = 0;
			
		// Check what the octave count is at and play the corresponding octave
		switch (octave_count)
		{
			case 0:
			{
				// If the octave count is 0 (default), play on octave 2
				while (PIND & (1<<PIND0))
					tone(NOTE_C2, 0);
				while (PIND & (1<<PIND1))
					tone(NOTE_D2, 0);
				while (PIND & (1<<PIND2))
					tone(NOTE_E2, 0);
				while (PIND & (1<<PIND3))
					tone(NOTE_G2, 0);
				while (PIND & (1<<PIND4))
					tone(NOTE_A2, 0);
				tone(0, 0);
				break;
			}
			case 1:
			{
				// If the octave count is 1, play on octave 3
				while (PIND & (1<<PIND0))
					tone(NOTE_C3, 0);
				while (PIND & (1<<PIND1))
					tone(NOTE_D3, 0);
				while (PIND & (1<<PIND2))
					tone(NOTE_E3, 0);
				while (PIND & (1<<PIND3))
					tone(NOTE_G3, 0);
				while (PIND & (1<<PIND4))
					tone(NOTE_A3, 0);
				tone(0, 0);
				break;
			}
			case 2:
			{
				// If the octave count is at 2, play on octave 4
				while (PIND & (1<<PIND0))
					tone(NOTE_C4, 0);
				while (PIND & (1<<PIND1))
					tone(NOTE_D4, 0);
				while (PIND & (1<<PIND2))
					tone(NOTE_E4, 0);
				while (PIND & (1<<PIND3))
					tone(NOTE_G4, 0);
				while (PIND & (1<<PIND4))
					tone(NOTE_A4, 0);
				tone(0, 0);
				break;
			}
			case 3:
			{
				// If the octave is at 3, play on octave 5
				while (PIND & (1<<PIND0))
					tone(NOTE_C5, 0);
				while (PIND & (1<<PIND1))
					tone(NOTE_D5, 0);
				while (PIND & (1<<PIND2))
					tone(NOTE_E5, 0);
				while (PIND & (1<<PIND3))
					tone(NOTE_G5, 0);
				while (PIND & (1<<PIND4))
					tone(NOTE_A5, 0);
				tone(0, 0);
				break;
			}
		}
	}
}


// Function definitions
void tone(int frequency, int duration)
{
	// Declare local variables
	int out_val = 0; // Value to write to the OCR0A register
	int prescale = 0; // Prescaler value
	
	// Calculate the output value for the desired frequency
	calc_ocr(frequency, &out_val, &prescale);
	
	// Setup CTC mode on timer 0
	TCCR0A = ((1<<COM0A0) | (1<<WGM01));
	
	// Set the prescaler
	if (prescale == 1)
	TCCR0B = (1<<CS00); // Prescale of 1
	else if (prescale == 8)
	TCCR0B = (1<<CS01); // Prescale of 8
	else if (prescale == 64)
	TCCR0B = ((1<<CS01) | (1<<CS00)); // Prescale of 64
	else if (prescale == 256)
	TCCR0B = (1<<CS02); // Prescale of 256
	else
	TCCR0B = ((1<<CS02) | (1<<CS00)); // Prescale of 1024
	
	// Set the output frequency of the pin
	OCR0A = out_val;
	
	// Delay for the duration
	delay(duration);
}

void calc_ocr(int frequency, int* audio_ocr, int* prescale_val)
{
	// Declare local variables
	int ocr = 0; // Value for calculating OCRnX value
	int prescalers[6] = {1, 8, 64, 256, 1024, 0}; // List of prescale values (N)
	uint8_t i = 0; // Index value for the prescalers list
	
	// If the frequency is 0, don't run the calculation
	if (frequency == 0)
	{
		ocr = 0;
	}
	else
	{
		// Run through all the prescale values
		do
		{
			// Calculate the value for OCRnX to achieve the desired frequency
			ocr = (F_CPU/prescalers[i]);
			ocr = (ocr/frequency);
			++i;
		} while ((ocr > 255) && (prescalers[i]));
	}
	
	// If we exhausted the list of prescalers, set it back by 1
	if (prescalers[i] == 0)
	i--;
	
	// If the value is greater than 255, cap at 255
	if (ocr > 255)
	ocr = 255;
	
	// Return the values
	*audio_ocr = ocr;
	*prescale_val = prescalers[i];
}

void delay(int time)
{
	// Loop until desired time is reached
	int i = 1;
	while (i <= time)
	{
		_delay_ms(1);
		i++;
	}
}

bool debounce(bool last)
{
	// Read the current state of the button
	bool current = (PIND & (1<<PIND7));
	
	// If the button changed state, re-read the button after a short delay (gets rid of bounce)
	if (current != last)
	{
		_delay_ms(5);
		
		current = (PIND & (1<<PIND7));
	}
	
	return current;
}
