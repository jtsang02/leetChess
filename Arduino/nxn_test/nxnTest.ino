/*
  Test of 6 x 6 Reed Switches
  using input pins 2-7 for columns
  and input pins 8-13 for rows
*/

#include <Adafruit_NeoPixel.h>
#define BOARD_SIZE 2               // number of columns and rows in the board
#define N_LEDS 8 * 8               // number of individual LEDs in one neopixel strip
#define BOARD_PIN 12               // pin for the neopixel strip
int columnPins[] = {8, 9, 11, 12}; // pins for columns
int rowPins[] = {2, 3, 6, 7};      // pins for rows

void setup()
{
    // initialize serial communication
    Serial.begin(9600);

    // initialize columns to output
    for (int i = 0; i < sizeof(columnPins) / sizeof(int); i++)
        pinMode(columnPins[i], OUTPUT);

    // initialize rows to input
    for (int i = 0; i < sizeof(rowPins) / sizeof(int); i++)
        pinMode(rowPins[i], INPUT);

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
    for (int i = 0; i < sizeof(columnPins) / sizeof(int); i++)
    {
        digitalWrite(columnPins[i], HIGH);
        for (int j = 0; j < sizeof(rowPins) / sizeof(int); j++)
        {
            if (digitalRead(rowPins[j]) == HIGH)
                setLED(i, j); // TODO: update to setLED(x, y) function
            else
                clearLED(i, j); // TODO: update to clearLED(x, y) function
        }
        digitalWrite(columnPins[i], LOW);
    }
}

void setLED(int x, int y)
{
    Serial.print("LED is on at: ");
    Serial.print(x);
    Serial.print(',');
    Serial.print(y);
    Serial.println();
    // strip.setPixelColor(matrix[x][y], c);
    // strip.show();
}

// turn off a single LED
void clearLED(int x, int y)
{
    Serial.print("LED is off at: ");
    Serial.print(x);
    Serial.print(',');
    Serial.print(y);
    Serial.println();
    // strip.setPixelColor(matrix[x][y], strip.Color(0, 0, 0, 0));
    // strip.show();
}