// test code here: https://www.tinkercad.com/things/iJSs6KtR5pe-leetchess-8-x-8-matrix/editel

#include <Adafruit_NeoPixel.h>
#define BOARD_SIZE 8               // number of columns and rows in the board
#define N_LEDS 8 * 8               // number of individual LEDs in one neopixel strip
#define BOARD_PIN 12               // pin for the neopixel strip
String msg;                        // string to read and print serial commands

int columnPins[] = {8, 9, 10, 11}; // pins for columns
int rowPins[] = {2, 3, 4, 6};      // pins for rows

// map 8 x 8 matrix to 64 LEDs
const int LED_matrix[BOARD_SIZE][BOARD_SIZE] = {
  {0, 1, 2, 3, 4, 5, 6, 7},
  {15, 14, 13, 12, 11, 10, 9, 8},
  {16, 17, 18, 19, 20, 21, 22, 23},
  {31, 30, 29, 28, 27, 26, 25, 24},
  {32, 33, 34, 35, 36, 37, 38, 39},
  {47, 46, 45, 44, 43, 42, 41, 40},
  {48, 49, 50, 51, 52, 53, 54, 55},
  {63, 62, 61, 60, 59, 58, 57, 56}
};

// boolean matrix to store the current state of the board
int board[BOARD_SIZE][BOARD_SIZE];
// boolean matrix to store the previous state of the board
int prevBoard[BOARD_SIZE][BOARD_SIZE];

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

// create neopixel object
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, BOARD_PIN, NEO_GRB + NEO_KHZ800); // tkcad testing
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, BOARD_PIN, NEO_RGBW + NEO_KHZ800); // physical testing
// color definitions for LED strip
uint32_t RED = strip.Color(255, 0, 0); // red
uint32_t GREEN = strip.Color(0, 255, 0); // green
uint32_t BLUE = strip.Color(0, 0, 255); // blue
uint32_t YELLOW = strip.Color(255, 255, 0); // yellow
uint32_t PURPLE = strip.Color(255, 0, 255); // purple
uint32_t CYAN = strip.Color(0, 255, 255); // cyan
uint32_t WHITE = strip.Color(255, 255, 255); // white

void setup(){
  // initialize serial communication
  Serial.begin(9600);
  // initialize board state to initial board state
  initializeBoard();
  // initialize neopixel strip
  strip.show();
  strip.setPin(BOARD_PIN);
  // print board state - this should be the initial board state
  printBoardState();
}

void loop() {
  
  // simulate a move at position (x, y)
  changePos(0, 7);
  mainLoop();
}

// main code here
void mainLoop() {
  
  // get the state of the board
  getBoardState();

  // check if the board state has changed
  if (!boardStateChanged())
    return;

  // check correct turn
  if (!userTurn())
    return;
    
  printBoardState();

  // update the previous board state
  updatePrevBoardState();
  delay(1000);
}
//=================================================================================================//
// functions to update pattern on board
//=================================================================================================//

void initializeBoard() {

  // initialize columns to output
  for (int i = 0; i < sizeof(columnPins) / sizeof(int); i++)
    pinMode(columnPins[i], OUTPUT);
  // initialize rows to input
  for (int i = 0; i < sizeof(rowPins) / sizeof(int); i++)
    pinMode(rowPins[i], INPUT);

  // initialize board state to initial board state
  for (int i = 0; i < BOARD_SIZE; i++)
    for (int j = 0; j < BOARD_SIZE; j++)
      board[i][j] = initBoard[i][j];
}

void getBoardState() {
  // loop through columns
  for (int i = 0; i < sizeof(columnPins) / sizeof(int); i++) {
    // set column to high
    digitalWrite(columnPins[i], HIGH);

    // loop through rows
    for (int j = 0; j < sizeof(rowPins) / sizeof(int); j++) {
      // if the row is high, set the corresponding board state to 1
      if (digitalRead(rowPins[j]) == HIGH)
        board[i][j] = 1;
      // if the row is low, set the corresponding board state to 0
      else
        board[i][j] = 0;
    }
    // set column to low
    digitalWrite(columnPins[i], LOW);
  }
}

// check if the board state has changed
bool boardStateChanged() {
  // check if the board state has changed since the last time the board state was checked
  for (int i = 0; i < BOARD_SIZE; i++)
    for (int j = 0; j < BOARD_SIZE; j++)
      if (board[i][j] != prevBoard[i][j])
        return true;
  return false;
}

// check who's turn it is (user or computer)
bool userTurn() {
  // send a request to the computer to check who's turn it is
  sendCommand("checkTurn", "");
  // wait for a response from the computer
  while (Serial.available() == 0);
  String response = Serial.readStringUntil('\n'); // read the response from the computer
  if (response == "user")
    return true;
  if (response == "computer")
    return false;

  return false;
}

// update the previous board state
void updatePrevBoardState() {
  // update the previous board state to the current board state
  for (int i = 0; i < BOARD_SIZE; i++)
    for (int j = 0; j < BOARD_SIZE; j++)
      prevBoard[i][j] = board[i][j];
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
  strip.fill(RED, 0, N_LEDS);
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
  printPos(x, y);
  strip.setPixelColor(LED_matrix[y][x], RED);
  strip.show();
}

// turn off a single LED
void clearLED(int x, int y) {
   Serial.print("LED is off at: ");
  printPos(x, y);
  strip.setPixelColor(LED_matrix[y][x], strip.Color(0, 0, 0, 0));
  strip.show();
}

//=================================================================================================//
// functions to print to serial monitor for debugging
//=================================================================================================//

// print the board state to the serial monitor for debugging
void printBoardState() {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++)
      Serial.print(board[i][j]);
    Serial.println();
  }
}

// print the LED matrix to the serial monitor for debugging
void printLEDMatrix() {
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++)
      Serial.print(LED_matrix[i][j]);
    Serial.println();
  }
}

// print the coordinates of a single piece
void printPos(int x, int y) {
  Serial.print(x);
  Serial.print(',');
  Serial.print(y);
  Serial.println();
}

//=================================================================================================//
// Raspberry Pi communication functions
//=================================================================================================//
 
// send a request to the Raspberry Pi
void sendCommand(String req, String arg) {
   Serial.println("Cmd: " + req + arg);
}

// receive a command from the Raspberry Pi
// TODO: add error checking for invalid commands
// TODO: add conditions for different commands
void receiveCommand() {
  // if there is data to read
  if (Serial.available() > 0) {  // Check if there is data coming
    msg = Serial.readString();    // Read the message as String
    Serial.println("Command: " + msg);
  }
}

//=================================================================================================//
// functions to simulate making a move on the board (for testing) - to be removed later
//=================================================================================================//
 
// simulate changing the board state
void changePos(int x, int y) {
  board[y][x] = !board[y][x];
  sendCommand("changePos at ", "" + String(x) + "," + String(y) + " to " + String(board[y][x]));
  // turn on LED at (x, y)
  if (board[y][x] == 1)
    setLED(x, y);
  // turn off LED at (x, y)
  else
    clearLED(x, y);
}