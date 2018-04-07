
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

//#include "Timers.h"               // dołączona biblioteka Timers z forum android http://bit.ly/arduinotimer
//Timers <2> akcja;

//#include <TimerOne.h>             //biblioteka obslugi timera TimerOne https://github.com/PaulStoffregen/TimerOne

#include <Task.h>
#include <DelayRun.h>
#include <Rotary.h>
#include <SoftPwmTask.h>
#include <Heartbeat.h>
#include <Dimmer.h>
#include <BlinkTask.h>
#include <Debouncer.h>
#include <TonePlayer.h>
#include <SoftTimer.h>
#include <FrequencyTask.h>
#include <PciListenerImp2.h>
#include <IPciChangeHandler.h>
#include <PciListener.h>
#include <PciListenerImp.h>
#include <PciManager.h>

#include <FuzzyInput.h>
#include <FuzzyComposition.h>
#include <FuzzyRuleAntecedent.h>
#include <FuzzyOutput.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyIO.h>
#include <FuzzyRule.h>
#include <Fuzzy.h>
#include <FuzzySet.h>
// Step 1 -  Instantiating an object library
Fuzzy* fuzzy = new Fuzzy();

unsigned long currentTime;
unsigned long loopTime;
const int pin_A = 1;                // pin 1 impulsatora
const int pin_B = 2;                // pin 2 impulsatora
unsigned char odczyt_A;
unsigned char odczyt_B;
unsigned char odczyt_A_ostatni = 0;
unsigned char odczyt_B_ostatni = 0;
int pwm = 30;                       // ustawienie wartości PWM
float output = 0;
int delta = 1;                      //o ile zmniejszyć wartość

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

void impulsator(Task* me) {
  lcd.clear();                      //czyszczenie LCD
  odczyt_A = digitalRead(pin_A);    //czytaj stany impulsatora
  odczyt_B = digitalRead(pin_B);
  if ((odczyt_B != odczyt_B_ostatni && odczyt_A == 1) || (odczyt_A != odczyt_A_ostatni && odczyt_B == 0 && odczyt_A == 0)) {
    if (pwm - delta >= 0)           //zmniejsz wartość, ale nie zejdź poniżej zera
      pwm -= delta;
  }

  if ((odczyt_A != odczyt_A_ostatni && odczyt_B == 1) || (odczyt_B != odczyt_B_ostatni && odczyt_A == 0 && odczyt_B == 0)) {
    if (pwm + delta <= 255)        //zwiększ wartość, ale nie przekrocz 255
      pwm += delta;
  }
  odczyt_A_ostatni = odczyt_A;     // zapisz wartość A na następną pętlę
  odczyt_B_ostatni = odczyt_B;     // zapisz wartość B na następną pętlę
  fuzzytask();
  noweznaki();
}

