// Array Fade Example  http://arduino.cc/en/Reference/Array
int ledPins[] = { 
  5, 6, 9, 10, 11};       // an array of pin numbers to which LEDs are attached
int pinCount = 5;           // the number of pins (i.e. the length of the array)

// the setup routine runs once when you press reset:
void setup()  { 
  for (int thisPin = 0; thisPin < pinCount; thisPin++)  {     //use an array to set all pins as outputs
    pinMode(ledPins[thisPin], OUTPUT);      
  }
} 

void loop()  {                                                //fade LEDs in one at a time
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {      //do a fade for each pin
    for (int brightness=0;brightness<256;brightness++){       // set the brightness with this nested loop
      analogWrite(ledPins[thisPin],brightness);
      delay(10);  
    }      
  }                                                          //fade LEDs out one at a time
  for (int thisPin = pinCount; thisPin > 0; thisPin--) {
    for (int brightness=255;brightness>0;brightness--){
      analogWrite(ledPins[thisPin],brightness);
      delay(10);  
    }                                                         //end brightness loop 
  }                                                           //end thisPin For loop
}                                                             //end main loop



