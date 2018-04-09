/*
 * Copyright (c) 2015, Alex Zen <me@alexzen.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of poseur-blocks nor the names of its
 *        contributors may be used to endorse or promote products derived from
 *        this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ''AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *  Pinouts:
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
 */

#include <LiquidCrystal.h>

#include "Timers.h"                 // dołączona biblioteka Timers z forum android http://bit.ly/arduinotimer
Timers <2> akcja;

#define LEFT   1
#define RIGHT  2
#define DOWN   3
#define ROTATE 4

#define N_ROWS 32
#define N_COLS 6

#define N_LCD_CHAR 8

#define SOUND_NOTE 262
#define SOUND_DURATION 100

unsigned long curr_time;
unsigned long prev_time;

int score;
int base_pointer;
const int Led1 =  13;         //przypisanie aliasów do pinów portów
const int Led2 =  12;
const int Led3 =  11;
const int Led4 =  10;
const int SW1  =  3;
const int SW2  =  2;
const int SW3  =  1;
const int SW4  =  0;
const int Buzzer =  A5;

typedef char game_matrix[N_ROWS][N_COLS];

struct lcd_map
{
    int max;
    int lcd_char_map[N_LCD_CHAR];
};

/* Matrix of the game */
game_matrix matrix;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

/*
 * Check if the char in input is "empty" ( == ' ' )
 */
int is_empty(char p)
{
    if (p == ' ')
        return 0;
    else
        return 1;
}

/* Point in 2-dimensional matrix */
struct point
{
    int x;
    int y;
};

/* Tetromino piece */
struct tetromino
{
    char type;

    struct point pos;
    int dir;

    struct point blocks[4];
} tetr;

int max_line = N_ROWS + 1;

struct tetromino start_pos[7];

/*
 * Draw the tetromino on the matrix of the game
 */
void draw_tetromino(struct tetromino *tetr, char val)
{
    struct point bl;
    int i;

    for (i=0; i<4; i++)
    {
        bl = tetr->blocks[i];

        matrix[bl.x][bl.y] = val;
    }
}

/*
 * Move the tetromino.
 * If collision occurs, return 1
 */
int move(int mov, struct tetromino *tetr)
{
    int i;
    struct point temp[4]; /* New position of blocks  */
    struct point pos; /* New position of tetromino */

    /* Compute new blocks' positions */
    switch(mov)
    {
        case LEFT:
            for (i=0; i<4; i++)
            {
                temp[i].x = tetr->blocks[i].x;
                temp[i].y = tetr->blocks[i].y - 1;
            }
            pos.x = tetr->pos.x;
            pos.y = tetr->pos.y - 1;

            break;
        case RIGHT:
            for (i=0; i<4; i++)
            {
                temp[i].x = tetr->blocks[i].x;
                temp[i].y = tetr->blocks[i].y + 1;
            }
            pos.x = tetr->pos.x;
            pos.y = tetr->pos.y + 1;

            break;
        case DOWN:
            for (i=0; i<4; i++)
            {
                temp[i].x = tetr->blocks[i].x + 1;
                temp[i].y = tetr->blocks[i].y;
            }
            pos.x = tetr->pos.x + 1;
            pos.y = tetr->pos.y;

            break;
        case ROTATE:
            for (i=0; i<4; i++)
            {
                temp[i].x = -(tetr->blocks[i].y - tetr->pos.y) + tetr->pos.x;
                temp[i].y = tetr->blocks[i].x - tetr->pos.x + tetr->pos.y;
            }
            pos.x = tetr->pos.x;
            pos.y = tetr->pos.y;

            break;
    }

    for (i=0; i<4; i++)
    {
        if (temp[i].x < 0  ||
            temp[i].x > base_pointer-1 ||
            temp[i].y < 0  ||
            temp[i].y > N_COLS-1)
                return 1;
    }

    /* Move the tetromino */
    draw_tetromino(tetr, ' ');

    /* Check for collisions */
    for (i=0; i<4; i++)
    {
        if (matrix[temp[i].x][temp[i].y] != ' ')
        {
            draw_tetromino(tetr, tetr->type);
            return 1;
        }
    }

    for (i=0; i<4; i++)
        tetr->blocks[i] = temp[i];

    tetr->pos = pos;

    if (mov == ROTATE)
        tetr->dir = (tetr->dir + 1) % 4;

    draw_tetromino(tetr, tetr->type);

    return 0;
}

/*
 * Search in the mapping table if the key is present
 */
int find_mapping(struct lcd_map *mapping, byte key)
{
    int i;
    for (i=0; i < (mapping->max); i++)
        if (mapping->lcd_char_map[i] == key)
            return i;

    return -1;
}

