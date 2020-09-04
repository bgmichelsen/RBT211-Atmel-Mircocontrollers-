/*
 * Michelsen_Assignment4-1.5.cpp
 *
 * Created: 9/28/2018 2:28 PM
 * Author : Brandon Michelsen
 */ 

#define F_CPU	16000000UL // 16 MHz clock

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "Headers/USART.h"

// Function prototypes
int read_distance(int angle); // Function to read the distance at different angles
void servo_move(int angle); // Function to move a servo motor
void write_leds(int sig1, int sig2, int sig3, int sig4); // Function to write values to the PWM LEDs
int analog_read_10(int channel); // Function to read an analog value
int _map(int x, int in_min, int in_max, int out_min, int out_max); // Function to map one number range to another
int linearize(int val); // Function to linearize the data

// Create an instance of the Serial class
Serial serial;

int main(void)
{
	// Setup inputs and outputs
	DDRB |= ((1<<DDB1) | (1<<DDB3));
	DDRD |= ((1<<DDD3) | (1<<DDD5) | (1<<DDD6));
	
	// Declare local variables
	int quadrants[4] = { 0, 0, 0, 0 }; // Array for the LEDs representing the 4 quadrants
	int measurement = 0; // Variable for reading the measurement values
	
	// Create and initialize UART
	serial.init(BAUD9600); // 9600 baud
	
    // Run infinitely
    while (1) 
    {
		for (int i = 0; i <= 180; i += 15)
		{
			// Read distance at angle at every 15 degrees
			measurement = read_distance(i);
			
			// Get the proper quadrant to light the corresponding LED
			if (i <= 45)
				// Quadrant 1
				quadrants[0] = measurement;
			else if (i > 45 && i <= 90)
				// Quadrant 2
				quadrants[1] = measurement;
			else if (i > 90 && i <= 135)
				// Quadrant 3
				quadrants[2] = measurement;
			else if (i > 135 && i <= 180)
				// Quadrant 4
				quadrants[3] = measurement;
			
			// Light the LEDs		
			write_leds(quadrants[0], quadrants[1], quadrants[2], quadrants[3]);
			_delay_ms(100);
			
			serial.transmit('\n');
		}
    }
}

// Function definitions
int read_distance(int angle)
{
	// Buffer for reading in numerical data
	char buffer[16];
	
	// Move the servo to the desired angle
	servo_move(angle);
	_delay_ms(200); // Wait for the servo to move
	
	// Read the analog sensor
	int val = analog_read_10(0); // Read ADC channel 0
	int dist = linearize(val); // Linearize the ADC value
	
	// Print out the data
	itoa(angle, buffer, 10); // Convert number to printable ASCII
	serial.print(buffer); // Print the data
	serial.transmit(','); 
	itoa(dist, buffer, 10); // Convert number to printable ASCII
	serial.print(buffer); // Print the data
	
	// Map to a value to be used by the LEDs
	int mapped_val = _map(dist, 5, 20, 0, 255);
	return mapped_val;
}

void servo_move(int angle)
{
	// Convert angle to millis
	int millis = angle*10.5 + 600; 
	// Calculation derived from this equation: Angle*UPD + LOW = Millis
	// Where UPD = microseconds per degree, and low is the low end of millis for servo
	
	// Setup Timer 1 for driving servo motor
	TCCR1A = ((1<<COM1A1) | (1<<COM1A0) | (1<<WGM11)); // Set fast PWM on OC1A
	TCCR1B = ((1<<WGM13) | (1<<WGM12) | (1<<CS10)); // Set TOP to ICR1
	ICR1 = 19999; // Top value, gives proper frequency
	
	// Output signal
	OCR1A = ICR1 - millis;
}

void write_leds(int sig1, int sig2, int sig3, int sig4)
{
	/* Setup timer 0 */
	TCCR0A = ((1<<COM0A1) | (1<<COM0B1) | (1<<WGM00) | (1<<WGM01)); // Set fast PWM for OC0A and OC0B
	TCCR0B = (1<<CS00); // Clkio, no pre-scaler
	OCR0A = sig2; // Set desired duty cycle OC0A
	OCR0B = sig1; // Set desired duty cycle OC0B
	
	/* Setup timer 2 */
	TCCR2A = ((1<<COM2A1) | (1<<COM2B1) | (1<<WGM21) | (1<<WGM20)); // Set fast PWM for OC2A and OC2B
	TCCR2B = (1<<CS20); // Clkio, no pre-scaler
	OCR2A = sig4; // Set desired duty cycle for OC2A
	OCR2B = sig3; // Set desired duty cycle for OC2B
}

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

int linearize(int val)
{
	// Linearize the data (see linearize spreadsheet for details)
	return (16663/(val + 102)) - 10;
}