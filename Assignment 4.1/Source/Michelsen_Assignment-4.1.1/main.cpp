/*
 * Michelsen_Assignment4-1.1.cpp
 *
 * Created: 9/24/2018 3:32:08 PM
 * Author : Brandon Michelsen
 */ 

#define F_CPU	16000000UL // 16 MHz clock

#define MOTOR	(1<<PD6) // Motor pin

#include <avr/io.h>
#include <util/delay.h>

// Function prototypes
void pwm_write(int duty_cycle); // Function to drive PWM signals

int main(void)
{
	// Setup inputs and outputs
	DDRD |= MOTOR;
	
    // Run infinitely
    while (1) 
    {
		for (int i = 0; i < 256; i++)
		{
			pwm_write(i);
			_delay_ms(10);
		}
		_delay_ms(1000);
		for (int i = 255; i >= 0; i--)
		{
			pwm_write(i);
			_delay_ms(10);
		}
		_delay_ms(1000);
    }
	
	return 0;
}

// Function definitions
void pwm_write(int duty_cycle)
{
	/* Setup Timer 0 */
	TCCR0A = ((1<<COM0A1) | (1<<WGM00) | (1<<WGM01)); // Fast PWM on OC0A
	TCCR0B = (1<<CS00); // Clkio, no pre-scaler
	OCR0A = duty_cycle; // Output PWM at desired duty cycle
}

