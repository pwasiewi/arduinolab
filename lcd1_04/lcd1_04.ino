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
- impulsator 1 2 (przycisk w gałce 0)
*/
#include <LiquidCrystal.h> //biblioteka obsługi LCD

#include "TimerOne.h"  //biblioteka obslugi timera1

const int Led1 = 12; //niebieski
const int Led2 = 11; //czerwony
const int Led3 = 10; //zielony
const int Led4 = A0;
const int SW1 = A1;
const int SW2 = A2;
const int SW3 = A3;
const int SW4 = A4;
const int Buzzer = 13;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //konfigurowanie linii do których został dołączony LCD

byte st[8] = { //tablica znaku stopnia dla wyświetlacza LCD
		B00100,
		B01010,
		B00100,
		B00000,
		B00100,
		B01010,
		B00100,
};
byte st1[8] = {
		0b01010,
		0b11011,
		0b00100,
		0b11011,
		0b01010,
		0b00100,
		0b01110,
		0b00100
};
int pwm = 120;   // ustawienie wartości PWM
int delta = 10;    // o ile zmniejszyć wartość
unsigned long currentTime;
unsigned long loopTime;
const int pin_A = 1;  // pin 1
const int pin_B = 2;  // pin 2
unsigned char odczyt_A;
unsigned char odczyt_B;
unsigned char odczyt_A_ostatni=0;
unsigned char odczyt_B_ostatni=0;
void setup() { //funkcja inicjalizacji
	lcd.begin(8, 2); //konfigurowanie rozdzielczości LCD
	lcd.createChar(0, st); //funkcja utworzenia własnego znaku z tablicy st o kodzie 0
	lcd.createChar(1, st1);
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
	pinMode(Led1, OUTPUT);
	pinMode(pin_A, INPUT);
	pinMode(pin_B, INPUT);
	currentTime = millis();
	loopTime = currentTime;
	Serial.begin(115200);
	Timer1.initialize(100000);      //inicjalizacja timer 1 - przerwanie co 100 ms
	Timer1.attachInterrupt(on_A);   //uruchomienie przerwania od timer 1 w procedurze on_A
	//attachInterrupt(pin_A, on_A, RISING);
	//attachInterrupt(pin_B, on_B, RISING);
}

void on_A()      //procedura obslugi przerwania zewnetrznego
{
	//Serial.println("A w gore");
	//odczyt_A = 1;
	odczyt_A = digitalRead(pin_A); // czytaj stany impulsatora
	odczyt_B = digitalRead(pin_B);
	if(odczyt_B) {
		// B jest wysoko to zwiększ wartość
		// ale nie przekrocz 255
		if(pwm + delta <= 255) pwm += delta;
	}
	else {
		// B jest nisko to zmniejsz wartość
		// ale nie zejdź poniżej zera
		if(pwm - delta >= 0) pwm -= delta;
	}
	if(!odczyt_A) {
		// A jest nisko to zwiększ wartość
		// ale nie przekrocz 255
		if(pwm + delta <= 255) pwm += delta;
	}
	else {
		// A jest wysoko to zmniejsz wartość
		// ale nie zejdź poniżej zera
		if(pwm - delta >= 0) pwm -= delta;
	}
}


void loop() { //pętla główna programu
	lcd.clear(); //czyszczenie LCD
	lcd.setCursor(0, 0); //ustawienie kursora

	lcd.print("U="); //wyświetlenie napisu U=

	odczyt_A = digitalRead(pin_A);    // czytaj stany impulsatora
    odczyt_B = digitalRead(pin_B);

	// ustaw błyski diody led1 niebieski
	analogWrite(Led1, pwm);
    lcd.print(pwm); //wyświetlenie napięcia

	lcd.write((uint8_t)1);
	lcd.print(":)"); //wyświetlenie napisu :)

	//lcd.print(loopTime); //wyświetlenie
	lcd.print("V"); //wyświetlenie znaku V

	lcd.setCursor(0, 1);
	//ustawienie kursora na drugim wierszu, pierwszej kolumnie
	lcd.print("A="); //wyświetlenie napisu A=
	lcd.print(odczyt_A);
	lcd.print("B="); //wyświetlenie napisu B=
	lcd.print(odczyt_B);
	//wyświetlenie wartości temperatury zaokrąglonej do pełnych stopni
	lcd.write((uint8_t)0); //wyświetlenie znaku zdefiniowanego 0
	lcd.print(")"); //wyświetlenie znaku )
	//sprawdzenie czy naciśnięto przycisk S1
	if (digitalRead(SW1) == LOW) {
		digitalWrite(Led1, HIGH); //zaświecenie LED1
		digitalWrite(Buzzer, HIGH); //włączenie brzęczyka
		lcd.clear(); //czyszczenie LCD
		//ustawienie kursora w pierwszym rzędzie i drugiej kolumnie lcd.
		lcd.setCursor(2, 0);
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

	delay(50); //opóźnienie o 50ms
} //koniec pętli głównej