/*
 * Insert the key into the mapping table
 */
int insert_mapping(struct lcd_map *mapping, byte key)
{
    int pos = mapping->max;

    if (pos > N_LCD_CHAR-1) // Upper limit of array
        return -1;

    mapping->lcd_char_map[pos] = key;

    mapping->max = pos + 1;

    return pos;
}

/*
 * Compose the key for the mapping table
 */
byte compose_key(int i, int j)
{
    byte key = 0;

    key |= is_empty(matrix[i  ] [j+2]) << 5;
    key |= is_empty(matrix[i+1] [j+2]) << 4;
    key |= is_empty(matrix[i  ] [j+1]) << 3;
    key |= is_empty(matrix[i+1] [j+1]) << 2;
    key |= is_empty(matrix[i  ] [j]  ) << 1;
    key |= is_empty(matrix[i+1] [j]  );

    return key;
}

/*
 * Create a new char for the LCD screen
 */
void create_new_char(int i, int j, int index)
{
    byte bt_1 = B00000;
    byte bt_2 = B00000;
    byte bt_3 = B00000;

    bt_1 |= is_empty(matrix[i  ] [j+2]) << 4;
    bt_1 |= is_empty(matrix[i  ] [j+2]) << 3;
    bt_1 |= is_empty(matrix[i+1] [j+2]) << 1;
    bt_1 |= is_empty(matrix[i+1] [j+2]);

    bt_2 |= is_empty(matrix[i  ] [j+1]) << 4;
    bt_2 |= is_empty(matrix[i  ] [j+1]) << 3;
    bt_2 |= is_empty(matrix[i+1] [j+1]) << 1;
    bt_2 |= is_empty(matrix[i+1] [j+1]);

    bt_3 |= is_empty(matrix[i  ] [j]  ) << 4;
    bt_3 |= is_empty(matrix[i  ] [j]  ) << 3;
    bt_3 |= is_empty(matrix[i+1] [j]  ) << 1;
    bt_3 |= is_empty(matrix[i+1] [j]  );

    byte new_char[8] =
        {
            bt_1,
            bt_1,
            B00000,
            bt_2,
            bt_2,
            B00000,
            bt_3,
            bt_3
        };

    lcd.createChar(index, new_char);

    return;
}

/*
 * Draw the matrix on Arduino's LCD
 */
void print_to_lcd()
{
    struct lcd_map lcd_char_map;
    lcd_char_map.max = 0;

    byte key = 0;
    int index, i, j;

    //lcd.clear();

    // Create custom LCD's characters
    for (i=0; i<base_pointer; i+=2)
        for (j=0; j<N_COLS; j+=3)
        {
            key = compose_key(i, j);

            if (key == 0)
                continue;

            index = find_mapping(&lcd_char_map, key);

            if (index == -1)
            {
                index = insert_mapping(&lcd_char_map, key);
                create_new_char(i, j, index);
            }
        }

    lcd.begin(16, 2);

    // Print the characters on LCD screen
    for (j=N_COLS-3; j>=0; j-=3)
    {
        for (i=0; i<N_ROWS; i+=2)
        {
            if (i >= base_pointer)
            {
                lcd.write("$");
                continue;
            }

            key = compose_key(i, j);

            if (key == 0)
            {
                lcd.write(" ");
                continue;
            }


            index = find_mapping(&lcd_char_map, key);

            lcd.write(byte(index));
        }

        lcd.setCursor(0, 1);
    }
}

/*
 * Add a new tetromino on the game
 */
int add_new_tetromino(int foo)
{
    tetr.blocks[0].x = start_pos[foo].blocks[0].x;
    tetr.blocks[0].y = start_pos[foo].blocks[0].y;
    tetr.blocks[1].x = start_pos[foo].blocks[1].x;
    tetr.blocks[1].y = start_pos[foo].blocks[1].y;
    tetr.blocks[2].x = start_pos[foo].blocks[2].x;
    tetr.blocks[2].y = start_pos[foo].blocks[2].y;
    tetr.blocks[3].x = start_pos[foo].blocks[3].x;
    tetr.blocks[3].y = start_pos[foo].blocks[3].y;

    tetr.dir = start_pos[foo].dir;
    tetr.type = start_pos[foo].type;
    tetr.pos.x = start_pos[foo].pos.x;
    tetr.pos.y = start_pos[foo].pos.y;

    // Check space
    if (matrix[tetr.blocks[0].x][tetr.blocks[0].y] != ' ' ||
        matrix[tetr.blocks[1].x][tetr.blocks[1].y] != ' ' ||
        matrix[tetr.blocks[2].x][tetr.blocks[2].y] != ' ' ||
        matrix[tetr.blocks[3].x][tetr.blocks[3].y] != ' ')

        return 1;
    else
        return 0;
}

