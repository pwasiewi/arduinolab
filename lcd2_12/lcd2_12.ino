 /*
  Przykład obsługi menu
  https://starter-kit.nettigo.pl/2017/04/menu-wyswietlaczu-16x2/
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

#include <LiquidCrystal.h>              //biblioteka obsługi LCD

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);    //konfiguracja linii do ktorych został dołaczony LCD

const int SW1 = 3;
const int SW2 = 2;
const int SW3 = 1;
const int SW4 = 0;
#define BTN_BACK  3
#define BTN_NEXT  0
#define BTN_PREV  1
#define BTN_OK    2

typedef struct {
  String label;
  int minVal;
  int maxVal;
  int currentVal;
  void (*handler)();
} STRUCT_MENUPOS;

typedef enum {
  BACK, NEXT, PREV, OK, NONE
} ENUM_BUTTON;


STRUCT_MENUPOS menu[5];

int currentMenuPos = 0;
int menuSize;
bool isInLowerLevel = false;
int tempVal;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  //lcd.setBacklight(255);

  pinMode(BTN_NEXT, INPUT_PULLUP);
  pinMode(BTN_PREV, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);

  menu[0] = {"Cyfry", 0, 9, 5, NULL};
  menu[1] = {"Liczby", 10, 1000, 15, NULL};
  menu[2] = {"Napisy", 0, 2, 0, formatNapisy};
  menu[3] = {"Ulamki", 0, 30, 15, formatUlamki};
  menu[4] = {"Port szer.", 0, 0, 0, actionPortSzeregowy};

  menuSize = sizeof(menu) / sizeof(STRUCT_MENUPOS);
}

/* Funkcje-uchwyty użytkownika */
void actionPortSzeregowy() {
  Serial.println("Wywolano akcje: Port szeregowy");
}

void formatNapisy() {
  String dictonary[] = {"Napis 1", "Napis 2", "Napis 3 :)"};
  lcd.print(dictonary[tempVal]);
}

void formatUlamki() {
  lcd.print(tempVal / 10.0);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//Funkcje biblioteki menu

void loop() {
  drawMenu();
}

ENUM_BUTTON getButton() {
  if (!digitalRead(BTN_BACK)) return BACK;
  if (!digitalRead(BTN_NEXT)) return NEXT;
  if (!digitalRead(BTN_PREV)) return PREV;
  if (!digitalRead(BTN_OK)) return OK;

  return NONE;
}

void drawMenu() {
  static unsigned long lastRead = 0;
  static ENUM_BUTTON lastPressedButton = OK;
  static unsigned int isPressedSince = 0;
  int autoSwitchTime = 500;

  ENUM_BUTTON pressedButton = getButton();

  if (pressedButton == NONE && lastRead != 0) {
    isPressedSince = 0;
    return;
  }
  if (pressedButton != lastPressedButton) {
    isPressedSince = 0;
  }

  if (isPressedSince > 3) autoSwitchTime = 70;
  if (lastRead != 0 && millis() - lastRead < autoSwitchTime && pressedButton == lastPressedButton) return;

  isPressedSince++;
  lastRead = millis();
  lastPressedButton = pressedButton;

  switch (pressedButton) {
    case NEXT: handleNext(); break;
    case PREV: handlePrev(); break;
    case BACK: handleBack(); break;
    case OK: handleOk(); break;
  }

  lcd.home();
  lcd.clear();
  if (isInLowerLevel) {
    lcd.print(menu[currentMenuPos].label);
    lcd.setCursor(0, 1);
    lcd.print(F("> "));

    if (menu[currentMenuPos].handler != NULL) {
      (*(menu[currentMenuPos].handler))();
    } else {
      lcd.print(tempVal);
    }
  } else {
    lcd.print(F("Menu glowne"));
    lcd.setCursor(0, 1);
    lcd.print(F("> "));

    lcd.print(menu[currentMenuPos].label);
  }
}

void handleNext() {
  if (isInLowerLevel) {
    tempVal++;
    if (tempVal > menu[currentMenuPos].maxVal) tempVal = menu[currentMenuPos].maxVal;
  } else {
    currentMenuPos = (currentMenuPos + 1) % menuSize;
  }
}

void handlePrev() {
  if (isInLowerLevel) {
    tempVal--;
    if (tempVal < menu[currentMenuPos].minVal) tempVal = menu[currentMenuPos].minVal;
  } else {
    currentMenuPos--;
    if (currentMenuPos < 0) currentMenuPos = menuSize - 1;
  }
}

void handleBack() {
  if (isInLowerLevel) {
    isInLowerLevel = false;
  }
}

void handleOk() {
  if (menu[currentMenuPos].handler != NULL && menu[currentMenuPos].maxVal <= menu[currentMenuPos].minVal) {
    (*(menu[currentMenuPos].handler))();
    return;
  }
  if (isInLowerLevel) {
    menu[currentMenuPos].currentVal = tempVal;
    isInLowerLevel = false;
  } else {
    tempVal = menu[currentMenuPos].currentVal;
    isInLowerLevel = true;
  }
}

