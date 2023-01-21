/*
  Test of 6 x 6 Reed Switches
  using input pins 2-7 for columns
  and input pins 8-13 for rows
*/

#include <Adafruit_NeoPixel.h>
#define BOARD_SIZE 8               // number of columns and rows in the board
#define N_LEDS 8 * 8               // number of individual LEDs in one neopixel strip
#define BOARD_PIN 12               // pin for the neopixel strip
int columnPins[] = {8, 9, 10, 11}; // pins for columns
int rowPins[] = {2, 3, 4, 6};      // pins for rows


// map 8 x 8 matrix to 64 LEDs
const int matrix[8][8] = {
  {0, 1, 2, 3, 4, 5, 6, 7},
  {8, 9, 10, 11, 12, 13, 14, 15},
  {16, 17, 18, 19, 20, 21, 22, 23},
  {24, 25, 26, 27, 28, 29, 30, 31},
  {32, 33, 34, 35, 36, 37, 38, 39},
  {40, 41, 42, 43, 44, 45, 46, 47},
  {48, 49, 50, 51, 52, 53, 54, 55},
  {56, 57, 58, 59, 60, 61, 62, 63}
};

// boolean matrix to store the state of the board
int board[BOARD_SIZE][BOARD_SIZE] = {
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1}
};

void setup(){
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

void loop() {
  // constantly check for changes in the grid
  checkGrid();
  delay(1000);
}

/*
  ///////////////////////////////functions/////////////////////////////////////
*/ 
void getBoardState() {
  // loop through columns
  for (int i = 0; i < sizeof(columnPins) / sizeof(int); i++) {
    // set column to high
    digitalWrite(columnPins[i], HIGH);

    // loop through rows
    for (int j = 0; j < sizeof(rowPins) / sizeof(int); j++) {
      // if the row is high, set the corresponding board state to 1
      if (digitalRead(rowPins[j]) == HIGH) {
        board[i][j] = 1;
      }
      // if the row is low, set the corresponding board state to 0
      else {
        board[i][j] = 0;
      }
    }
    // set column to low
    digitalWrite(columnPins[i], LOW);
  }
}

// check the grid for changes
void checkGrid() {
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

// turn on LED at (x, y)
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
  //  Serial.print("LED is off at: ");
  //  Serial.print(x);
  //  Serial.print(',');
  //  Serial.print(y);
  //  Serial.println();
  // strip.setPixelColor(matrix[x][y], strip.Color(0, 0, 0, 0));
  // strip.show();
}
