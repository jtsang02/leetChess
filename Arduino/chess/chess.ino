#include <Adafruit_NeoPixel.h>
#define BOARD_SIZE 8               // number of columns and rows in the board
#define N_LEDS 8 * 8               // number of individual LEDs in one neopixel strip
#define BOARD_PIN 12               // pin for the neopixel strip

int columnPins[] = {8, 9, 10, 11}; // pins for columns
int rowPins[] = {2, 3, 4, 6};      // pins for rows

// map 8 x 8 matrix to 64 LEDs
const int LED_matrix[BOARD_SIZE][BOARD_SIZE] = {
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
int board[BOARD_SIZE][BOARD_SIZE];

const int initBoard[BOARD_SIZE][BOARD_SIZE] = {
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1}
};

// create neopixel strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, BOARD_PIN, NEO_GRB + NEO_KHZ800); // tkcad testing
uint32_t c = strip.Color(255, 0, 0); // tkcad testing

void setup(){
  // initialize serial communication
  Serial.begin(9600);

  // initialize columns to output
  for (int i = 0; i < sizeof(columnPins) / sizeof(int); i++)
    pinMode(columnPins[i], OUTPUT);

  // initialize rows to input
  for (int i = 0; i < sizeof(rowPins) / sizeof(int); i++)
    pinMode(rowPins[i], INPUT);

  // initialize neopixel strip
  strip.show();
  strip.setPin(BOARD_PIN);

  // initialize board state
  for (int i = 0; i < BOARD_SIZE; i++)
    for (int j = 0; j < BOARD_SIZE; j++)
      board[i][j] = initBoard[i][j];

  // print board state
  printBoardState();
}

void loop() {
  // get the state of the board
  getBoardState();
  testLEDs();
  delay(1000);
}

//=================================================================================================//
// functions to update pattern on board
//=================================================================================================//

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

void printBoardState() {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++)
      Serial.print(board[i][j]);
    Serial.println();
  }
}

//=================================================================================================//
// functions to update NeoPixel strip
//=================================================================================================//

void testLEDs() {
  allLEDsOn();
  delay(1000);
  allLEDsOff();
  delay(1000);
}

void allLEDsOn() {
  strip.fill(c, 0, N_LEDS);
  strip.show();
}

// turn off all LEDs
void allLEDsOff() {
  strip.clear();
  strip.show();
}

// turn on LED at (x, y)
void setLED(int x, int y) {
  Serial.print("LED is on at: ");
  Serial.print(x);
  Serial.print(',');
  Serial.print(y);
  Serial.println();
  strip.setPixelColor(LED_matrix[x][y], c);
  strip.show();
}

// turn off a single LED
void clearLED(int x, int y) {
   Serial.print("LED is off at: ");
   Serial.print(x);
   Serial.print(',');
   Serial.print(y);
   Serial.println();
  strip.setPixelColor(LED_matrix[x][y], strip.Color(0, 0, 0, 0));
  strip.show();
}
