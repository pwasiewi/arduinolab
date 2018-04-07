

/*
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

  Przykład programu do obsługi modułu AVT1722 miniLCD z:
  - wyświetlaczem LCD 2x8 znaków
  - diodą LED RGB G 10, R 11, B 12 (kolor podłączenie arduino)
  - 4 przyciskami A1,A2,A3,A4
  - przekaźnik A0
  - impulsator 0 1 2
*/
#include <LiquidCrystal.h>          //biblioteka obsługi LCD

//#include "Timers.h"               //dołączona biblioteka Timers z forum android http://bit.ly/arduinotimer
//Timers <2> akcja;

#include <Timers.h>                 //Nettigo timers 
Timer NettigoTimer1;
Timer NettigoTimer2;

unsigned long currentTime;
unsigned long loopTime;
const int pin_A = 1;                // pin 1 impulsatora
const int pin_B = 2;                // pin 2 impulsatora
unsigned char odczyt_A;
unsigned char odczyt_B;
unsigned char odczyt_A_ostatni = 0;
unsigned char odczyt_B_ostatni = 0;
int pwm = 30;                      // ustawienie wartości PWM
int delta = 1;                      // o ile zmniejszyć wartość

const int Led1 = 12;                //przypisanie aliasów do pinów portów
const int Led2 = 11;
const int Led3 = 10;
const int Led4 = A0;
const int SW1 = A1;
const int SW2 = A2;
const int SW3 = A3;
const int SW4 = A4;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);//konfigurowanie linii do których został dołączony LCD
byte st[8] = {                      //tablica znaku stopnia dla wyświetlacza LCD
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11111,
  B11111,
};

void impulsator() {
  lcd.clear();                      //czyszczenie LCD
  odczyt_A = digitalRead(pin_A);    //czytaj stany impulsatora
  odczyt_B = digitalRead(pin_B);
  if ((odczyt_B != odczyt_B_ostatni && odczyt_A == 1) || (odczyt_A != odczyt_A_ostatni && odczyt_B == 0 && odczyt_A == 0)){                           
    if (pwm - delta >= 0)           //zmniejsz wartość, ale nie zejdź poniżej zera
      pwm -= delta;
  }

  if ((odczyt_A != odczyt_A_ostatni && odczyt_B == 1) || (odczyt_B != odczyt_B_ostatni && odczyt_A == 0 && odczyt_B == 0)){
    if (pwm + delta <= 255)        //zwiększ wartość, ale nie przekrocz 255
      pwm += delta;
  }
  odczyt_A_ostatni = odczyt_A;     // zapisz wartość A na następną pętlę
  odczyt_B_ostatni = odczyt_B;     // zapisz wartość B na następną pętlę
  noweznaki();
}

void noweznaki() {
  lcd.setCursor(0, 0);             //ustawienie kursora
  lcd.print("U=");                 //wyświetlenie napisu U=
  lcd.print(pwm);
  lcd.print(" ");                  //wyświetlenie napisu :)
  lcd.write((uint8_t)0);           //wyświetlenie znaku zdefiniowanego 0
  lcd.setCursor(0, 1);             //ustawienie kursora na drugim wierszu, pierwszej kolumnie
  lcd.print("A=");                 //wyświetlenie napisu A=
  lcd.print(odczyt_A);
  lcd.print("B=");                 //wyświetlenie napisu B=
  lcd.print(odczyt_B);
  lcd.print(":)");                 //wyświetlenie znaku :)
}

void sprawdzprzycisk(int SW, int i)
{
  if (digitalRead(SW) == LOW) {
    digitalWrite(Led1, HIGH);      //zaświecenie LED1
    lcd.clear();                   //czyszczenie LCD
    lcd.setCursor(2, 0);           //ustawienie kursora w pierwszym rzędzie i drugiej kolumnie LCD
    lcd.print("S"); lcd.print(i);  //wyświetlenie nazwy przycisku
    while (digitalRead(SW) == LOW);//oczekiwanie na zwolnienie przycisku S1
  } else {                         //w przeciwnym razie
    digitalWrite(Led1, LOW);       //wyłączenie diody LED1
  }
}

void przyciski() {
  sprawdzprzycisk(SW1, 1);         //sprawdzenie czy naciśnięto przycisk S1 
  sprawdzprzycisk(SW2, 2);         //sprawdzenie czy naciśnięto przycisk S2 
  sprawdzprzycisk(SW3, 3);         //sprawdzenie czy naciśnięto przycisk S3 
  sprawdzprzycisk(SW4, 4);         //sprawdzenie czy naciśnięto przycisk S4
}


void setup() {                     //funkcja inicjalizacji
  lcd.begin(8, 2);                 //konfigurowanie rozdzielczości LCD
  lcd.createChar(0, st);           //funkcja utworzenia własnego znaku z tablicy st o kodzie 0
  analogReference(DEFAULT);        //konfigurowanie napięcia odniesienia dla przetwornika A/C - domyślnie 5V
  pinMode(Led1, OUTPUT);           //konfigurowanie I/O, do których są dołączone diody LED
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(Led4, OUTPUT);
  pinMode(SW1, INPUT);             //konfigurowanie I/O, do których są dołączone przyciski
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);
  digitalWrite(SW1, HIGH);         //dołączenie wewnętrznych rezystorów zasilających
  digitalWrite(SW2, HIGH);
  digitalWrite(SW3, HIGH);
  digitalWrite(SW4, HIGH);
  digitalWrite(Led1, LOW);         //wyłączenie diod LED
  digitalWrite(Led2, LOW);
  digitalWrite(Led3, LOW);
  digitalWrite(Led4, LOW);
  //akcja.attach(0, 122, przyciski); // Wątek 1
  //akcja.attach(1, 30, impulsator); // Wątek 2
  loopTime = currentTime;
  pinMode(pin_A, INPUT);
  pinMode(pin_B, INPUT);
  NettigoTimer1.begin(122);
  NettigoTimer2.begin(30);
}

void loop() {                      //pętla główna programu
//  akcja.process();               // inicjalizacja lub aktualizacja wszystkich procedur(wątków, zdarzeń itp.)
  if (NettigoTimer1.available())
  {
    przyciski();
    NettigoTimer1.restart();
  }
  if (NettigoTimer2.available())
  {
    impulsator();
    NettigoTimer2.restart();
  }
}                                  //koniec pętli głównej

