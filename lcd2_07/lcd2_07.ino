/*
  Projekt: Gra 'Alderaan Trip' w oparciu o platformę Arduino z wykorzystaniem wyświetlacza LCD 16x2
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

#include <LiquidCrystal.h> 				// załadowanie biblioteki LCD
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); 	// ustalenie pinów LCD



// zdefiniowanie wysokości dźwięków
#define NOTE_C4  262
#define NOTE_C4_1 260
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523




const int RANDSEEDPIN     = 0; 	    // ustalenie pinu losowego ziarna
const int MAXSTEPDURATION = 500; 	// maksymalny czas pomiędzy 'klatkami'
const int MINSTEPDURATION = 150; 	// minimalny czas pomiędzy 'klatkami'
const int Led1 =  13;               //przypisanie aliasów do pinów portów
const int Led2 =  12;
const int Led3 =  11;
const int Led4 =  10;
const int SW1  =  3;
const int SW2  =  2;
const int SW3  =  1;
const int SW4  =  0;
const int Buzzer =  A0;
int adc_key_in = 0;                 // deklaracja zmiennej wciśniętego klawisza

const int Nsymbols = 6; 			// ilość zdefiniowanych symboli lcd
byte symbols[Nsymbols][8] = { 		// mapa 6 narysowanych symboli
		{		B00000,
				B11100,
				B01111,
				B11100,
				B00000,
				B00000,
				B00000,
				B00000} // statek na górze linii
		,{	B00000,
				B00000,
				B00000,
				B00000,
				B11100,
				B01111,
				B11100,
				B00000} // statek na dole linii
		,{	B00000,
				B01110,
				B10111,
				B01110,
				B00000,
				B00000,
				B00000,
				B00000} // gwiazda na górze linii
		,{	B00000,
				B00000,
				B00000,
				B00000,
				B01110,
				B11101,
				B01110,
				B00000} // gwiazda na dole linii
		,{	B10101,
				B01110,
				B01110,
				B10101,
				B00000,
				B00000,
				B00000,
				B00000} // kolizja na górze linii
		,{	B00000,
				B00000,
				B00000,
				B10101,
				B01110,
				B01110,
				B10101,
				B00000} // kolizja na dole linii
};


const int  NxwingPOSITIONS = 4; 			// ilość pozycji statku
const char BLANK=32; 						// pusty znak
char xwing2symbols[NxwingPOSITIONS][2] = { 	// mapa pozycji statków
		{1,BLANK},{2,BLANK},{BLANK,1},{BLANK,2}
};
char star2symbols[NxwingPOSITIONS][2] = { 	// mapa pozycji gwiazd
		{3,BLANK},{4,BLANK},{BLANK,3},{BLANK,4}
};
char crash2symbols[NxwingPOSITIONS][2] = { 	// mapa pozycji kolizji
		{5,BLANK},{6,BLANK},{BLANK,5},{BLANK,6}
};

const int SPACELEN = 15; 					// długość dostępnego ekranu - poza pierwszą kolumną, w której jest statek.
int       SPACE[SPACELEN]; 				    // tablica na mapę gwiazd
char      line_buff[2+SPACELEN]; 			// bufor linii
const int MAXSPACEPOS = 3*NxwingPOSITIONS; 	// prawdopodobieństwo wystąpnienia gwiazdy w kolumnie (1/3)
int       SPACE_index;
int       xwing_pos;
int       step_duration;
int       crash;
unsigned int crashtime;
const int CRASHSOUNDDURATION = 250; 		// czas odgrywania dźwięku zderzenia

// zdefiniowanie tekstu intro
const char *INTRO1=" Alderaan trip ";
const char *INTRO2="    by VILQ    ";
const char *INTRO3="Move up/down to";
const char *INTRO4="avoid collision";

// dźwięki melodii intro
// int melody[] = { NOTE_C4,   NOTE_G4,   NOTE_F4,   NOTE_E4,   NOTE_D4,   NOTE_C5,   NOTE_G4,   NOTE_F4,   NOTE_E4,   NOTE_D4,   NOTE_C5,   NOTE_G4,   NOTE_F4,   NOTE_E4,   NOTE_F4,   NOTE_D4,   NOTE_D4, };
int melody[] = { NOTE_C4,   NOTE_G4,   NOTE_F4,   NOTE_E4,   NOTE_D4,   NOTE_C5,   NOTE_G4,   NOTE_F4,   NOTE_E4,   NOTE_D4,   NOTE_C5,   NOTE_G4,   NOTE_F4,   NOTE_E4,   NOTE_F4,   NOTE_D4,   NOTE_D4, };
// długość nut
//int noteDurations[] = {    1,    2,    7,    7,    7,    1,    2,    7,    7,    7,    1,    2,    7,    7,    7,    1,    1,    };
int noteDurations[] = {    1,    2,    7,    7,    7,    1,    2,    7,    7,    7,    1,    2,    7,    7,    7,    1,    1,    };

int mute; // dźwięk w trakcie gry on/off
int moveUp()   { if (xwing_pos > 0) { xwing_pos--; } else xwing_pos = 0; } // przesunięcie statku o jedną pozycję do góry
int moveDown() { if (xwing_pos < 3) { xwing_pos++; } else xwing_pos = 3; } // przesunięcie statku o jedną pozycję do dołu

int read_LCD_buttons() // zczytanie wciśniętego klawisza z modułu wyświetlacza na podstawie adresu
{
	if (digitalRead(SW1) == LOW || digitalRead(SW2) == LOW) {         //Sprawdzenie czy nacisniety przycisk S1
		moveDown(); return 0;
	}

	if (digitalRead(SW3) == LOW || digitalRead(SW4) == LOW) {         //Sprawdzenie czy nacisniety przycisk S1
		moveUp(); return 0;
	}
}

void play_melody(){ // odegranie melodii
	for (int thisNote = 0; thisNote < 16; thisNote++) {
		int noteDuration = 800/noteDurations[thisNote];
		tone(13, melody[thisNote],noteDuration);
		int pauseBetweenNotes = noteDuration + 50;
		delay(pauseBetweenNotes);
		noTone(9);
	}
}

void drawSPACE() { // 'wyrysowanie' mapy
	for (int i=0; i<2; i++) {
		if (crash) {
			line_buff[0]=crash2symbols[xwing_pos][i];
		} else {
			line_buff[0]=xwing2symbols[xwing_pos][i];
		}
		for (int j=0; j<SPACELEN; j++) {
			int pos = SPACE[(j+SPACE_index)%SPACELEN];
			line_buff[j+1] = pos>=0 && pos<NxwingPOSITIONS ? star2symbols[pos][i] : BLANK;
		}
		lcd.setCursor(0,i);
		lcd.print(line_buff);
	}
}

void setup() // konfiguracja wstępna
{
	crash = crashtime = SPACE_index = mute = 0; 	// wyzerowanie zmiennych
	step_duration = MAXSTEPDURATION; 				// ustalenie prędkości początkowej
	line_buff[1+SPACELEN] = '\0'; 					// ustalenie końca buforu
	randomSeed(analogRead(RANDSEEDPIN)); 			// randomizacja
	for (int i=0; i<Nsymbols; i++) { lcd.createChar(i+1,symbols[i]); }
	for (int i=0; i<SPACELEN; i++) { SPACE[i]=-1; }
	pinMode(Buzzer,OUTPUT); 					    // zdefiniowanie pinu głośnika piezo
	analogWrite(Buzzer,0); 					    	// ustawienie głośności na 0
	lcd.begin(16,2); 								// 'konstruktor' wyświetlacza LCD o 16 kolumnach i 2 wierszach
	drawSPACE(); 									// 'wyrysowanie' mapy
	lcd.setCursor(1,0);  lcd.print(INTRO1);  lcd.setCursor(1,1);  lcd.print(INTRO2);  play_melody();  // pierwszy ekran intro + pierwsze odegranie melodii
	delay(200); 									// opóźnienie pomiędzy ekranami
	lcd.setCursor(1,0);  lcd.print(INTRO3);  lcd.setCursor(1,1);  lcd.print(INTRO4);   play_melody(); // drugi ekran intro + drugie odegranie melodii
	//  Serial.begin(57600);    // konfiguracja portu szeregowego do zczytywania logów przez komputer
	pinMode(Led1, OUTPUT);      //Konfiguracja linii do ktorych sa dolaczone diody jako wyjscia
	pinMode(Led2, OUTPUT);
	pinMode(Led3, OUTPUT);
	pinMode(Led4, OUTPUT);
	pinMode(Buzzer, OUTPUT);    //konfiguracja linii do ktorej jest dolaczony brzeczyk jako wyjscie
	pinMode(SW1, INPUT);        //konfiguracja linii do ktorych sa dolaczone przyciski jako wejscia
	pinMode(SW2, INPUT);
	pinMode(SW3, INPUT);
	pinMode(SW4, INPUT);
	digitalWrite(SW1, HIGH);    //dolaczenie do linii do ktorych sa dolaczone przyciski rezystorow podciagajacych co wymusi na nich domyslnie stan wysoki
	digitalWrite(SW2, HIGH);
	digitalWrite(SW3, HIGH);
	digitalWrite(SW4, HIGH);
	digitalWrite(Led1, HIGH);   //domyslne wylaczenie diod LED
	digitalWrite(Led2, HIGH);
	digitalWrite(Led3, HIGH);
	digitalWrite(Led4, HIGH);
	digitalWrite(Buzzer, HIGH); //domyslne wylaczenie brzeczyka
}

void loop() {
	unsigned long now = millis()-12000; 	// ustawienie czasu startu z uwzględnieniem czasu trwania intro
	if (!crash) { 							// warunki normalne:
		read_LCD_buttons(); 				// zczytanie klawisza + odpowiedni ruch statku, jeśli wystąpił
		if (!mute) tone(13,100+(SPACE_index % 3)*300,10); 	// odegranie dźwięku 'trasy'
		crash = (xwing_pos==SPACE[SPACE_index]); 			// sprawdzenie czy doszło do zderzenia
	}
	if (crash) { 							// warunki zderzenia:
		if (!crashtime) { 					// nieznany czas zderzenia
			crashtime=now; 					// ustalenie czasu
			drawSPACE(); 					// 'wyrysowanie' mapy
			lcd.setCursor(2,0); 			// ustawienie kursora na początku ekranu
			lcd.print("You flew only"); 	// wyswietlenie informacji o osiągniętym wyniku ->
			lcd.setCursor(2,1);
			lcd.print(now/1000);
			lcd.print(" parsecs");
		}
		if ((now-crashtime)<CRASHSOUNDDURATION) { // przy znanym czasie zderzenia, ale czasie krótszym niż czas odgrywania dźwięku zderzenia
			analogWrite(Buzzer,random(255)); 	// odegranie 'dźwięku' zderzenia
		}
		else {
			analogWrite(Buzzer,0); 				// wyciszenie dźwięków
		}
		delay(10);
	}
	else {

		int prev_pos = SPACE[(SPACE_index-1)%SPACELEN]; // ustalenie indeksu ostatniej pozycji
		int this_pos = random(MAXSPACEPOS); // losowanie gwiazdy na ostatniej pozycji ->
		while (abs(this_pos-prev_pos)<2) {  // <- do czasu znalezienia pozycji z zakresu
			this_pos = random(MAXSPACEPOS);
		}
		SPACE[SPACE_index] = this_pos; 		// ustawienie wylosowanej wartości na końcu tabeli
		SPACE_index = (SPACE_index+1)%SPACELEN; // przesunięcie mapy o 1 pozycje w lewo
		drawSPACE(); 						// 'wyrysowanie' mapy
		delay(step_duration); 				// opóźnienie kolejnego kroku
		if (step_duration>MINSTEPDURATION) {// zmniejszenie opóźnienia o 1, aż do osiągnięcia minimalnego czasu międzi klatkami
			step_duration--;
		}
	}
}
