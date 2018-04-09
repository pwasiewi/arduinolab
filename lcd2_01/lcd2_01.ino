/*
  Przykład programu do obsługi modułu AVT1615 z:
  - wyświetlaczem LCD 2x16 znaków
  - 4 diodami LED
  - 4 przyciskami
  - brzęczykiem piezzo
  - czujnikiem temperatury LM35
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

#include "Timers.h"                 //dołączona biblioteka Timers z forum android http://bit.ly/arduinotimer
Timers <2> akcja;

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
float wart_pot;                     //zmienna na wartość zmierzona z potencjometru
float wart_czuj;                    //zmienna na wartość zmierzonej z czujnika temperatury
float wart_nap;                     //zmienna na wartość zmierzonego napięcia
float temperatura;                  //zmienna na wartość zmierzonej temperatury
byte st[8] = {                      //tablica znaku stopnia dla wyswietlacza LCD
  B00100,
  B01010,
  B00100,
  B00000,
  B00000,
  B00000,
  B00000,
};


void sprawdzprzycisk(int SW, int i)
{
  if (digitalRead(SW) == LOW) {
    digitalWrite(Led1, LOW);       //zaświecenie LED1
    digitalWrite(Buzzer, LOW);     //włączenie brzęczyka
    lcd.clear();                   //czyszczenie LCD
    lcd.setCursor(2, 0);           //ustawienie kursora w pierwszym rzędzie i drugiej kolumnie LCD
    lcd.print("S"); lcd.print(i);  //wyświetlenie nazwy przycisku
    while (digitalRead(SW) == LOW);//oczekiwanie na zwolnienie przycisku S1
  } else {                         //w przeciwnym razie
    digitalWrite(Led1, HIGH);      //wyłączenie diody LED1
    digitalWrite(Buzzer, HIGH);    //wyłączenie brzęczyka
  }
}

void przyciski() {
  sprawdzprzycisk(SW1, 1);         //sprawdzenie czy naciśnięto przycisk S1
  sprawdzprzycisk(SW2, 2);         //sprawdzenie czy naciśnięto przycisk S2
  sprawdzprzycisk(SW3, 3);         //sprawdzenie czy naciśnięto przycisk S3
  sprawdzprzycisk(SW4, 4);         //sprawdzenie czy naciśnięto przycisk S4
}

float srednia(int PIN, int LOOP) {
  for (int i = 0; i < LOOP; i++) { //pomiar napięcia z potencjometru i dodanie wyniku do wart_pot
    wart_pot = wart_pot + analogRead(PIN);//pętla wykonywana 20 razy
  }
  wart_pot = wart_pot / LOOP;        //obliczenie średniej arytmetycznej z 20 pomiarów
}

void termometr() {
  //lcd.clear();                   //czyszczenie LCD
  lcd.setCursor(4, 0);             //ustawienie kursora w 5 kolumnie pierwszego wiersza
  lcd.print("ArduinoL");           //wyświetlenie na LCD napisu Arduino
  wart_pot = srednia(A0, 20);      //obliczenie średniej arytmetycznej z 20 pomiarów
  wart_nap = (5.0 * wart_pot) / 1024.0;//przeliczenie odczytanej wartości na napięcie
  lcd.setCursor(0, 1);             //ustawienie kursora w pierwszej pozycji drugiego wiersza LCD
  lcd.print("U=");                 //wyświetlenie napisu U=
  lcd.print(wart_nap);             //wyświetlenie napięcia
  lcd.print("V ");                 //wyświetlenie znaku V
  wart_czuj = srednia(A1, 20);     //obliczenie średniej arytmetycznej z 20 pomiarów
  temperatura = (5.0 * wart_czuj * 100) / 1024.0;//przeliczenie wartości na stopnie Celsjusza
  //ustawienie kursora na pozycji 9 drugiego wiersza LCD lcd.setCursor(9, 1);
  lcd.print("T=");                 //wyświetlenie napisu T=
  lcd.print((long)temperatura);
  //wyświetlenie wartości temperatury zaokrąglonej do pełnych stopni
  lcd.write((uint8_t)0);                    //wyświetlenie znaku stopnia
  lcd.print("C");                  //wyświetlenie znaku C
}

void setup() {                     //funkcja inicjalizacji
  lcd.begin(16, 2);                //konfigurowanie rozdzielczości LCD
  lcd.createChar(0, st);           //funkcja utworzenia własnego znaku z tablicy st o kodzie 0
  analogReference(DEFAULT);        //konfigurowanie napięcia odniesienia dla przetwornika A/C - domyślnie 5V
  pinMode(Led1, OUTPUT);           //konfigurowanie I/O, do których są dołączone diody LED
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(Led4, OUTPUT);
  pinMode(Buzzer, OUTPUT);         //konfigurowanie I/O, do której jest dołączony brzęczyk piezzo
  pinMode(SW1, INPUT);             //konfigurowanie I/O, do których są dołączone przyciski
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);
  digitalWrite(SW1, HIGH);         //dołączenie wewnętrznych rezystorów zasilających
  digitalWrite(SW2, HIGH);
  digitalWrite(SW3, HIGH);
  digitalWrite(SW4, HIGH);
  digitalWrite(Led1, HIGH);        //wyłączenie diod LED
  digitalWrite(Led2, HIGH);
  digitalWrite(Led3, HIGH);
  digitalWrite(Led4, HIGH);
  digitalWrite(Buzzer, HIGH);      //wyłączenie brzęczyka piezzo
  akcja.attach(0, 122, przyciski); // Wątek 1
  akcja.attach(1, 500,  termometr);// Wątek 2
}

void loop() {                      //pętla główna programu
  akcja.process();                 //inicjalizacja lub aktualizacja wszystkich procedur(wątków, zdarzeń itp.)
}                                  //koniec pętli głównej


