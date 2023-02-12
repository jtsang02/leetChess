/*
  Test of 6 x 6 Reed Switches
  using input pins 2-7 for columns
  and input pins 8-13 for rows
*/

#include <Adafruit_NeoPixel.h>
#define BOARD_SIZE 2               // number of columns and rows in the board
#define N_LEDS 8 * 8               // number of individual LEDs in one neopixel strip
#define BOARD_PIN 12               // pin for the neopixel strip
int columnPins[] = { 2, 3 };      // pins for columns
int rowPins[] = { 8, 9 };      // pins for rows

void setup()
{
    // initialize serial communication
    Serial.begin(9600);

    // initialize columns to input
    for (int i = 0; i < sizeof(columnPins) / sizeof(int); i++)
        pinMode(columnPins[i], INPUT);

    // initialize rows to output
    for (int i = 0; i < sizeof(rowPins) / sizeof(int); i++)
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
    for (int i = 0; i < sizeof(rowPins) / sizeof(int); i++)
    {
        digitalWrite(rowPins[i], HIGH);
        for (int j = 0; j < sizeof(rowPins) / sizeof(int); j++)
        {
            if (digitalRead(columnPins[j]) == HIGH)
                setLED(i, j); // TODO: update to setLED(x, y) function
            // else
            //     clearLED(i, j); // TODO: update to clearLED(x, y) function
        }
        digitalWrite(rowPins[i], LOW);
    }
    
    // columns only

    // digitalWrite(2, HIGH);
    // // digitalWrite(3, LOW);
    // if (digitalRead(6) == HIGH)
    //   setLED(0, 0);
    
    // if (digitalRead(7) == HIGH)
    //   setLED(0, 1);    

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