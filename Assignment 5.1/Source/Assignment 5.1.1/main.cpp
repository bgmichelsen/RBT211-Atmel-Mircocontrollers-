/*
 * Michelsen_Assignment5-1.1.cpp
 *
 * Created: 10/3/2018 11:50:31 AM
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

int main(void)
{
	// Setup inputs and outputs
	DDRD |= (1<<DDD6);
	
	/* Declare local variables */
	// Array for notes (plays "Mary Had a Little Lamb")
	int notes[26] = {
		NOTE_E3, NOTE_D3, NOTE_C3, NOTE_D3,
		NOTE_E3, NOTE_E3, NOTE_E3, NOTE_D3,
		NOTE_D3, NOTE_D3, NOTE_E3, NOTE_G3,
		NOTE_G3, NOTE_E3, NOTE_D3, NOTE_C3,
		NOTE_D3, NOTE_E3, NOTE_E3, NOTE_E3,
		NOTE_E3, NOTE_D3, NOTE_D3, NOTE_E3,
		NOTE_D3, NOTE_C3
	};
	
	// Array for note durations ("Mary Had a Little Lamb")
	int durations[26] = {
		D_QUARTER, EIGHTH, QUARTER, QUARTER,
		QUARTER, QUARTER, HALF, QUARTER,
		QUARTER, HALF, QUARTER, QUARTER,
		HALF, D_QUARTER, EIGHTH, QUARTER,
		QUARTER, QUARTER, QUARTER, QUARTER,
		QUARTER, QUARTER, QUARTER, QUARTER,
		QUARTER, WHOLE 		
	};
	
	// C Major Scale
	/*int notes[8] = {
		NOTE_C4, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_A3, NOTE_B3, NOTE_C4
	};
	
	int durations[8] = {
		QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER, QUARTER
	};*/
	
    // Run infinitely
    while (1) 
    {
		// Play the song
		for (int i = 0; i < 26; i++)
		{
			tone(notes[i], durations[i]); // Play the music
			tone(0, 1); // Delay
		}
		// Delay
		_delay_ms(1000);
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