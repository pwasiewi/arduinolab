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
Przykład programu do obsługi modułu AVT1722 miniLCD z:
- wyświetlaczem LCD 2x8 znaków
- diodą LED RGB G 10, R 11, B 12 (kolor podłączenie arduino)
- 4 przyciskami A1,A2,A3,A4
- przekaźnik A0
- impulsator 0 1 2
*/
#include <LiquidCrystal.h> //biblioteka obsługi LCD
const int Led1 = 12; //przypisanie aliasów do pinów portów
const int Led2 = 11;
const int Led3 = 10;
const int Led4 = A0;
const int SW1 = A1;
const int SW2 = A2;
const int SW3 = A3;
const int SW4 = A4;
const int Buzzer = 13;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //konfigurowanie linii do których został dołączony LCD
int wart_pot; //zmienna na wartość zmierzona z potencjometru
int wart_czuj; //zmienna na wartość zmierzonej z czujnika temperatury
float wart_nap; //zmienna na wartość zmierzonego napięcia
float temperatura; //zmienna na wartość zmierzonej temperatury
byte st[8] = { //tablica znaku stopnia dla wyświetlacza LCD
  B00100,
  B01010,
  B00100,
  B00000,
  B00000,
  B00000,
  B00000,
};
void setup() { //funkcja inicjalizacji
  lcd.begin(8, 2); //konfigurowanie rozdzielczości LCD
  lcd.createChar(0, st); //funkcja utworzenia własnego znaku z tablicy st o kodzie 0
  analogReference(DEFAULT); //konfigurowanie napięcia odniesienia
  //dla przetwornika A/C - domyślnie 5V
  pinMode(Led1, OUTPUT); //konfigurowanie I/O, do których są
  //dołączone diody LED
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(Led4, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  //pinMode(Buzzer, OUTPUT); //konfigurowanie I/O, do której jest
  //dołączony brzęczyk piezzo
  pinMode(SW1, INPUT); //konfigurowanie I/O, do których są
  //dołączone przyciski
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);
  digitalWrite(SW1, HIGH); //dołączenie wewnętrznych rezystorów
  //zasilających
  digitalWrite(SW2, HIGH);
  digitalWrite(SW3, HIGH);
  digitalWrite(SW4, HIGH);
  digitalWrite(Led1, LOW); //wyłączenie diod LED
  digitalWrite(Led2, LOW);
  digitalWrite(Led3, LOW);
  digitalWrite(Led4, LOW);
  //digitalWrite(Buzzer, HIGH); //wyłączenie brzęczyka piezzo


}
void loop() { //pętla główna programu
  lcd.clear(); //czyszczenie LCD
  lcd.setCursor(0, 0); //ustawienie kursora w 5 kolumnie
  //pierwszego wiersza
  //lcd.print("ArduPW"); //wyświetlenie na LCD napisu Arduino
  //pomiar napięcia z potencjometru i dodanie wyniku do wart_pot

  wart_pot = digitalRead(0);

  //ustawienie kursora w pierwszej pozycji drugiego wiersza LCD

  lcd.print("U="); //wyświetlenie napisu U=
  lcd.print(wart_pot); //wyświetlenie napięcia
  lcd.print("V"); //wyświetlenie znaku V

  lcd.setCursor(0, 1);

  //pomiar napięcia z czujnika temperatury i dodawanie wart_czuj
  wart_czuj = digitalRead(2);

  //ustawienie kursora na pozycji 9 drugiego wiersza LCD lcd.setCursor(9, 1);
  lcd.print("T="); //wyświetlenie napisu T=
  lcd.print(wart_czuj);
  //wyświetlenie wartości temperatury zaokrąglonej do pełnych stopni
  //lcd.write(0); //wyświetlenie znaku stopnia
  lcd.print("C"); //wyświetlenie znaku C
  //sprawdzenie czy naciśnięto przycisk S1
  if (digitalRead(SW1) == LOW) {
    digitalWrite(Led1, HIGH); //zaświecenie LED1
    digitalWrite(Buzzer, HIGH); //włączenie brzęczyka
    lcd.clear(); //czyszczenie LCD
    //ustawienie kursora w pierwszym rzędzie i drugiej kolumnie lcd.
    lcd.setCursor(2, 0);-
    lcd.print("S1"); //wyświetlenie nazwy przycisku
    //oczekiwanie na zwolnienie przycisku S1
    while (digitalRead(SW1) == LOW);
  } else { //w przeciwnym razie
    digitalWrite(Led1, LOW); //wyłączenie diody LED1
    digitalWrite(Buzzer, LOW); //wyłączenie brzęczyka
  }
  if (digitalRead(SW2) == LOW) { //sprawdzenie czy naciśnięto S2
    digitalWrite(Led2, HIGH);
    digitalWrite(Buzzer, HIGH);
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("S2");
    while (digitalRead(SW2) == LOW);
  } else {
    digitalWrite(Led2, LOW);
    digitalWrite(Buzzer, LOW);
  }
  if (digitalRead(SW3) == LOW) { //sprawdzenie czy naciśnięto S3
    digitalWrite(Led3, HIGH);
    digitalWrite(Buzzer, HIGH);
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("S3");
    while (digitalRead(SW3) == LOW);
  } else {
    digitalWrite(Led3, LOW);
    digitalWrite(Buzzer, LOW);
  }
  if (digitalRead(SW4) == LOW) { //sprawdzenie czy naciśnięto S4
    digitalWrite(Led4, HIGH);
    digitalWrite(Buzzer, HIGH);
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("S4");
    while (digitalRead(SW4) == LOW);
  } else {
    digitalWrite(Led4, LOW);
    digitalWrite(Buzzer, LOW);
  }

  delay(300); //opóźnienie o 300ms
} //koniec pętli głównej

