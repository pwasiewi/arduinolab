
// * Simple Arduino Draw

import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;      // Data received from the serial port
int shrink;

void setup() 
{
  size(500, 500);
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = Serial.list()[9];
  myPort = new Serial(this, portName, 9600);
  background(100);
}

void draw()
{
  if ( myPort.available() > 0) {  // If data is available,
    val = myPort.read();         // read it and store it in val
  }
               // Set background to white
  if (val == 0) {              // If the serial value is 0,
   shrink++;
    ellipseMode(CENTER);            //draw ellipse from center
    stroke(random(0),random(0),random(255));
    ellipse(250, 250, 500-shrink, 500-shrink);                 
  } 
  else {                       // If the serial value is not 0,
    rectMode(CENTER);          //draw retanble from center
    stroke(random(3),random(255),random(255));
    rect(250, 250, 500-shrink, 500-shrink);              
  }
  
}

