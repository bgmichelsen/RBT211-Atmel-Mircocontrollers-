/*
 * Michelsen_Assignment4-1.3.cpp
 *
 * Created: 9/24/2018 4:58:29 PM
 * Author : Brandon Michelsen
 */ 

#define F_CPU	16000000UL // 16 MHz clock

#define EN1		(1<<PD6) // Enable pin on the H-Bridge
#define MC1		(1<<PD7) // Control pin 1
#define MC2		(1<<PB0) // control pin 2

#include <avr/io.h>
#include <util/delay.h>

// Function prototypes
void pwm_write(int duty_cycle); // Function for writing a PWM signal
int analog_read_10(int channel); // Function for reading an analog signal
long num_map(long x, long in_min, long in_max, long out_min, long out_max); // Function for mapping a number range to another

void forward(int speed); // Function for driving the motor forward
void reverse(int speed); // Function for driving the motor reverse
void brake(); // Function for stopping the motor

int main(void)
{
	// Setup inputs and outputs
	DDRD |= (EN1 | MC1);
	DDRB |= MC2;
	
	// Declare local variables
	int val = 0; // Variable to read analog values
	int velocity = 0; // Variable to control the motor speed
	
    // Run infinitely
    while (1) 
    {
		val = analog_read_10(0); // Read channel 0 on the ADC
		
		if (val > 562)
		{
			// Drive forward, potentiometer increases speed
			velocity = num_map(val, 563, 1023, 0, 255);
			forward(velocity);
		}
		else if (val < 462)
		{
			// Drive reverse, potentiometer increases speed
			velocity = num_map(val, 461, 0, 0, 255);
			reverse(velocity);
		}
		else
		{
			// Stop the motors
			brake();
		}
    }
	
	return 0;
}

// Function definitions
void pwm_write(int duty_cycle)
{
	/* Setup Timer 0 */
	TCCR0A = ((1<<COM0A1) | (1<<WGM00) | (1<<WGM01)); // Fast PWM on OC0A
	TCCR0B = (1<<CS00); // Clkio, no pre-scaler
	OCR0A = duty_cycle; // Drive the PWM signal at desired duty cycle
}

int analog_read_10(int channel)
{
	// Turn off digital I/O on Port C
	DIDR0 = 0x3F;
	
	// Determine the proper ADC channel
	if (channel == 0) ADMUX |= 0x00; // Channel 0
	if (channel == 1) ADMUX |= 0x01; // Channel 1
	if (channel == 2) ADMUX |= 0x02; // Channel 2
	if (channel == 3) ADMUX |= 0x03; // Channel 3
	if (channel == 4) ADMUX |= 0x04; // Channel 4
	if (channel == 5) ADMUX |= 0x05; // Channel 5
	
	// Read the analog signal
	ADCSRA = ((1<<ADEN) | (1<<ADSC) | (1<<ADATE));
	_delay_ms(1);
	int val = ADCL;
	val += 256*ADCH;
	return val;
}

long num_map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (((float)(out_max - out_min))/(in_max - in_min))*(x - in_min) + out_min;
}

void forward(int speed)
{
	// Disable the motor temporarily
	PORTD &= ~(EN1);
	
	// Set up pins for driving forward
	PORTD |= MC1;
	PORTB &= ~(MC2);
	
	// Drive the motor at the desired speed
	pwm_write(speed);
}

void reverse(int speed)
{
	// Disable the motor temporarily
	PORTD &= ~(EN1);
	
	// Set up pins for driving reverse
	PORTD &= ~(MC1);
	PORTB |= MC2;
	
	// Drive the motor at the desired speed
	pwm_write(speed);
}

void brake()
{
	// Turn off the motor
	PORTD &= ~(EN1);
	PORTD &= ~(MC1);
	PORTB &= ~(MC2);
}