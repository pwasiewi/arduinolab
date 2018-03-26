//Obsługa wyświetlacza LCD
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
#include <LiquidCrystal.h>    //biblioteka obsługi LCD

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);    //konfuguracja linii do ktorych został dołaczony LCD

int wart_temp=24;              //przykladowa zmienna z zapisana wartoscia temperatury

byte st[8] = {            //tablica znaku stopnia dla wyswietlacza LCD
  B00100,
  B01010,
  B00100,
  B00000,
  B00000,
  B00000,
  B00000,
};

void setup() {            //funkcja inicjalizacji
  lcd.begin(16, 2);        //konfiguracja rozdzielczosci wyswietlacza LCD
  lcd.createChar(0, st);  //funkcja utworzenia wlasnego znaku z tablicy st o kodzie 0
}
void loop() {                //petla glowna programu
 lcd.clear();              //czyszczenie LCD
 lcd.setCursor(2, 0);      //ustawienie kursora w 2 kolumnie pierwszego wiersza
 lcd.print("Arduino w EP");      //wyswietlenie na LCD napisu Arduino w EP
 lcd.setCursor(0, 1);      //ustawienie kursora w 1 kolumnie drugiego wiersza
 lcd.print("Temp: ");      //wyswietlenie na LCD napisu Temp:
 lcd.print(wart_temp);          //wyswietlenie wartosci zapisanej w zmiennej wart_temp
 lcd.write((char*)0);                        //wyswietlenie wczesniej zdefiniowanego znaku stopnia
 lcd.print("C");                      //wyswietlenie znaku C
 lcd.setCursor(12, 1);            //ustawienie kursora w 12 kolumnie drugiego wiersza
 lcd.cursor();                    //właczenie kursora
 lcd.blink();                     //włączenie migającego kursora
 while(1);  //nieskończona petla - zatrzymanie dzialania programu
}                                      //koniec petli glownie programu