void noweznaki() {
  lcd.setCursor(0, 0);             //ustawienie kursora
  lcd.print("U=");                 //wyświetlenie napisu U=
  lcd.print(pwm);
  lcd.print(" ");                  //wyświetlenie napisu :)
  lcd.write((uint8_t)0);           //wyświetlenie znaku zdefiniowanego 0
  lcd.setCursor(0, 1);             //ustawienie kursora na drugim wierszu, pierwszej kolumnie
  lcd.print("OUT=");                 //wyświetlenie napisu A=
  lcd.print(output);
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



void przyciski(Task* me) {
  sprawdzprzycisk(SW1, 1);         //sprawdzenie czy naciśnięto przycisk S1
  sprawdzprzycisk(SW2, 2);         //sprawdzenie czy naciśnięto przycisk S2
  sprawdzprzycisk(SW3, 3);         //sprawdzenie czy naciśnięto przycisk S3
  sprawdzprzycisk(SW4, 4);         //sprawdzenie czy naciśnięto przycisk S4
}

void fuzzytask() {
  // Step 5 - Report inputs value, passing its ID and value
  fuzzy->setInput(1, pwm);
  // Step 6 - Exe the fuzzification
  fuzzy->fuzzify();
  // Step 7 - Exe the desfuzzyficação for each output, passing its ID
  output = fuzzy->defuzzify(1);
}

Task t1(30, impulsator);
Task t2(122, przyciski);

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

  // Step 2 - Creating a FuzzyInput distance
  FuzzyInput* distance = new FuzzyInput(1);         // With its ID in param

  // Creating the FuzzySet to compond FuzzyInput distance
  FuzzySet* small = new FuzzySet(0, 20, 20, 40);    // Small distance
  distance->addFuzzySet(small); // Add FuzzySet small to distance
  FuzzySet* safe = new FuzzySet(30, 50, 50, 70);    // Safe distance
  distance->addFuzzySet(safe); // Add FuzzySet safe to distance
  FuzzySet* big = new FuzzySet(60, 80, 80, 80);     // Big distance
  distance->addFuzzySet(big); // Add FuzzySet big to distance

  fuzzy->addFuzzyInput(distance); // Add FuzzyInput to Fuzzy object

  // Rule 3 - Creating FuzzyOutput velocity
  FuzzyOutput* velocity = new FuzzyOutput(1);       // With its ID in param

  // Creating FuzzySet to compond FuzzyOutput velocity
  FuzzySet* slow = new FuzzySet(0, 10, 10, 20);     // Slow velocity
  velocity->addFuzzySet(slow);                      // Add FuzzySet slow to velocity
  FuzzySet* average = new FuzzySet(10, 20, 30, 40); // Average velocity
  velocity->addFuzzySet(average);                   // Add FuzzySet average to velocity
  FuzzySet* fast = new FuzzySet(30, 40, 40, 50);    // Fast velocity
  velocity->addFuzzySet(fast);                      // Add FuzzySet fast to velocity

  fuzzy->addFuzzyOutput(velocity);                  // Add FuzzyOutput to Fuzzy object

  //Rule 4 - Assembly the Fuzzy rules
  // FuzzyRule "IF distance = samll THEN velocity = slow"
  FuzzyRuleAntecedent* ifDistanceSmall = new FuzzyRuleAntecedent(); // Instantiating an Antecedent to expression
  ifDistanceSmall->joinSingle(small);                               // Adding corresponding FuzzySet to Antecedent object
  FuzzyRuleConsequent* thenVelocitySlow = new FuzzyRuleConsequent();// Instantiating a Consequent to expression
  thenVelocitySlow->addOutput(slow);                                // Adding corresponding FuzzySet to Consequent object
  // Instantiating a FuzzyRule object
  FuzzyRule* fuzzyRule01 = new FuzzyRule(1, ifDistanceSmall, thenVelocitySlow); // Passing the Antecedent and the Consequent of expression

  fuzzy->addFuzzyRule(fuzzyRule01);                  // Adding FuzzyRule to Fuzzy object

  // FuzzyRule "IF distance = safe THEN velocity = normal"
  FuzzyRuleAntecedent* ifDistanceSafe = new FuzzyRuleAntecedent();   // Instantiating an Antecedent to expression
  ifDistanceSafe->joinSingle(safe);                  // Adding corresponding FuzzySet to Antecedent object
  FuzzyRuleConsequent* thenVelocityAverage = new FuzzyRuleConsequent(); // Instantiating a Consequent to expression
  thenVelocityAverage->addOutput(average);           // Adding corresponding FuzzySet to Consequent object
  // Instantiating a FuzzyRule object
  FuzzyRule* fuzzyRule02 = new FuzzyRule(2, ifDistanceSafe, thenVelocityAverage); // Passing the Antecedent and the Consequent of expression

  fuzzy->addFuzzyRule(fuzzyRule02);                  // Adding FuzzyRule to Fuzzy object

  // FuzzyRule "IF distance = big THEN velocity = fast"
  FuzzyRuleAntecedent* ifDistanceBig = new FuzzyRuleAntecedent();       // Instantiating an Antecedent to expression
  ifDistanceBig->joinSingle(big); // Adding corresponding FuzzySet to Antecedent object
  FuzzyRuleConsequent* thenVelocityFast = new FuzzyRuleConsequent();    // Instantiating a Consequent to expression
  thenVelocityFast->addOutput(fast);// Adding corresponding FuzzySet to Consequent object
  // Instantiating a FuzzyRule object
  FuzzyRule* fuzzyRule03 = new FuzzyRule(3, ifDistanceBig, thenVelocityFast); // Passing the Antecedent and the Consequent of expression

  fuzzy->addFuzzyRule(fuzzyRule03);                                     // Adding FuzzyRule to Fuzzy object

  //Timer1.initialize(30000);             //inicjalizacja timer 1 - przerwanie co 100 ms
  //Timer1.attachInterrupt(impulsator);   //uruchomienie przerwania od timer 1 w procedurze on_A
  //attachInterrupt(pin_A, on_A, RISING);
  //attachInterrupt(pin_B, on_B, RISING);
  SoftTimer.add(&t1);
  SoftTimer.add(&t2);
}


