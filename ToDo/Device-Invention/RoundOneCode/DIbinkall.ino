/*
  Blink All
 Turns on each LED on for one-tenth of a second, then off for for one-tenth of a second, repeatedly.
 */

// this integer data type controls the delay time
int flash = 100;

// the setup routine runs once when you press reset:
void setup() { 
 // sets up serial communication at 9600 bitss per second 
  Serial.begin(9600); 
  // initializes the digital pins as an outputs.
  pinMode(12, OUTPUT); 
  pinMode(11, OUTPUT); 
  pinMode(10, OUTPUT); 
  pinMode(9, OUTPUT); 
  pinMode(8, OUTPUT); 
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT); 
  pinMode(5, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level which is 5v)
  delay(flash);               // wait 
  digitalWrite(12, LOW);    // turn the LED off by making the voltage LOW (0v)
  digitalWrite(11, HIGH);   // turn the LED on (HIGH is the voltage level which is 5v)
  delay(flash);               // wait 
  digitalWrite(11, LOW);    // turn the LED off by making the voltage LOW (0v)
  digitalWrite(10, HIGH);   
  delay(flash);               
  digitalWrite(10, LOW);    
  digitalWrite(9, HIGH);   
  delay(flash);              
  digitalWrite(9, LOW);    
  digitalWrite(8, HIGH);   
  delay(flash);               
  digitalWrite(8, LOW);    
  digitalWrite(7, HIGH);   
  delay(flash);               
  digitalWrite(7, LOW);    
  digitalWrite(6, HIGH);   
  delay(flash);               
  digitalWrite(6, LOW);    
  digitalWrite(5, HIGH);   
  delay(flash);               
  digitalWrite(5, LOW);    
  Serial.println("looping");    // communication sent back to serial window on PC - ln is a line break
}


