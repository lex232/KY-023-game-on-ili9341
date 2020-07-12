/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

#define axis_X A1    // Ось Х подключена к Analog 0
#define axis_Y A2    // Ось Y подключена к Analog 1
#define button 2     // Кнопка подключена к D2

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

unsigned int x;
unsigned int y;
int val_X, val_Y;    // Переменные для хранения значений осей

unsigned int x_enemy;
unsigned int y_enemy;

unsigned int count;
unsigned int count2;
unsigned int flag_enemy = 1;


void setup() {
  Serial.begin(9600);
  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  digitalWrite(button, HIGH);       // Кнопка выстрела
  tft.begin();
  delay(500);
  randomSeed(analogRead(0));
  state();
  read_joy();
}

// Serial.print("X:");
//  Serial.println(val_X, DEC);      // Выводим значение в Serial Monitor


//  Serial.print(" | Y:");
//  Serial.println(val_Y, DEC);      // Выводим значение в Serial Monitor

void loop(void) {
  new_enemy();
  scores();
  clear_aim();
  read_joy();
  cursor_aim();
  monster();
  strike();
  delay(30);
}

void new_enemy (void)
{
  if (flag_enemy == 1)
  {
    random_enemy();
    flag_enemy = 0;
  }
}

void random_enemy (void) {
  x_enemy = random(2, 210);
  y_enemy = random(30, 290);
}

void monster (void) {
  tft.fillRect(x_enemy, y_enemy, 30, 15, ILI9341_GREEN);
  tft.fillRect(x_enemy, (y_enemy - 4), 7, 4, ILI9341_GREEN);
  tft.fillRect((x_enemy + 23), (y_enemy - 4), 7, 4, ILI9341_GREEN);
  tft.fillRect((x_enemy + 5), (y_enemy + 15), 20, 5, ILI9341_GREEN);
}

void read_joy()
{
  val_X = analogRead(axis_X);    // Считываем аналоговое значение оси Х
  val_Y = analogRead(axis_Y);    // Считываем аналоговое значение оси Y
  x = (val_X / 3) - 30;
  y = (val_Y / 3) + 10;
  if (y < 25)
  {
    y = 25;
  }
}

void scores (void)
{
  if (count != count2) {
    count++;
    tft.fillRect(200, 0, 50, 25, ILI9341_BLACK);
    tft.setTextColor(ILI9341_RED);    tft.setTextSize(2);
    tft.setCursor(200, 0);
    tft.println(count);
  }
}


void strike (void) {
  if (digitalRead(button)==0 && x_enemy + 30 > x && x_enemy < x && y_enemy + 20 > y && y_enemy < y)
  {
    count2++;
    tft.fillRect(x_enemy, y_enemy, 30, 15, ILI9341_BLACK);
    tft.fillRect(x_enemy, (y_enemy - 4), 7, 4, ILI9341_BLACK);
    tft.fillRect((x_enemy + 23), (y_enemy - 4), 7, 4, ILI9341_BLACK);
    tft.fillRect((x_enemy + 5), (y_enemy + 15), 20, 5, ILI9341_BLACK);
    flag_enemy = 1;
  }
}


void state (void)
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_RED);    tft.setTextSize(2);
  tft.print("SCORE:::");
}

void cursor_aim (void)
{
  tft.drawCircle(x, y, 3, ILI9341_RED);
  tft.drawCircle(x, y, 4, ILI9341_RED);
  tft.drawCircle(x, y, 7, ILI9341_RED);
  tft.drawCircle(x, y, 8, ILI9341_RED);
}

void clear_aim (void)
{
  tft.drawCircle(x, y, 3, ILI9341_BLACK);
  tft.drawCircle(x, y, 4, ILI9341_BLACK);
  tft.drawCircle(x, y, 7, ILI9341_BLACK);
  tft.drawCircle(x, y, 8, ILI9341_BLACK);
}
