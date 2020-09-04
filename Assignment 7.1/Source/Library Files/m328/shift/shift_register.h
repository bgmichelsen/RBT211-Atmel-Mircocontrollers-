/*
 * shift_register.h
 *
 * Created: 10/15/2018 3:02:42 PM
 *  Author: Brandon Michelsen
 */ 


#ifndef SHIFT_REGISTER_H_
#define SHIFT_REGISTER_H_

// Defines
#define R_PORT	PORTD
#define SH_CLK	PD4
#define LATCH	PD3
#define DATA	PD2

#define DATA_HIGH()		(R_PORT |= (1<<DATA))
#define DATA_LOW()		(R_PORT &= ~(1<<DATA))
#define SH_CLK_HIGH()	(R_PORT |= (1<<SH_CLK))
#define SH_CLK_LOW()	(R_PORT &= ~(1<<SH_CLK))
#define LATCH_HIGH()	(R_PORT |= (1<<LATCH))
#define LATCH_LOW()		(R_PORT &= ~(1<<LATCH))

#define MSB_FIRST	1
#define LSB_FIRST	0

// Function prototype
void shift_init(); // Function to initiate the shift register
void shift_out(uint8_t bit_dir, uint8_t data); // Function for writing to the shift register

#endif /* SHIFT_REGISTER_H_ */