/*
 * Arduino's setup() function. It is called when the sketch starts
 */
void setup()
{
    int i, j, mvm, foo;
    char cmd;

    score = 0;
    base_pointer = N_ROWS;

    randomSeed(millis());

    pinMode(Led1, OUTPUT);    //Konfiguracja linii do ktorych sa dolaczone diody jako wyjscia
    pinMode(Led2, OUTPUT);
    pinMode(Led3, OUTPUT);
    pinMode(Led4, OUTPUT);
    pinMode(Buzzer, OUTPUT);    //konfiguracja linii do ktorej jest dolaczony brzeczyk jako wyjscie
    pinMode(SW1, INPUT);        //konfiguracja linii do ktorych sa dolaczone przyciski jako wejscia
    pinMode(SW2, INPUT);
    pinMode(SW3, INPUT);
    pinMode(SW4, INPUT);
    digitalWrite(SW1, HIGH);   //dolaczenie do linii do ktorych sa dolaczone przyciski rezystorow podciagajacych co wymusi na nich domyslnie stan wysoki
    digitalWrite(SW2, HIGH);
    digitalWrite(SW3, HIGH);
    digitalWrite(SW4, HIGH);
    digitalWrite(Led1, HIGH);  //domyslne wylaczenie diod LED
    digitalWrite(Led2, HIGH);
    digitalWrite(Led3, HIGH);
    digitalWrite(Led4, HIGH);
    digitalWrite(Buzzer, HIGH);  //domyslne wylaczenie brzeczyka

    /* initial positions of the tetrominoes */

    /* I-block */
    start_pos[0].type = 'I';
    start_pos[0].pos.x = 1;
    start_pos[0].pos.y = 2;
    start_pos[0].dir = 0;
    start_pos[0].blocks[0].x = 1;
    start_pos[0].blocks[0].y = 1;
    start_pos[0].blocks[1].x = 1;
    start_pos[0].blocks[1].y = 2;
    start_pos[0].blocks[2].x = 1;
    start_pos[0].blocks[2].y = 3;
    start_pos[0].blocks[3].x = 1;
    start_pos[0].blocks[3].y = 4;

    /* J-block */
    start_pos[1].type = 'J';
    start_pos[1].pos.x = 1;
    start_pos[1].pos.y = 3;
    start_pos[1].dir = 0;
    start_pos[1].blocks[0].x = 0;
    start_pos[1].blocks[0].y = 2;
    start_pos[1].blocks[1].x = 1;
    start_pos[1].blocks[1].y = 2;
    start_pos[1].blocks[2].x = 1;
    start_pos[1].blocks[2].y = 3;
    start_pos[1].blocks[3].x = 1;
    start_pos[1].blocks[3].y = 4;

    /* L-block */
    start_pos[2].type = 'L';
    start_pos[2].pos.x = 1;
    start_pos[2].pos.y = 3;
    start_pos[2].dir = 0;
    start_pos[2].blocks[0].x = 1;
    start_pos[2].blocks[0].y = 2;
    start_pos[2].blocks[1].x = 1;
    start_pos[2].blocks[1].y = 3;
    start_pos[2].blocks[2].x = 1;
    start_pos[2].blocks[2].y = 4;
    start_pos[2].blocks[3].x = 0;
    start_pos[2].blocks[3].y = 4;

    /* O-block */
    start_pos[3].type = 'O';
    start_pos[3].pos.x = 1;
    start_pos[3].pos.y = 3;
    start_pos[3].dir = 0;
    start_pos[3].blocks[0].x = 0;
    start_pos[3].blocks[0].y = 2;
    start_pos[3].blocks[1].x = 0;
    start_pos[3].blocks[1].y = 3;
    start_pos[3].blocks[2].x = 1;
    start_pos[3].blocks[2].y = 2;
    start_pos[3].blocks[3].x = 1;
    start_pos[3].blocks[3].y = 3;

    /* S-block */
    start_pos[4].type = 'S';
    start_pos[4].pos.x = 1;
    start_pos[4].pos.y = 3;
    start_pos[4].dir = 0;
    start_pos[4].blocks[0].x = 1;
    start_pos[4].blocks[0].y = 2;
    start_pos[4].blocks[1].x = 1;
    start_pos[4].blocks[1].y = 3;
    start_pos[4].blocks[2].x = 0;
    start_pos[4].blocks[2].y = 3;
    start_pos[4].blocks[3].x = 0;
    start_pos[4].blocks[3].y = 4;

    /* T-block */
    start_pos[5].type = 'T';
    start_pos[5].pos.x = 1;
    start_pos[5].pos.y = 3;
    start_pos[5].dir = 0;
    start_pos[5].blocks[0].x = 0;
    start_pos[5].blocks[0].y = 3;
    start_pos[5].blocks[1].x = 1;
    start_pos[5].blocks[1].y = 2;
    start_pos[5].blocks[2].x = 1;
    start_pos[5].blocks[2].y = 3;
    start_pos[5].blocks[3].x = 1;
    start_pos[5].blocks[3].y = 4;

    /* Z-block */
    start_pos[6].type = 'Z';
    start_pos[6].pos.x = 1;
    start_pos[6].pos.y = 3;
    start_pos[6].dir = 0;
    start_pos[6].blocks[0].x = 0;
    start_pos[6].blocks[0].y = 2;
    start_pos[6].blocks[1].x = 0;
    start_pos[6].blocks[1].y = 3;
    start_pos[6].blocks[2].x = 1;
    start_pos[6].blocks[2].y = 3;
    start_pos[6].blocks[3].x = 1;
    start_pos[6].blocks[3].y = 4;

    for (i=0; i<N_ROWS; i++)
        for (j=0; j<N_COLS; j++)
            matrix[i][j] = ' ';

    prev_time = millis();

    add_new_tetromino(random(7));

    draw_tetromino(&tetr, tetr.type);

    print_to_lcd();
}

