/*
 * Michelsen_Assignment4-1.2.cpp
 *
 * Created: 9/24/2018 4:15:44 PM
 * Author : Brandon Michelsen
 */ 

#define F_CPU	16000000UL // 16 MHz clock

#define MOTOR	(1<<PD6) // Motor pin

#include <avr/io.h>

// Function prototypes
void pwm_write(int duty_cycle); // Function for writing a PWM signal
int analog_read_8(int channel); // Function to read an analog signal

int main(void)
{
	// Setup inputs and outputs
	DDRD |= MOTOR;
	
	// Declare local variables
	int analog_val = 0; // Variable for sensed analog value
	
    // Run infinitely
    while (1) 
    {
		analog_val = analog_read_8(0); // Read analog channel 0
		pwm_write(analog_val); // Write the value to the motor
    }
	
	return 0;
}

// Function definitions
void pwm_write(int duty_cycle)
{
	/* Setup Timer 0 */
	TCCR0A = ((1<<COM0A1) | (1<<WGM00) | (1<<WGM01)); // Fast PWM for OC0A
	TCCR0B = (1<<CS00); // Clkio, no pre-scaler
	OCR0A = duty_cycle; // Drive PWM signal at desired duty cycle
}

int analog_read_8(int channel)
{
	// Turn off digital I/O on Port C
	DIDR0 = 0x3F;
	
	// Set 8-bit mode for ADC
	ADMUX = (1<<ADLAR);
	
	// Determine the ADC channel
	if (channel == 0) ADMUX |= 0x00; // Channel 0
	if (channel == 1) ADMUX |= 0x01; // Channel 1
	if (channel == 2) ADMUX |= 0x02; // Channel 2
	if (channel == 3) ADMUX |= 0x03; // Channel 3
	if (channel == 4) ADMUX |= 0x04; // Channel 4
	if (channel == 5) ADMUX |= 0x05; // Channel 5
	
	// Read the analog value
	ADCSRA = ((1<<ADEN) | (1<<ADSC) | (1<<ADATE));
	
	return ADCH;
}

