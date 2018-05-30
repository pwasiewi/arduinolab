//Conditionals - If statement
//Explanation: http://arduino.cc/en/Reference/If
//Explanation: http://arduino.cc/en/Reference/Random

long randnumber;                 //long is another data type, holds a 32 bit number

void setup() {                   //set up outputs
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  Serial.begin(9600);           //set up serial communication
  randomSeed(analogRead(0));    //this will make psudo random numbers seem more random
}

void loop() {
  randnumber= random(6);         //pick a random number between 0 and 5
  Serial.println(randnumber);    //prints the random number to the serial window

  if (randnumber==0){          //if conditional statement compares randnumber to 0. 
    digitalWrite(8, HIGH);     // turn the LED on (HIGH is the voltage level 5v)
    delay(200);                // wait for a second
    digitalWrite(8, LOW);      // turn the LED off by making the voltage LOW
    delay(200);                // wait for a second
  }                            //end of conditional statement
  if (randnumber==1){
    digitalWrite(9, HIGH);   
    delay(200);               
    digitalWrite(9, LOW);   
    delay(200);               
  }
  if (randnumber==2){
    digitalWrite(10, HIGH);   
    delay(200);               
    digitalWrite(10, LOW);    
    delay(200);               
  }
  if (randnumber!=0 && randnumber!=1 && randnumber!=2){  //use "&&" to test multiple conditions, use != for NOT equal to
    digitalWrite(8, HIGH);   
    digitalWrite(9, HIGH);   
    digitalWrite(10, HIGH);   
    delay(200);               
    digitalWrite(8, LOW);    
    digitalWrite(9, LOW);    
    digitalWrite(10, LOW);    
    delay(200);               
  }
  else{                                                 //if none of the above conditionals are true, do this.
    Serial.println("doing nothing");    
  }
}                                                      //end of main loop



