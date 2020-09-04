// Import serial library
import processing.serial.*;
Serial port; 

// Variable for storing returned potentiometer value
float brightness = 0;

void setup()
{
  size(500, 500);
  port = new Serial(this, Serial.list()[0], 9600); // Create the serial port
  port.bufferUntil('\n');
}

void draw()
{
  background(0,0,brightness); // Update the brightness of the window
}

void serialEvent(Serial port)
{
  brightness = float(port.readStringUntil('\n'));
}