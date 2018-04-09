/*
   lcd_nav.ino - Example code using the menu system library

   This example shows using the menu system with a 16x2 LCD display
   (controled over serial).

   Copyright (c) 2015 arduino-menusystem
   Licensed under the MIT license (see LICENSE)
*/

#include <MenuSystem.h>
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

// renderer

// The LCD circuit:
//    * LCD RS pin to digital pin 8
//    * LCD Enable pin to digital pin 9
//    * LCD D4 pin to digital pin 4
//    * LCD D5 pin to digital pin 5
//    * LCD D6 pin to digital pin 6
//    * LCD D7 pin to digital pin 7
//    * LCD R/W pin to ground
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);//konfigurowanie linii do których został dołączony LCD

class MyRenderer : public MenuComponentRenderer {
  public:
    void render(Menu const& menu) const {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(menu.get_name());
      lcd.setCursor(0, 1);
      menu.get_current_component()->render(*this);
    }

    void render_menu_item(MenuItem const& menu_item) const {
      lcd.print(menu_item.get_name());
    }

    void render_back_menu_item(BackMenuItem const& menu_item) const {
      lcd.print(menu_item.get_name());
    }

    void render_numeric_menu_item(NumericMenuItem const& menu_item) const {
      lcd.print(menu_item.get_name());
    }

    void render_menu(Menu const& menu) const {
      lcd.print(menu.get_name());
    }
};
MyRenderer my_renderer;

// Forward declarations

void on_item1_selected(MenuComponent* p_menu_component);
void on_item2_selected(MenuComponent* p_menu_component);
void on_item3_selected(MenuComponent* p_menu_component);

// Menu variables

MenuSystem ms(my_renderer);
MenuItem mm_mi1("Level 1 - Item 1 (Item)", &on_item1_selected);
MenuItem mm_mi2("Level 1 - Item 2 (Item)", &on_item2_selected);
Menu mu1("Level 1 - Item 3 (Menu)");
MenuItem mu1_mi1("Level 2 - Item 1 (Item)", on_item3_selected);

// Menu callback function

void on_item1_selected(MenuComponent* p_menu_component) {
  lcd.setCursor(0, 1);
  lcd.print("Item1 Selected  ");
}

void on_item2_selected(MenuComponent* p_menu_component) {
  lcd.setCursor(0, 1);
  lcd.print("Item2 Selected  ");
}

void on_item3_selected(MenuComponent* p_menu_component) {
  lcd.setCursor(0, 1);
  lcd.print("Item3 Selected  ");
}

void serial_print_help() {
  Serial.println("***************");
  Serial.println("w: go to previus item (up)");
  Serial.println("s: go to next item (down)");
  Serial.println("a: go back (right)");
  Serial.println("d: select \"selected\" item");
  Serial.println("?: print this help");
  Serial.println("h: print this help");
  Serial.println("***************");
}

void serial_handler() {
  char inChar;
  if ((inChar = Serial.read()) > 0) {
    switch (inChar) {
      case 'w': // Previus item
        ms.prev();
        ms.display();
        break;
      case 's': // Next item
        ms.next();
        ms.display();
        break;
      case 'a': // Back presed
        ms.back();
        ms.display();
        break;
      case 'd': // Select presed
        ms.select();
        ms.display();
        break;
      case '?':
      case 'h': // Display help
        serial_print_help();
        break;
      default:
        break;
    }
  }
}

// Standard arduino functions

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(SW1, INPUT);             //konfigurowanie I/O, do których są dołączone przyciski
  pinMode(SW2, INPUT);
  pinMode(SW3, INPUT);
  pinMode(SW4, INPUT);
  digitalWrite(SW1, HIGH);         //dołączenie wewnętrznych rezystorów zasilających
  digitalWrite(SW2, HIGH);
  digitalWrite(SW3, HIGH);
  digitalWrite(SW4, HIGH);

  serial_print_help();

  ms.get_root_menu().add_item(&mm_mi1);
  ms.get_root_menu().add_item(&mm_mi2);
  ms.get_root_menu().add_menu(&mu1);
  mu1.add_item(&mu1_mi1);

  ms.display();
  akcja.attach(0, 500, serial);    // Wątek 1
  akcja.attach(1, 100, przyciski); // Wątek 2
}

void serial() {
  serial_handler();
}

void loop() {                      //pętla główna programu
  akcja.process();                 //inicjalizacja lub aktualizacja wszystkich procedur(wątków, zdarzeń itp.)
}                                  //koniec pętli głównej


void przyciski() {
  if (digitalRead(SW1) == LOW) {
    ms.prev();
    ms.display();
  }
  if (digitalRead(SW2) == LOW) {
    ms.next();
    ms.display();
  }
  if (digitalRead(SW3) == LOW) {
    ms.back();
    ms.display();
  }
  if (digitalRead(SW4) == LOW) {
    ms.select();
    ms.display();
  }
}


