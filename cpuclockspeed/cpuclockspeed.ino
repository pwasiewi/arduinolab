 /*
  Mierzenie częstotliwości taktowania procesora Arduino
  https://github.com/arkhipenko/CPUClockSpeed
  Pinouts:
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

*/

// These lines are for testing on an Arduino ATTiny:
#include <Arduino.h>
#include <SoftwareSerial.h>
const int rx = 4;
const int tx = 3;
#define   S   mySerial
SoftwareSerial mySerial(rx, tx); // RX, TX
// end

// Uncomment this to test on Arduino UNO, Nano, ESPx, Teensy:
#define S Serial
// end

unsigned long t1, t2, cnt;
void setup() {
  // put your setup code here, to run once:
  S.begin(115200);
  S.println(F("Processor speed test"));
  S.println("====================");
  S.print(F("Reported speed=")); S.println((float)(F_CPU/1000000.0));
  S.println();
  delay(5000);
  
  t1 = micros();
  delay(1000);
  t2 = micros();
  S.print(F("Method 1 sec delay (micros): t1=")); S.print(t1); S.print("\tt2="); S.print(t2); S.print("\tdiff="); S.println(t2-t1);
  S.println(); S.flush(); delay(100);

  cnt = 1987895UL; // This takes ~1 second on 16MHz Uno
  unsigned long tmp=0;
  t1 = micros();
  while (cnt-- > 0) {
    asm("nop");
  }
  t2 = micros();
  S.print(F("Method 2: 1M for loop (micros) t1=")); S.print(t1); S.print("\tt2="); S.print(t2); S.print("\tdiff="); S.println(t2-t1);
  S.println(); S.flush();
  float d = (float) t2-t1;
  
  // reverse relationaship:
  // 16 mhz = 1000000 micros
  // X mhz = d micros
  // x = 16 * d / 993937;
  float mhz = 16000000.0 / d;
  S.print(F("Calculated MHz = ")); S.print(mhz); S.println("MHz");
}


void loop() {
  // put your main code here, to run repeatedly:

}

