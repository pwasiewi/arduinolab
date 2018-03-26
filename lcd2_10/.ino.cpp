//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2016-01-08 17:45:49

#include "Arduino.h"
#include <LiquidCrystal.h>
int is_empty(char p) ;
void draw_tetromino(struct tetromino *tetr, char val) ;
int move(int mov, struct tetromino *tetr) ;
int find_mapping(struct lcd_map *mapping, byte key) ;
int insert_mapping(struct lcd_map *mapping, byte key) ;
byte compose_key(int i, int j) ;
void create_new_char(int i, int j, int index) ;
void print_to_lcd() ;
int add_new_tetromino(int foo) ;
void setup() ;
int find_full_rows() ;
void mode_down_rows(int row) ;
void loop() ;

#include "lcd2_10.ino"

