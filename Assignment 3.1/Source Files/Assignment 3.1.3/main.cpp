/*
 * Michelsen_Assignment3-1.2.cpp
 *
 * Created: 9/17/2018 3:37:04 PM
 * Author : Brandon Michelsen
 */ 

#define F_CPU	16000000UL // 16 MHz

#define LOW_THRESHOLD	0 // Lower bound of light range
#define HIGH_THRESHOLD	1023 // Upper bound of light range

#include <avr/io.h>
#include <util/delay.h>

// Function prototypes
void pwm_write(uint8_t duty_cycle); // Function to write to a PWM signal
int adc_read_10(int channel); // Function to read an analog value
int num_map(int x, int in_min, int in_max, int out_min, int out_max); // Function to map a value
int constrain(int x, int min, int max); // Function to constrain a value

int main(void)
{
	// Set inputs and outputs
	DDRD |= (1<<DDD6);
	
	// Declare local variables
	int light_val = 0; // Variable for reading the photo-resistor
	
    // Run infinitely
    while (1) 
    {
		light_val = adc_read_10(0); // Read in the value of the photo-resistor connected to channel 0
		
		// Map the light value to be between 0 and 255
		light_val = num_map(light_val, LOW_THRESHOLD, HIGH_THRESHOLD, 0, 255);
		//light_val = constrain(light_val, 0, 255);
		// Write the value to the red of the RGB
		pwm_write(light_val);
	}
	
	return 0;
}

// Function definitions
void pwm_write(uint8_t duty_cycle)
{
	/* Timer 0 PWM Setup */
	TCCR0A = ((1<<COM0A1) | (1<<WGM00) | (1<<WGM01)); // Fast PWM for OC0A
	TCCR0B = (1<<CS00); // 4kHz (no pre-scaler)
	OCR0A = duty_cycle; // Write the PWM signal at the desired duty cycle
}

int adc_read_10(int channel)
{
	DIDR0 = 0x3F; // Turn off digital inputs on PORTA
	
	// Set the proper channel
	if (channel == 0) ADMUX = 0x00; // Channel 0
	if (channel == 1) ADMUX = 0x01; // Channel 1
	if (channel == 2) ADMUX = 0x02; // Channel 2
	if (channel == 3) ADMUX = 0x03; // Channel 3
	if (channel == 4) ADMUX = 0x04; // Channel 4
	if (channel == 5) ADMUX = 0x05; // Channel 5
	
	// Read the analog value
	ADCSRA = ((1<<ADEN) | (1<<ADSC) | (1<<ADATE));
	_delay_ms(1);
	
	// Return the read value
	int value = ADCL; // Read the low byte of the ADC
	value += (256*ADCH); // Read the high byte of the ADC
	return value;
}

int num_map(int x, int in_min, int in_max, int out_min, int out_max)
{
	// Linear interpolation6
	return ((x-in_min)*(out_max-out_min)/(in_max-in_min) + out_min);
}

int constrain(int x, int min, int max)
{
	// Check if the number is within the given value range
	if (x < min)
		return min;
	else if (x > max)
		return max;
	else
		return x;
}