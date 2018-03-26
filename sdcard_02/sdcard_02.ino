 // include the SD library:
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



// set up variables using the SD utility library functions:

Sd2Card card;

SdVolume volume;

SdFile root;



// change this to match your SD shield or module;

// Arduino Ethernet shield: pin 4

// Adafruit SD shields and modules: pin 10

// Sparkfun SD shield: pin 8

const int chipSelect = 8;



void setup()

{

  Serial.begin(115200);

  Serial.print("\nInitializing SD card...");

  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.

  // Note that even if it's not used as the CS pin, the hardware SS pin

  // (10 on most Arduino boards, 53 on the Mega) must be left as an output

  // or the SD library functions will not work.

  pinMode(chipSelect, OUTPUT);     // change this to 53 on a mega



  // we'll use the initialization code from the utility libraries

  // since we're just testing if the card is working!

  if (!card.init(SPI_QUARTER_SPEED, chipSelect)) {

    Serial.println("initialization failed. Things to check:");

    Serial.println("* is a card is inserted?");

    Serial.println("* Is your wiring correct?");

    Serial.println("* did you change the chipSelect pin to match your shield or module?");

    return;

  } else {

   Serial.println("Wiring is correct and a card is present.");

  }



  // print the type of card

  Serial.print("\nCard type: ");

  switch(card.type()) {

    case SD_CARD_TYPE_SD1:

      Serial.println("SD1");

      break;

    case SD_CARD_TYPE_SD2:

      Serial.println("SD2");

      break;

    case SD_CARD_TYPE_SDHC:

      Serial.println("SDHC");

      break;

    default:

      Serial.println("Unknown");

  }



  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32

  if (!volume.init(card)) {

    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");

    return;

  }









  // print the type and size of the first FAT-type volume

  long volumesize;

  Serial.print("\nVolume type is FAT");

  Serial.println(volume.fatType(), DEC);

  Serial.println();



  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks

  volumesize *= volume.clusterCount();       // we'll have a lot of clusters

  volumesize *= 512;                            // SD card blocks are always 512 bytes

  Serial.print("Volume size (bytes): ");

  Serial.println(volumesize);

  Serial.print("Volume size (Kbytes): ");

  volumesize /= 1024;

  Serial.println(volumesize);

  Serial.print("Volume size (Mbytes): ");

  volumesize /= 1024;

  Serial.println(volumesize);



  Serial.println("\nFiles found on the card (name, date and size in bytes): ");

  root.openRoot(volume);



  // list all files in the card with date and size

  root.ls(LS_R | LS_DATE | LS_SIZE);

}





void loop(void) {



}

