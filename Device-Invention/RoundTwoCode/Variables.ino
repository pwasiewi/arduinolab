/*
Uses a variable to slow down blinking
http://arduino.cc/en/Tutorial/Variables
 */
 
// set a pin to be a variable:
int led = 12;
int interval = 5;
int addtime = 2;

void setup() {                
  
  pinMode(led, OUTPUT);      // initialize the digital pin as an output.
  Serial.begin(9600);        // initialize serial communication at 9600 bits per second: 
}

void loop() {
  digitalWrite(led, HIGH);       // turn the LED on (HIGH is the voltage level 5v)
  delay(interval);               // wait for "interval" time
  digitalWrite(led, LOW);        // turn the LED off by making the voltage LOW
  delay(interval);               // wait for "interval" time
  interval = interval+addtime;   //increase interval by adding the value of "addtime"
  Serial.print("interval=");     //prints the word "interval" on the screen
  Serial.println (interval);     //prints the VALUE of "interval" and makes a new line
}
