/*
  Test of 6 x 6 Reed Switches
  using input pins 2-7 for columns
  and input pins 8-13 for rows
*/
#include <Adafruit_NeoPixel.h>

#define BOARD_PIN 12               // pin for the neopixel strip
int columnPins[] = { 2, 3, 4, 5, 6 };      // pins for columns
int rowPins[] = { 23, 25, 27, 29, 31};      // pins for rows
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
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        digitalWrite(rowPins[i], HIGH);
        for (int j = 0; j < BOARD_SIZE; j++)
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

void setLED(int y, int x)
{
    Serial.print("Piece is on: ");
    Serial.print(x);
    Serial.print(',');
    Serial.print(y);
    Serial.println();
}

// turn off a single LED
void clearLED(int x, int y)
{
    Serial.print("Piece is off: ");
    Serial.print(x);
    Serial.print(',');
    Serial.print(y);
    Serial.println();
}