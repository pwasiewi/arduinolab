// Function Example  http://arduino.cc/en/Reference/AnalogWrite

int led0 = 5;           // the pin that the LED is attached to
int led1 = 6;           // the pin that the LED is attached to
int led2 = 7;           // the pin that the LED is attached to NO PWM
int led3 = 8;           // the pin that the LED is attached to NO PWM
int led4 = 9;           // the pin that the LED is attached to
int led5 = 10;          // the pin that the LED is attached to
int led6 = 11;          // the pin that the LED is attached to
int led7 = 12;          // the pin that the LED is attached to NO PWM
int brightness = 0;     // LED brightness
int fadeAmount = 2;     // how many levels to fade the LED by 0=off 255=full bright

void setup()  {           //set pins as outputs
  pinMode(led0, OUTPUT);  
  pinMode(led1, OUTPUT);  
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
} 

void loop() {             //main loop
  fade();                 //call function
  flash();                //call function
}                         //end main loop

void fade(){                                   //fade function starts here
  for (int counter; counter<256;counter++){  
    analogWrite(led0, brightness);
    analogWrite(led1, brightness);
    analogWrite(led4, brightness);
    analogWrite(led5, brightness);  
    analogWrite(led6, brightness);  

    // change the brightness for next time through the loop:
    brightness = brightness + fadeAmount;

    // reverse the direction of the fading at the ends of the fade: 
    if (brightness == 0 || brightness >= 254) {
      fadeAmount = -fadeAmount ; 
    }    
    // wait for 50 milliseconds to see the dimming effect    
    delay(10);  
  }  
}                                              //fade function ends here, return where main code left off

void flash(){                                  //fade function starts here
  for (int counter; counter<20;counter++){
    digitalWrite (led2, HIGH);
    digitalWrite (led3, HIGH);
    digitalWrite (led7, HIGH);
    delay(20);
    digitalWrite (led2, LOW);
    digitalWrite (led3, LOW);
    digitalWrite (led7, LOW);
    delay(20);
  }     
}                                              //fade function ends here, return where main code left off



