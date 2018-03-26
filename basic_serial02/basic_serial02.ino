/*
 *   Pinouts:
                                      +-----+
         +----[PWR]-------------------| USB |--+
         |                            +-----+  |
         |         GND/RST2  [ ][ ]            |
         |       MOSI2/SCK2  [ ][ ]  A5/SCL[ ] |   C5
         |          5V/MISO2 [ ][ ]  A4/SDA[ ] |   C4
         |                             AREF[ ] |
         |                              GND[ ] |
         | [ ]N/C                    SCK/13[ ] |  PB5
         | [ ]IOREF                 MISO/12[ ] |   .
         | [ ]RST                   MOSI/11[ ]~|   .
         | [ ]3V3    +---+               10[ ]~|   .
         | [ ]5v    -| A |-               9[ ]~|   .
         | [ ]GND   -| R |-               8[ ] |  PB0
         | [ ]GND   -| D |-                    |
         | [ ]Vin   -| U |-               7[ ] |  PD7
         |          -| I |-               6[ ]~|   .
     PC0 | [ ]A0    -| N |-               5[ ]~|   .
      .  | [ ]A1    -| O |-               4[ ] |   .
      .  | [ ]A2     +---+           INT1/3[ ]~|   .
      .  | [ ]A3                     INT0/2[ ] |   .
      .  | [ ]A4/SDA  RST SCK MISO     TX>1[ ] |   .
     PC5 | [ ]A5/SCL  [ ] [ ] [ ]      RX<0[ ] |  PD0
         |            [ ] [ ] [ ]              |
         |  UNO_R3    GND MOSI 5V  ____________/
          \_______________________/

		  http://busyducks.com/ascii-art-arduinos
 *
 */

#include <SoftwareSerial.h>

int bluetoothTx = 0; // TX-O pin of bluetooth mate, Arduino D2
int bluetoothRx = 1; // RX-I pin of bluetooth mate, Arduino D3

int led = 13;

int buttonPin1 = 7;
int buttonPin2 = 8;
int button1State = 0;
int button2State = 0;

int dataFromBt;

boolean lightBlink = true;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup()
{
 Serial.begin(115200); // Begin the serial monitor at 9600bps

 bluetooth.begin(115200); // The Bluetooth Mate defaults to 115200bps
 bluetooth.print("$"); // Print three times individually
 bluetooth.print("$");
 bluetooth.print("$"); // Enter command mode
 delay(100); // Short delay, wait for the Mate to send back CMD
 bluetooth.println("U,9600,N"); // Temporarily Change the baudrate to 9600, no parity
 // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
 bluetooth.begin(9600); // Start bluetooth serial at 9600
 pinMode(led, OUTPUT);
 pinMode(buttonPin1, INPUT);
 pinMode(buttonPin2, INPUT);
}

void loop()
{

 if(bluetooth.available()) // If the bluetooth sent any characters
 {
 // Send any characters the bluetooth prints to the serial monitor
 //Serial.println((char)bluetooth.read());

 dataFromBt = bluetooth.read();
 Serial.println(dataFromBt);
 if(dataFromBt == '1'){
 Serial.println("led on");
 digitalWrite(led, HIGH);
 bluetooth.print("1");
 }
 if(dataFromBt == '0'){
 Serial.println("led off");
 digitalWrite(led, LOW);
 bluetooth.print("0");
 }
 if(dataFromBt == 'b'){
 Serial.println("a");
 lightBlink = true;
 }else{
 lightBlink = false;
 }

 }

 if(Serial.available()) // If stuff was typed in the serial monitor
 {
 // Send any characters the Serial monitor prints to the bluetooth
 bluetooth.print((char)Serial.read());
 }

 // and loop forever and ever!
 if(lightBlink){
 digitalWrite(led, HIGH);
 bluetooth.print("1");
 Serial.println("HIGH");
 delay(500);
 digitalWrite(led, LOW);
 bluetooth.print("0");
 Serial.println("LOW");
 delay(500);
 }
}
