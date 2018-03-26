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

#include <SD.h>



File myFile;

const int chipSelect = 8;
const int chipdeny = 10;


void setup()

{

  Serial.begin(115200);

  Serial.print("Initializing SD card...");

  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.

  // Note that even if it's not used as the CS pin, the hardware SS pin

  // (10 on most Arduino boards, 53 on the Mega) must be left as an output

  // or the SD library functions will not work.
  pinMode(chipSelect, OUTPUT);
  digitalWrite(chipSelect, HIGH);




  if (!SD.begin(chipSelect)) {

    Serial.println(SD.begin(chipSelect));

    //return;

  }

  Serial.println("initialization done.");



  // open the file. note that only one file can be open at a time,

  // so you have to close this one before opening another.

  myFile = SD.open("test.txt", FILE_WRITE);



  // if the file opened okay, write to it:

  if (myFile) {

    Serial.print("Writing to test.txt...");

    myFile.println("testing 1, 2, 3.");

          // close the file:

    myFile.close();

    Serial.println("done.");

  } else {

    // if the file didn't open, print an error:

    Serial.println("error opening test.txt");

  }

}



void loop()

{

          // nothing happens after setup

}

