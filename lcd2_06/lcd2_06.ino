/*
  Przykład obsługi menu
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
#include <Fsm.h>                    //obsługa przełączania stanów https://github.com/jonblack/arduino-fsm

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

// This example shows how two finite state machines can be used to simulate
// multitasking on an arduino. Two LED's are turned on and off at irregular
// intervals; the finite state machines take care of the transitions.

void on_led1_on_enter() {
  lcd.setCursor(0, 1);
  lcd.print("led1_on_enter");
  digitalWrite(Led1, LOW);
}

void on_led1_off_enter() {
  lcd.setCursor(0, 1);
  lcd.print("led1_off_enter");
  digitalWrite(Led1, HIGH);
}

void on_led2_on_enter() {
  lcd.setCursor(0, 1);
  lcd.print("led2_on_enter");
  digitalWrite(Led2, LOW);
}

void on_led2_off_enter() {
  lcd.setCursor(0, 1);
  lcd.print("led2_off_enter");
  digitalWrite(Led2, HIGH);
}

void on_led3_on_enter() {
  lcd.setCursor(0, 1);
  lcd.print("led3_on_enter");
  digitalWrite(Led3, LOW);
}

void on_led3_off_enter() {
  lcd.setCursor(0, 1);
  lcd.print("led3_off_enter");
  digitalWrite(Led3, HIGH);
}

void on_led4_on_enter() {
  lcd.setCursor(0, 1);
  lcd.print("led4_on_enter");
  digitalWrite(Led4, LOW);
}

void on_led4_off_enter() {
  lcd.setCursor(0, 1);
  lcd.print("led4_off_enter");
  digitalWrite(Led4, HIGH);
}

State state_led1_on(&on_led1_on_enter, NULL, NULL);
State state_led1_off(&on_led1_off_enter, NULL, NULL);

State state_led2_on(&on_led2_on_enter, NULL, NULL);
State state_led2_off(&on_led2_off_enter, NULL, NULL);

State state_led3_on(&on_led3_on_enter, NULL, NULL);
State state_led3_off(&on_led3_off_enter, NULL, NULL);

State state_led4_on(&on_led4_on_enter, NULL, NULL);
State state_led4_off(&on_led4_off_enter, NULL, NULL);

Fsm fsm_led1(&state_led1_off);
Fsm fsm_led2(&state_led2_off);
Fsm fsm_led3(&state_led3_off);
Fsm fsm_led4(&state_led4_off);

void setup() {
  lcd.begin(16, 2);                //konfigurowanie rozdzielczości LCD
  lcd.setCursor(0, 0);
  lcd.print("Stany:");

  pinMode(Led1, OUTPUT);
  pinMode(Led2, OUTPUT);
  pinMode(Led3, OUTPUT);
  pinMode(Led4, OUTPUT);

  fsm_led1.add_timed_transition(&state_led1_off, &state_led1_on, 1200, NULL);
  fsm_led1.add_timed_transition(&state_led1_on, &state_led1_off, 3100, NULL);
  fsm_led2.add_timed_transition(&state_led2_off, &state_led2_on, 1700, NULL);
  fsm_led2.add_timed_transition(&state_led2_on, &state_led2_off, 2100, NULL);
  fsm_led3.add_timed_transition(&state_led3_off, &state_led3_on, 1400, NULL);
  fsm_led3.add_timed_transition(&state_led3_on, &state_led3_off, 2300, NULL);
  fsm_led4.add_timed_transition(&state_led4_off, &state_led4_on, 4700, NULL);
  fsm_led4.add_timed_transition(&state_led4_on, &state_led4_off, 7900, NULL);
  akcja.attach(0, 200, stany); // Wątek 1

}

void stany() {
  fsm_led1.run_machine();
  fsm_led2.run_machine();
  fsm_led3.run_machine();
  fsm_led4.run_machine();
}

void loop() {                      //pętla główna programu
  akcja.process();                 //inicjalizacja lub aktualizacja wszystkich procedur(wątków, zdarzeń itp.)
}                                  //koniec pętli głównej


