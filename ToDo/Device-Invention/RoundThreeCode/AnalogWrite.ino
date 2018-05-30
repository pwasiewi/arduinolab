// LED Fadeing Example (PWM)   http://arduino.cc/en/Reference/AnalogWrite

int led0 = 5;           // the pin that the LED is attached to
int led1 = 6;           // the pin that the LED is attached to
int led2 = 7;           // the pin that the LED is attached to NO PWM
int led3 = 8;           // the pin that the LED is attached to NO PWM
int led4 = 9;           // the pin that the LED is attached to
int led5 = 10;          // the pin that the LED is attached to
int led6 = 11;          // the pin that the LED is attached to
int led7 = 12;          // the pin that the LED is attached to NO PWM
int brightness = 0;     // LED brightness

void setup()  { 
  pinMode(led0, OUTPUT);  
  pinMode(led1, OUTPUT);  
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
} 

void loop()  { 
  //------fade in loop--------------
  for (int fadein=0;fadein<254;fadein++){
    // set the brightness for the LEDs that support PWM
    analogWrite(led0, brightness);
    analogWrite(led1, brightness);
    analogWrite(led4, brightness);
    analogWrite(led5, brightness);  
    analogWrite(led6, brightness);  

    // change the brightness for next time through the loop
    brightness = brightness + 1;
    // wait for 2 milliseconds to see the dimming effect    
    delay(2); 
  }
  //-----end of fade in loop---------
  //------fade out loop--------------
  for (int fadeout=0;fadeout<254;fadeout++){
    // set the brightness for the LEDs that support PWM
    analogWrite(led0, brightness);
    analogWrite(led1, brightness);
    analogWrite(led4, brightness);
    analogWrite(led5, brightness);  
    analogWrite(led6, brightness);  

    // change the brightness for next time through the loop
    brightness = brightness - 1;
    // wait for 2 milliseconds to see the dimming effect    
    delay(2); 
  }
  //-----end of fade in loop---------

  brightness=0;   //make sure brightness is set at before loop restarts
}


