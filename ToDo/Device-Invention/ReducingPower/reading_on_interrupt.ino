// This sketch demonstrates INPUT_ PULLUP and Interrupts
int led = 13;                          //led becomes output
unsigned int reading = 0;              //variable to keep track of readings
volatile boolean interrupted = false;  //variable needed for interrupt

void setup()
{
  pinMode(2, INPUT_PULLUP);           //allows for button without resistor hook button to PIN 2 and GND
  pinMode(led, OUTPUT);               //set PIN 13 as OUTPUT
  attachInterrupt(0, takereading, FALLING);   //Turn on interrupt 0 which is PIN 2 and define function on interrupt
  Serial.begin(9600);                  //set up serial communication
}

void loop()
{
  delay (10000);
  if (interrupted == true){           //see if interrupt was executed
    reading=reading + 1;
    Serial.print(reading);
    interrupted = false;
    digitalWrite(led, HIGH);
    delay (300);
    digitalWrite(led, LOW);
  }
}

void takereading()                    //if interrupt function
{
  interrupted = true;              
}

