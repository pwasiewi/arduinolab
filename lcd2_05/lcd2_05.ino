/*
Przykład obsługi przerwan wewnętrznych oraz zewnetrznych.
Przerwanie zewnetrzne jest zglaszane po nacisnieciu przycisku S4. Powoduje zapalenie lub zgaszenie diody Led1.
Przerwanie wewnetrzne jest zglaszane od timer1 co 500 ms i powoduje miganie diody Led3.
W procedura glowna programu powoduje miganie diody Led2
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

#include "TimerOne.h"  //biblioteka obslugi timera1

const int Led1 =  13;         //przypisanie aliasów do pinów portów
const int Led2 =  12;
const int Led3 =  11;
const int SW4 =  0;

byte f_led1 = 0;              //flaga diody Led1
byte f_led2 = 0;              //flaga diody Led2
byte f_led3 = 0;              //flaga diody Led3

void setup() {            //funkcja inicjalizacji
  pinMode(Led1, OUTPUT);    //Konfiguracja linii do ktorych sa dolaczone diody jako wyjscia
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(SW4, INPUT);    //konfiguracja linii do ktorej dolaczono SW4
  digitalWrite(SW4, HIGH);  //dolaczenie do SW4 rezystora podciagajacego
  digitalWrite(Led1, HIGH);  //domyslne wylaczenie diod LED
  digitalWrite(Led2, HIGH);
  digitalWrite(Led3, HIGH);
  attachInterrupt(SW4, on_off, FALLING);   //konfiguracja zewnetrznego przerwania od SW4 wywolujacego procedure on_off przy opadajacym zboczu na SW4
  Timer1.initialize(500000);      //inicjalizacja timer 1 - przerwanie co 500 ms
  Timer1.attachInterrupt(int_led3);   //uruchomienie przerwania od timer 1 w procedurze int_led3
}

void loop() {                //petla glowna programu
  digitalWrite(Led1, f_led1);   //zapis stanu Led1
  digitalWrite(Led2, f_led2);  //zapis stanu Led2
  f_led2 = !f_led2;   //zmiana stanu na przeciwny flagi led2
  delay(100);                          //opoznienie 100ms
}                                      //koniec petli glownej programu

void on_off()      //procedura obslugi przerwania zewnetrznego
{
  f_led1 = !f_led1;   //zmiana na przeciwna stanu flagi dla Led1
}

void int_led3() {      //funkcja przerwania zglaszanego od timer 3 obslugujacego led3
  digitalWrite(Led3, f_led3);  //zapis stanu Led3
  f_led3 = !f_led3;   //zmiana na przeciwna stanu flagi dla Led3
};