/*
 * Return the first full row found
 */
int find_full_rows()
{
    int i, j;
    int flag;

    for (i = base_pointer-1; i>=0; i--)
    {
        flag = 0;

        for (j=0; j<N_COLS; j++)
            if (matrix[i][j] == ' ')
                flag = 1;

        if (flag == 0)
            return i;
    }

    return -1;
}

/*
 * Erase a row
 */
void mode_down_rows(int row)
{
    int i, j;

    for (i=row; i>0; i--)
        for (j=0; j<N_COLS; j++)
            matrix[i][j] = matrix[i-1][j];

    matrix[0][0] = ' ';
    matrix[0][1] = ' ';
    matrix[0][2] = ' ';
    matrix[0][3] = ' ';
}

int SW1value = 0;
int SW2value = 0;
int SW3value = 0;
int SW4value = 0;

/*
 * Arduino's loop() function
 */
void loop()
{
    int foo, ret, ret_mov;
    int mov = 0;
    int row_index;

    int i, j;
    int flag = 0;

    curr_time = millis();
    int key_val = analogRead(A0);
    //Serial.println(key_val);

    SW1value = digitalRead(SW1);
    SW2value = digitalRead(SW2);
    SW3value = digitalRead(SW3);
    SW4value = digitalRead(SW4);
    Serial.print(SW1value);
    Serial.print(SW2value);
    Serial.print(SW3value);
    Serial.println(SW4value);
    if (SW1value == LOW || SW2value == LOW) {         //Sprawdzenie czy nacisniety przycisk S1
    	mov = LEFT;
    }
    if (SW4value == LOW || SW3value == LOW) {         //Sprawdzenie czy nacisniety przycisk S1
    	mov = RIGHT;
    }

    // Check input
    if (key_val >= 127 && key_val <= 255) // rotate
        mov = ROTATE;

    // Move/rotate the piece
    if (mov != 0)
    {
        ret = move(mov, &tetr);
        if (ret == 0)
        {
            draw_tetromino(&tetr, tetr.type);
            print_to_lcd();
            tone(8, SOUND_NOTE, SOUND_DURATION);
            delay(100);
            return;
        }
    }

    // Move down the piece
    curr_time = millis();
    if (curr_time - prev_time > 500)
    {
        prev_time = curr_time;

        if ((ret_mov = move(DOWN, &tetr)) == 1) // piece has reached the bottom
        {
            while ((row_index = find_full_rows()) != -1)
            {
                mode_down_rows(row_index);

                score += 10;
            }

            // Apply the rule
            for (i=0; i<base_pointer;i++)
            {
                for (j=0; j<N_COLS; j++)
                    if (matrix[i][j] != ' ')
                    {
                        flag = 1;

                        if (base_pointer - i > 4)
                        {
                            base_pointer -= (base_pointer - i - 4);

                            if (base_pointer % 2 != 0)
                                base_pointer -= 1;
                        }

                        break;
                    }

                if (flag == 1)
                    break;
            }

            ret = add_new_tetromino(random(7));

            if (ret != 0)
            {
                // print score
                lcd.clear();
                lcd.print("Score: " );
                lcd.print(score);

                while(1)
                    ;
            }

            draw_tetromino(&tetr, tetr.type);
        }

        print_to_lcd();

        if (ret_mov == 1)
            tone(8, SOUND_NOTE, SOUND_DURATION);
    }
}
