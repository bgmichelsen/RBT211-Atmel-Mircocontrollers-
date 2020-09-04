/*
 * tone.h
 *
 * Created: 10/12/2018 4:31:29 PM
 *  Author: Brandon Michelsen
 */ 


#ifndef TONE_H_
#define TONE_H_

// Function prototypes
void tone(int frequency, int duration); // Function to play a tone on Timer 0
void delay(int time); // Function for variable delay
void calc_ocr(int frequency, int* audio_ocr, int* prescaler_val); // Function to calculate the OCRnX value based on a given frequency

#endif /* TONE_H_ */