// Setup serial
import processing.serial.*;

// Create an image
PImage img;

// Create the serial port
Serial port;

void setup()
{
  size(640,256); // Create a window the size of the image
  img = loadImage("hsv.jpg"); // Load the background image
  port = new Serial(this, Serial.list()[0], 9600); // Create the serial port
}

void draw()
{
  background(0); // Black background
  image(img,0,0); // Overlay image
}

void mousePressed()
{
  color c = get(mouseX, mouseY); // Get the mouse position and translate to a color
  String colors = int(red(c))+","+int(green(c))+","+int(blue(c))+"\n"; // Extract colors
  print(colors); // Debugging
  port.write(colors); // Send message to ATmega328
}