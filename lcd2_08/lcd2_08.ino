/* Simple Car game for a 16x2 LCD display
   You can use any Hitachi HD44780 compatible LCD.
   Wiring explained at http://www.arduino.cc/en/Tutorial/LiquidCrystal
   (I used theLCD Electronic Brick on bus 1:
     rs on pin 2, rw on pin 3, enable on pin 4,
     data on pins 5,6,7,8)
   There's also a "steering wheel" potentiometer on analog input 1,
   and a Piezo speaker on pin 9 (PWM).

   Enjoy,
   @TheRealDod, Nov 25, 2010

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
#include <LiquidCrystal.h>
// LiquidCrystal display
// You can use any Hitachi HD44780 compatible. Wiring explained at
// http://www.arduino.cc/en/Tutorial/LiquidCrystal
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);// ustalenie pinów AVTLCD inne niż powyżej

// Steering wheel potentiometer
const int POTPIN = A0;
const int MAXPOT = 800; 			// no need to turn the wheel all the way to 1023 :)

// Piezo speaker
const int SPEAKERPIN =  3;
const int RANDSEEDPIN = 0; 			// an analog pin that isn't connected to anything
const int MAXSTEPDURATION = 300; 	// Start slowly, each step is 1 millisec shorter.
const int MINSTEPDURATION = 150; 	// This is as fast as it gets
const int Led1 =  13;               //przypisanie aliasów do pinów portów
const int Led2 =  12;
const int Led3 =  11;
const int Led4 =  10;
const int SW1  =  3;
const int SW2  =  2;
const int SW3  =  1;
const int SW4  =  0;
const int Buzzer =  A0;

const int NGLYPHS = 6;
// the glyphs will be defined starting from 1 (not 0),
// to enable lcd.print() of null-terminated strings
byte glyphs[NGLYPHS][8] = {
		// 1: car up
		{ B00000,
				B01110,
				B11111,
				B01010,
				B00000,
				B00000,
				B00000,
				B00000}
		// 2: car down
		,{B00000,
				B00000,
				B00000,
				B00000,
				B01110,
				B11111,
				B01010,
				B00000}
		// 3: truck up
		,{B00000,
				B11110,
				B11111,
				B01010,
				B00000,
				B00000,
				B00000,
				B00000}
		// 4: truck down
		,{B00000,
				B00000,
				B00000,
				B00000,
				B11110,
				B11111,
				B01010,
				B00000}
		// 5: crash up
		,{B10101,
				B01110,
				B01110,
				B10101,
				B00000,
				B00000,
				B00000,
				B00000}
		// 6: crash down
		,{B00000,
				B00000,
				B00000,
				B10101,
				B01110,
				B01110,
				B10101,
				B00000}
};

const int NCARPOSITIONS = 4;

// Each position is mapped to a column of 2 glyphs
// Used to make sense when I had a 5th position
// where car or crash was drawn as 2 glyphs
// (can't do that since 0 terminates strings),
// so it's kinda silly now, but it ain't broke :)
const char BLANK=32;
char car2glyphs[NCARPOSITIONS][2] = {
		{1,BLANK},{2,BLANK},{BLANK,1},{BLANK,2}
};
char truck2glyphs[NCARPOSITIONS][2] = {
		{3,BLANK},{4,BLANK},{BLANK,3},{BLANK,4}
};
char crash2glyphs[NCARPOSITIONS][2] = {
		{5,BLANK},{6,BLANK},{BLANK,5},{BLANK,6}
};

const int ROADLEN = 15; 		// LCD width (not counting our car)
int road[ROADLEN]; 				// positions of other cars
char line_buff[2+ROADLEN]; 		// aux string for drawRoad()
int road_index;
int car_pos;
// Off-the-grid position means empty column, so MAXROADPOS
// determines the probability of a car in a column
// e.g. 3*NCARPOSITIONS gives p=1/3
const int MAXROADPOS = 3*NCARPOSITIONS;
int step_duration;

int crash; 						// true if crashed
unsigned int crashtime; 		// millis() when crashed
const int CRASHSOUNDDURATION = 250;

const char *INTRO1="Trucks ahead,";
const char *INTRO2="Drive carefully";
const int INTRODELAY = 2000;

void setup()
{
	crash = crashtime = road_index = 0;
	step_duration = MAXSTEPDURATION;
	line_buff[1+ROADLEN] = '\0'; // null terminate it
	randomSeed(analogRead(RANDSEEDPIN));
	for (int i=0; i<NGLYPHS; i++) {
		lcd.createChar(i+1,glyphs[i]);
	}
	for (int i=0; i<ROADLEN; i++) {
		road[i]=-1;
	}
	pinMode(SPEAKERPIN,OUTPUT);
	analogWrite(SPEAKERPIN,0); 	// to be on the safe side
	lcd.begin(16,2);
	getSteeringWheel();
	drawRoad();
	lcd.setCursor(1,0);
	lcd.print(INTRO1);
	lcd.setCursor(1,1);
	lcd.print(INTRO2);
	delay(INTRODELAY);
	pinMode(Led1, OUTPUT);    	//Konfiguracja linii do ktorych sa dolaczone diody jako wyjscia
	pinMode(Led2, OUTPUT);
	pinMode(Led3, OUTPUT);
	pinMode(Led4, OUTPUT);
	pinMode(Buzzer, OUTPUT);    	//konfiguracja linii do ktorej jest dolaczony brzeczyk jako wyjscie
	pinMode(SW1, INPUT);        	//konfiguracja linii do ktorych sa dolaczone przyciski jako wejscia
	pinMode(SW2, INPUT);
	pinMode(SW3, INPUT);
	pinMode(SW4, INPUT);
	digitalWrite(SW1, HIGH);   	//dolaczenie do linii do ktorych sa dolaczone przyciski rezystorow podciagajacych co wymusi na nich domyslnie stan wysoki
	digitalWrite(SW2, HIGH);
	digitalWrite(SW3, HIGH);
	digitalWrite(SW4, HIGH);
	digitalWrite(Led1, HIGH);  	//domyslne wylaczenie diod LED
	digitalWrite(Led2, HIGH);
	digitalWrite(Led3, HIGH);
	digitalWrite(Led4, HIGH);
	digitalWrite(Buzzer, HIGH);  	//domyslne wylaczenie brzeczyka
}

void loop() {
	unsigned long now = millis()-INTRODELAY;
	if (!crash) {
		getSteeringWheel();
		crash = (car_pos==road[road_index]);
	}
	if (crash) {
		if (!crashtime) {
			crashtime=now;
			drawRoad();
			// Game over text
			// (keep first 2 "crash" columns intact)
			lcd.setCursor(2,0);
			lcd.print("Crashed after");
			lcd.setCursor(2,1);
			lcd.print(now/1000);
			lcd.print(" seconds.");
		}
		if ((now-crashtime)<CRASHSOUNDDURATION) {
			analogWrite(SPEAKERPIN,random(255)); // white noise
		}
		else {
			analogWrite(SPEAKERPIN,0); 		// dramatic post-crush silence :)
		}
		delay(10); 							// Wait a bit between writes
	}
	else {

		int prev_pos = road[(road_index-1)%ROADLEN];
		int this_pos = random(MAXROADPOS);
		while (abs(this_pos-prev_pos)<2) { 	// don't jam the road
			this_pos = random(MAXROADPOS);
		}
		road[road_index] = this_pos;
		road_index = (road_index+1)%ROADLEN;
		drawRoad();
		delay(step_duration);
		if (step_duration>MINSTEPDURATION) {
			step_duration--; 					// go faster
		}
	}
}
void getSteeringWheel() {
	car_pos = map(analogRead(POTPIN),0,1020,0,NCARPOSITIONS);
}

void drawRoad() {
	for (int i=0; i<2; i++) {
		if (crash) {
			line_buff[0]=crash2glyphs[car_pos][i];
		}
		else {
			line_buff[0]=car2glyphs[car_pos][i];
		}
		for (int j=0; j<ROADLEN; j++) {
			int pos = road[(j+road_index)%ROADLEN];
			line_buff[j+1] = pos>=0 && pos<NCARPOSITIONS ? truck2glyphs[pos][i] : BLANK;
		}
		lcd.setCursor(0,i);
		lcd.print(line_buff);
	}
}
