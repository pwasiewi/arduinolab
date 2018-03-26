/*
Przykład obsługi łańcuchow tekstowych string
Instrukcje programu powoduja wyswietlenie na wyswietlaczu
kilku lancuchow wraz z operacja dodawania oraz zapisu wartosci zmierzonej przez A/C
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


#include <LiquidCrystal.h>    //biblioteka obsługi LCD

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);    //konfuguracja linii do ktorych został dołaczony LCD

void setup() {            //funkcja inicjalizacji
  lcd.begin(16, 2);        //konfiguracja rozdzielczosci wyswietlacza LCD
}

void loop()
{                //petla glowna programu
  lcd.clear();              //czyszczenie LCD
  delay(1000);    //opoznienie 1s
  String string1 = "String ";   //utworzenie zmiennej string String1 z zapisanym do niej przykladowym tekstem
  lcd.print(string1);      // wyswietlenie na LCD zawartosci String1
  delay(1000);                          //opoznienei 1s
  string1 =  String('1');        //zapisanie do zmiennej String1 wartosci 1
  lcd.print(string1); // wyswietlenie na LCD zawartosci String1
  delay(1000);                          //opoznienei 1s
  lcd.clear();  //czyszczenie LCD
  String string2 = String("Arduino");  //utworzenie zmiennej string String2 z zapisanym do niej przykladowym tekstem
  lcd.print(string2);   // wyswietlenie na LCD zawartosci String2
  delay(1000);                          //opoznienei 1s
  lcd.clear();  //czyszczenie LCD
  string1 =  String(string2 + " w EP");     //zapisanie do zmiennej string1 tekstu zapisanego w string2 oraz dodatkowego tekstu w EP
  lcd.print(string1);// wyswietlenie na LCD zawartosci String1
  delay(1000);                          //opoznienei 1s

  while(1)    //poczatek nieskonczonej petli
  {
    string2 =  String(analogRead(A0), DEC); //odczyt wartosci z potencjometru przez A/C i zapis wyniku do string2
    lcd.setCursor(0, 1);  //ustawienie kursora na poczatku drugiej linii LCD
    string1 =  String("      ");   //zapisanie do string 1 kilku znakow spacjii
    lcd.print(string1);// wyswietlenie na LCD zawartosci String1
    lcd.setCursor(0, 1);//ustawienie kursora na poczatku drugiej linii LCD
    lcd.print(string2);// wyswietlenie na LCD zawartosci String2
    delay(100);   //opoznienie 100 ms
  };
}                                      //koniec petli glownej programu

