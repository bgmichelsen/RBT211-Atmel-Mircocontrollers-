/*
 * Michelsen_Assignment4-1.4.cpp
 *
 * Created: 9/25/2018 12:13:07 PM
 * Author : Brandon Michelsen
 */ 

#define F_CPU	16000000UL // 16 MHz clock

#include <avr/io.h>
#include <util/delay.h>

// Function prototypes
void servo_move(int millis); // Function to move the servo
int analog_read_10(int channel); // Function to read the ADC
int _map(int x, int in_min, int in_max, int out_min, int out_max); // Function to map values

int main(void)
{
	// Setup inputs and outputs
	DDRB |= (1<<DDB1);
	
	// Declare local variables
	int analog_val = 0; // Variable for read analog value
	int output_val = 0; // Variable for outputting signal to servo
	
    // Run infinitely
    while (1) 
    {
		analog_val = analog_read_10(0); // Read ADC channel 0
		output_val = _map(analog_val, 0, 1023, 800, 2300); // Map value to proper servo signals (1 ms min, 2 ms max)
		servo_move(output_val); // Move the Servo
    }
}

// Function definitions
void servo_move(int millis)
{
	// Setup Timer 1 for driving servo motor
	TCCR1A = ((1<<COM1A1) | (1<<COM1A0) | (1<<WGM11)); // Set fast PWM on OC1A
	TCCR1B = ((1<<WGM13) | (1<<WGM12) | (1<<CS10)); // Set TOP to ICR1
	ICR1 = 19999; // Top value, gives proper frequency
	
	// Output signal
	OCR1A = ICR1 - millis;
}

int analog_read_10(int channel)
{
	// Turn off digital I/O on Port C
	DIDR0 = 0x3F;
	
	// Choose the proper channel
	if (channel == 0) ADMUX = 0x00; // Channel 0
	if (channel == 1) ADMUX = 0x01; // Channel 1
	if (channel == 2) ADMUX = 0x02; // Channel 2
	if (channel == 3) ADMUX = 0x03; // Channel 3
	if (channel == 4) ADMUX = 0x04; // Channel 4
	if (channel == 5) ADMUX = 0x05; // Channel 5
	
	// Read the analog signal
	ADCSRA = ((1<<ADEN) | (1<<ADSC) | (1<<ADATE));
	int val = ADCL;
	val += 256*ADCH;
	return val;
}

int _map(int x, int in_min, int in_max, int out_min, int out_max)
{
	return (x - in_min)*(((float)(out_max - out_min))/(in_max - in_min)) + out_min;
}
