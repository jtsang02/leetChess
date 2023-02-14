/*
  nxnTest.ino
  Created by: Josiah Tsang
  Date: 2023 02 01
  Description: Test code for nxn LED matrix
  https://www.tinkercad.com/things/iJSs6KtR5pe-leetchess-8-x-8-matrix/editel
*/

#include <Adafruit_NeoPixel.h>

#define BOARD_PIN 13               // pin for the neopixel strip
#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19

int columnPins[] = { 2, 3, 4, 5, 6, 7, 8, 9 };      // pins for columns
int rowPins[] = { 19, 18, 17, 16, 15, 14, 11, 10 };      // pins for rows
int BOARD_SIZE = sizeof(rowPins) / sizeof(int); // size of the board

void setup()
{
    // initialize serial communication
    Serial.begin(9600);

    // initialize columns to input
    for (int i = 0; i < BOARD_SIZE; i++)
        pinMode(columnPins[i], INPUT);

    // initialize rows to output
    for (int i = 0; i < BOARD_SIZE; i++)
        pinMode(rowPins[i], OUTPUT);
  

    /*
      initialize neopixel strip here // TODO
    */
}

void loop()
{
    // constantly check for changes in the grid
    checkGrid();
    delay(1000);
}

void checkGrid()
{
    for (int i = 0; i < BOARD_SIZE; i++) {
        digitalWrite(rowPins[i], HIGH);
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (digitalRead(columnPins[j]) == HIGH)
                setLED(i, j); // TODO: update to setLED(x, y) function
        }
        digitalWrite(rowPins[i], LOW);
    }
}

void setLED(int x, int y)
{
    Serial.print("LED is on at: ");
    Serial.print(x);
    Serial.print(',');
    Serial.print(y);
    Serial.println();
}

// turn off a single LED
void clearLED(int x, int y)
{
    Serial.print("LED is off at: ");
    Serial.print(x);
    Serial.print(',');
    Serial.print(y);
    Serial.println();
}