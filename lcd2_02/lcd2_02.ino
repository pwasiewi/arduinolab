/*
Przykład obsługi linii I/O mikrokontrolera.
Dolinii wejsciowych dolaczony 4 przyciski
Do linii wyjsciowych dolaczono 4 diody LED oraz generator PIEZO
Przycisniecie przycisku S1 powoduje zapalenie diody LED1
Przycisniecie przycisku S2 powoduje miganie diody LED2
Przycisniecie przycisku S3 powoduje generacje kodu SOS w kodzie Morse'a (SOS sygnalizowane jest miganiem LED3 oraz sygnalem z przeczyka PIEZO)
Przycisniecie przycisku S4 powoduje generowanie efekty swietlnego weza z wykorzystaniem diod LED1 - LED4
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

void setup() {                     //funkcja inicjalizacji
  lcd.begin(16, 2);                //konfigurowanie rozdzielczości LCD
  pinMode(Led1, OUTPUT);           //Konfiguracja linii do ktorych sa dolaczone diody jako wyjscia
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(Led4, OUTPUT);
  pinMode(Buzzer, OUTPUT);         //konfiguracja linii do ktorej jest dolaczony brzeczyk jako wyjscie
  pinMode(SW1, INPUT);             //konfiguracja linii do ktorych sa dolaczone przyciski jako wejscia
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);
  digitalWrite(SW1, HIGH);         //dolaczenie do linii do ktorych sa dolaczone przyciski rezystorow podciagajacych co wymusi na nich domyslnie stan wysoki
  digitalWrite(SW2, HIGH);
  digitalWrite(SW3, HIGH);
  digitalWrite(SW4, HIGH);
  digitalWrite(Led1, HIGH);        //domyslne wylaczenie diod LED
  digitalWrite(Led2, HIGH);
  digitalWrite(Led3, HIGH);
  digitalWrite(Led4, HIGH);
  digitalWrite(Buzzer, HIGH);      //domyslne wylaczenie brzeczyka
  akcja.attach(0, 200, przyciski); //Wątek 1
  lcd.setCursor(0, 0);             //ustawienie kursora w 5 kolumnie pierwszego wiersza
  lcd.print("Wybierz przycisk");   //wyświetlenie na LCD napisu Arduino 
}

void przyciski() {
  sprawdzprzycisk1();              //sprawdzenie czy naciśnięto przycisk S1 
  sprawdzprzycisk2();              //sprawdzenie czy naciśnięto przycisk S2 
  sprawdzprzycisk3();              //sprawdzenie czy naciśnięto przycisk S3 
  sprawdzprzycisk4();              //sprawdzenie czy naciśnięto przycisk S4
}

void sprawdzprzycisk1()
{
  if (digitalRead(SW1) == LOW) {   //Sprawdzenie czy nacisniety przycisk S1
    digitalWrite(Led1, LOW);       //Zapalenie LED1
    while(digitalRead(SW1) == LOW);//oczekiwanie na puszczenie przycisku S1
  }
  else {                           //w przeciwnym razie
    digitalWrite(Led1, HIGH);      //wylaczenie diody LED1
  }
}

void sprawdzprzycisk2()
{
  if (digitalRead(SW2) == LOW) {   //Sprawdzenie czy nacisniety przycisk S2
    while(digitalRead(SW2) == LOW) //jesli przycisk nadal przycisniety
    {
      digitalWrite(Led2, LOW);     //wlaczenie LED2
      delay(200);                  //opoznienie 200 ms
      digitalWrite(Led2, HIGH);    //wylaczenie LED2
      delay(200);                  //opoznienie 200 ms

    }
  }
  else {                           //w przeciwnym razie
    digitalWrite(Led2, HIGH);      //wylaczenie diody LED2
  }  
}

void sprawdzprzycisk3(){
  if (digitalRead(SW3) == LOW) {   //Sprawdzenie czy nacisniety przycisk S3
    while(digitalRead(SW3) == LOW) //jesli przycisk nadal przycisniety
    {
      for (int i=0; i<3; i++) {    //petla genetujaca trzy kropki
        digitalWrite(Led3, LOW);   //wlaczenie Led3
        digitalWrite(Buzzer, LOW); //wlaczenie brzeczyka
        delay(150);                //opoznienie 150 ms
        digitalWrite(Led3, HIGH);  //wylaczenie led3
        digitalWrite(Buzzer, HIGH);//wylaczenie brzeczyka
        delay(100);                //opoznienie 100 ms
      }
       for (int i=0; i<3; i++) {   //petla genetujaca trzy kreski
        digitalWrite(Led3, LOW);   //wlaczenie Led3
        digitalWrite(Buzzer, LOW); //wlaczenie brzeczyka
        delay(400);                //opoznienie 400 ms
        digitalWrite(Led3, HIGH);  //wylaczenie led3
        digitalWrite(Buzzer, HIGH);//wylaczenie brzeczyka
        delay(100);                //opoznienie 100 ms
      }
      for (int i=0; i<3; i++) {    //petla genetujaca trzy kropki
        digitalWrite(Led3, LOW);   //wlaczenie Led3
        digitalWrite(Buzzer, LOW); //wlaczenie brzeczyka
        delay(150);                //opoznienie 150 ms
        digitalWrite(Led3, HIGH);  //wylaczenie led3
        digitalWrite(Buzzer, HIGH);//wylaczenie brzeczyka
        delay(100);                //opoznienie 100 ms
      }
    }
  }
  else {                           //w przeciwnym razie
    digitalWrite(Led3, HIGH);      //wylaczenie diody LED3
    digitalWrite(Buzzer, HIGH);    //wylaczenie brzeczyka
  }  
}

void sprawdzprzycisk4(){
  if (digitalRead(SW4) == LOW) {   //Sprawdzenie czy nacisniety przycisk S4
    while(digitalRead(SW4) == LOW) //jesli przycisk nadal przycisniety
    {
      digitalWrite(Led1, LOW);     //wlaczenie Led1
      delay(100);                  //opoznienie 100 ms
      digitalWrite(Led2, LOW);     //wlaczenie Led2
      delay(100);                  //opoznienie 100 ms
      digitalWrite(Led3, LOW);     //wlaczenie Led3
      delay(100);                  //opoznienie 100 ms
      digitalWrite(Led4, LOW);     //wlaczenie Led4
      delay(100);                  //opoznienie 100 ms
      digitalWrite(Led1, HIGH);    //wylaczenie Led1
      delay(100);                  //opoznienie 100 ms
      digitalWrite(Led2, HIGH);    //wylaczenie Led2
      delay(100);                  //opoznienie 100 ms
      digitalWrite(Led3, HIGH);    //wylaczenie Led3
      delay(100);                  //opoznienie 100 ms
      digitalWrite(Led4, HIGH);    //wylaczenie Led4
      delay(100);                  //opoznienie 100 ms
    }
  }  
}

void loop() {                      //pętla główna programu
  akcja.process();                 //inicjalizacja lub aktualizacja wszystkich procedur(wątków, zdarzeń itp.)
}                                  //koniec pętli głównej

