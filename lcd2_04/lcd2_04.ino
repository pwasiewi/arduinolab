/*
  Przykład obsługi wyświetlacza LCD
  Wyświetlenie na LCD przykładowego tekstu oraz wartości analogowej i napieciowej zmierzonej z wyjscia potencjometru.
  Przykład obsługi generatora PWM
  Jasność świecenia diody LED4 zalezna jest od napiecia zmierzonego z potencjometru
  Dioda LED3 pulsuje czyli zmienia jasnosc od najmniejszej do najwiekszej i odwrotnie
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


#include <LiquidCrystal.h>          //biblioteka obsługi LCD

#include "Timers.h"                 // dołączona biblioteka Timers z forum android http://bit.ly/arduinotimer
Timers <1> akcja;

const int Led1 = 13;                //przypisanie aliasów do pinów portów
const int Led2 = 12;
const int Led3 = 11;
const int Led4 = 10;
const int SW1 = 3;
const int SW2 = 2;
const int SW3 = 1;
const int SW4 = 0;
const int Buzzer = A5;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);//konfigurowanie linii do których został dołączony LCD


int wart = 0;                       //zmienna pomocnicza
int wart_pot;                       //zmienna na wartosc zmierzona z potencjometru
float wart_nap;                     //zmienna na wartosc zmierzonego napiecia

void setup() {                      //funkcja inicjalizacji
  lcd.begin(16, 2);                 //konfiguracja rozdzielczosci wyswietlacza LCD
  analogReference(DEFAULT);         //konfiguracja napiecia odniesienia dla przetwornika A/C - domyslnie napieciem odniesienia jest 5V.
  akcja.attach(0, 200, diody);      //Wątek 1
}

void diody() {
  lcd.clear();                      //czyszczenie LCD
  lcd.setCursor(3, 0);              //ustawienie kursora w 4 kolumnie pierwszego wiersza
  lcd.print("Analog I/O");          //wyswietlenie na LCD napisu Analog I/O
  wart_pot = analogRead(A0);        //pomiar napiecia z potencjometru do zmiennej wart_pot
  wart_nap = (5.0 * wart_pot) / 1024.0; //przeliczenie odczytanej wartosci na napiecie
  lcd.setCursor(0, 1);              //ustawienie kursora w pierwszej pozycji drugiego wiersza LCD
  lcd.print("U=");                  //wyswietlenie napisu U=
  lcd.print(wart_nap);              //wyswietlenie zmierzonej wartosci napiecia z potencjometru
  lcd.print("V");                   //wyswietlenie znaku V
  lcd.setCursor(8, 1);              //ustawienie kursora w 8 pozycji drugiego wiersza LCD
  lcd.print("A/C=");                //wyswietlenie napisu A/C=
  lcd.print(wart_pot);              //wyswietlenie zmierzonej wartosci przez A/C
  for (int i = 0; i < 256; i++)     //petla wykonywana 255 razy gdzie i zmienia wartosc od 0 do 255
  {
    analogWrite(Led4, i);           //zapisanie poziomu wypelnienia sygnalu PWM ze zmiennej i
    delay(1);                       //opoznienie
  }
  for (int i = 255; i > 0; i--)     //petla wykonywana 255 razy gdzie i zmienia wartosc od 255 do 0
  {
    analogWrite(Led4, i);           //zapisanie poziomu wypelnienia sygnalu PWM ze zmiennej i
    delay(1);                       //opoznienie
  }
  wart =  analogRead(A0);           //pomiar napiecia z potencjometru ktorego wartosc zapisywana jest do zmiennej wart
  analogWrite(Led3, wart / 4);      //zapisanie poziomu wypelnienia sygnalu PWM diody Led3 odczytanego z potencjometry i jego podziale przez 4
}

void loop() {                       //pętla główna programu
  akcja.process();                  //inicjalizacja lub aktualizacja wszystkich procedur(wątków, zdarzeń itp.)
}                                   //koniec pętli głównej

