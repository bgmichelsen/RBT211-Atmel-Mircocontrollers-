/*
 * Michelsen_Assignment6-2.cpp
 *
 * Created: 10/12/2018 4:38:09 PM
 * Author : Brandon Michelsen
 */ 

#define F_CPU	16000000UL // 16 MHz clock
#define OCTAVE	7 // 7 notes in an octave

#include <avr/io.h>
#include <util/delay.h>
#include <m328/serial/USART.h>
#include <m328/tone/tone.h>
#include <m328/tone/Notes.h>

int main(void)
{
	// Setup inputs and outputs
	DDRD |= (1<<DDD6); // Set PD6 as output pin (audio signal)
	
	// Initialize serial
	Serial serial_com;
	serial_com.init(BAUD9600);
	
	/* Declare local variables */
	char input; // Variable for reading in serial data
	char inputs[30]; // Array for reading in data
	int notes_index = 0; // Variable for tracking index of notes array
	int input_index = 0; // Variable for tracking index of input array
	
	// Array for the pitches
	int pitches[OCTAVE] = {
		NOTE_A3, NOTE_B3, NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_G3
	};
	
	// Array used for the common notes
	char notes[OCTAVE] = {
		'A', 'B', 'C', 'D', 'E', 'F', 'G'
	};
	
    // Run infinitely
    while (1) 
    {
		// Reset input index to 0
		input_index = 0;
		do 
		{
			// Receive a character
			input = serial_com.receive();
			
			// If the character is not a delimiter
			if (input != ',' && input != '\n')
			{
				// Add the character to the buffer
				inputs[input_index] = input;
				input_index++;
			}
		} while (input != '\n');
		
		serial_com.print(inputs);
		serial_com.transmit('\n');
		
		// Loop through the buffer, playing each found note
		for (unsigned int i = 0; i < sizeof(inputs); i++)
		{
			// Check to make sure that the input is not 0
			if (inputs[i] == '0')
				// If it is, play no tone for a quarter rest
				tone(0, QUARTER);
			else
			{
				// Find the corresponding note in the sequence
				notes_index = 0;
				while (notes[notes_index] != inputs[i])
				{
					// Increase the index as long as it is not over the array size
					if (notes_index < OCTAVE)
					{
						notes_index++;
					}
					else
					{
						break;
					}
					serial_com.print("Looking for note\n");
				}
			
				// If the note was found, play it
				if (notes[notes_index] == inputs[i])
				{
					tone(pitches[notes_index], QUARTER);
					serial_com.print("Playing note...\n");
				}
			}
		
			tone(0, 0);
			
			// Reset the input at the current index to null
			inputs[i] = 0;
		}
    }
}


