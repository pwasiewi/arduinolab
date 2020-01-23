/*
  Up to 8 dices on a tiny 80x160px TFT display
  Author: Piotr Wasiewicz <pwasiewi(at)gmail.com>
  based on two dices Alfonso de Cala demo version
  and some ideas from M5StickC_Nixie_tube_Clock
  and FactoryTest Basic M5StickC Example
*/

#include "Timers.h"                 // Timers from Polish forum arduino http://bit.ly/arduinotimer
Timers <2> akcja;

#include <M5StickC.h>
#include "DiceMemoir.c"

const uint8_t*n[] = { Soldier, Grenade, Star, Flag, Tank, Soldier };

float accX = 0;
float accY = 0;
float accZ = 0;
int randint = 1;
int dice_count = 1;
int led_count = 8;
int sleep_count = 0;

void setup(void) {
  Serial.begin(115200);
  M5.begin();
  M5.IMU.Init();
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(TFT_GREEN);
  M5.Lcd.setTextColor(TFT_BLACK);  // Adding a background colour erases previous text automatically
  M5.Lcd.setCursor(10, 30);
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("SHAKE ME");
  M5.Axp.ScreenBreath(led_count);
  M5.IMU.getAccelData(&accX, &accY, &accZ);
  randint = (int)1000 * (accX + accY + accZ);
  randomSeed(micros() + randint % 100);
  pinMode(M5_LED, OUTPUT);
  digitalWrite(M5_LED, HIGH);
  pinMode(M5_BUTTON_HOME, INPUT);
  pinMode(M5_BUTTON_RST, INPUT);
  akcja.attach(0, 100, sensors); // Thread 1
  akcja.attach(1, 200, buttons); // Thread 1
}



void sensors() {
  // Serial.println("Sensors");
  if (M5.Axp.GetWarningLeve()) {
    sleep_count++;
    if (sleep_count >= 1) {
      M5.Lcd.fillScreen(WHITE);
      M5.Lcd.setCursor(0, 20, 1);
      M5.Lcd.setTextColor(RED, WHITE);
      M5.Lcd.printf("Warning: low battery");
      if (sleep_count >= 10) {
        sleep_count = 0;
        M5.Axp.SetSleep();
      }
    }
  } else {
    M5.IMU.getAccelData(&accX, &accY, &accZ);
    if (accX > 1.5 ||  accY > 1.5 ) {
      depict();
    }
  }
}

void buttons() {
  // Serial.println("Buttons");
  if (digitalRead(M5_BUTTON_RST) == LOW) {
    led_count++;
    if (led_count > 12)
      led_count = 7;
    while (digitalRead(M5_BUTTON_RST) == LOW);
    M5.Axp.ScreenBreath(led_count); //screen light control
  }
  if (digitalRead(M5_BUTTON_HOME) == LOW) {
    //M5.IMU.getAccelData(&accX, &accY, &accZ);
    randint = (int) 1000 * (accX + accY + accZ);
    randomSeed(micros() + randint % 100);
    dice_count++;
    if (dice_count > 8)
      dice_count = 1;
    while (digitalRead(M5_BUTTON_HOME) == LOW);
  }
}

void depict() {
  M5.update();
  digitalWrite(M5_LED, LOW);
  M5.Lcd.fillScreen(TFT_RED);
  delay(1500);  // A little delay to increase suspense :-D
  M5.Lcd.fillScreen(TFT_GREEN);
  digitalWrite(M5_LED, HIGH);
  if (dice_count == 1) {
    // Draw first dice
    int number = random(0, 6);
    M5.Lcd.pushImage(62, 22, 36, 36, (uint16_t *)n[number]);
  } else if (dice_count == 2) {
    // Draw first dice
    int number = random(0, 6);
    M5.Lcd.pushImage(22, 22, 36, 36, (uint16_t *)n[number]);
    // Draw second dice
    number = random(0, 6);
    M5.Lcd.pushImage(102, 22, 36, 36, (uint16_t *)n[number]);
  } else {
    for (int i = 1; i <= dice_count; i++) {
      int number = random(0, 6);
      if (dice_count < 5) {
        M5.Lcd.pushImage(2 + (i - 1) * 40, 22, 36, 36, (uint16_t *)n[number]);
      } else if (i < 5)
        M5.Lcd.pushImage(2 + (i - 1) * 40, 2, 36, 36, (uint16_t *)n[number]);
      else
        M5.Lcd.pushImage(2 + (i - 5) * 40, 42, 36, 36, (uint16_t *)n[number]);
    }
  }
}

void loop() {
  akcja.process();
}
