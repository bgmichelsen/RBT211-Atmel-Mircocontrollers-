/*
 * Michelsen_Assignment6-1.6.cpp
 *
 * Created: 10/10/2018 10:14:04 PM
 * Author : Brandon Michelsen
 */ 

#define F_CPU	16000000UL // 16 MHz clock

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <m328/serial/USART.h>

// Function prototypes
int analog_read_10(int channel); // Function to read a value from the ADC
int _map(int x, int in_min, int in_max, int out_min, int out_max); // Function to map a number range to another number range

int main(void)
{
	// Inititialize serial communication
	Serial serial_com;
	serial_com.init(BAUD9600);
	
	// Declare local variables
	int val = 0; // Value to send to the computer
	char buffer[8]; // Buffer for sending the number
	
	// Run infinitely
	while (1)
	{
		// Read and map the sensed analog value
		val = _map(analog_read_10(0), 0, 1023, 0, 255);
		
		// Convert to ASCII
		itoa(val, buffer, 10);
		
		// Send to the computer
		serial_com.print(buffer);
		serial_com.transmit('\n');
		
		// Delay
		_delay_ms(50);
	}
}

// Function definitions
int analog_read_10(int channel)
{
	// Set digital I/O to be off on Port C
	DIDR0 = 0x3F;
	
	// Get the proper ADC channel
	if (channel == 0) ADMUX = 0x00; // Channel 0
	if (channel == 1) ADMUX = 0x01; // Channel 1
	if (channel == 2) ADMUX = 0x02; // Channel 2
	if (channel == 3) ADMUX = 0x03; // Channel 3
	if (channel == 4) ADMUX = 0x04; // Channel 4
	if (channel == 5) ADMUX = 0x05; // Channel 5
	
	// Read the ADC signal
	ADCSRA = ((1<<ADEN) | (1<<ADSC) | (1<<ADATE));
	int val = ADCL;
	val += 256*ADCH;
	return val;
}

int _map(int x, int in_min, int in_max, int out_min, int out_max)
{
	// Map the data to an output range
	return (x - in_min)*(((float)(out_max - out_min))/(in_max - in_min)) + out_min;
}

