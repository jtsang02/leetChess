/*
  chess.ino
  Created by: Josiah Tsang
  Date: 2023 02 01
  Description: TinkerCAD test code for nxn LED matrix, reed switch, and serial communication
  https://www.tinkercad.com/things/iJSs6KtR5pe-leetchess-8-x-8-matrix/editel
*/
#include <Adafruit_NeoPixel.h>

#define N_LEDS 64                   // number of individual LEDs in one neopixel strip
#define BOARD_SIZE 8
#define BOARD_PIN 13               // pin for the neopixel strip
#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19

String msg = "starting board...";         // string to read and print serial commands
int columnPins[] = { 2, 3, 4, 5, 6, 7, 8, 9 };      // pins for columns
int rowPins[] = { 19, 18, 17, 16, 15, 14, 11, 10 };      // pins for rows
// int BOARD_SIZE = sizeof(rowPins) / sizeof(int); // size of the board

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
// Argument 1 - Number of pixels in NeoPixel strip
// Argument 2 - Arduino pin number (most are valid)
// Argument 3 - Pixel type flags, add together as needed
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, BOARD_PIN, NEO_GRB + NEO_KHZ800); // physical testing

// color definitions for LED strip
uint32_t RED = strip.Color(255, 0, 0); // red
uint32_t GREEN = strip.Color(0, 255, 0); // green
uint32_t BLUE = strip.Color(0, 0, 255); // blue
uint32_t YELLOW = strip.Color(255, 255, 0); // yellow
uint32_t PURPLE = strip.Color(255, 0, 255); // purple
uint32_t CYAN = strip.Color(0, 255, 255); // cyan
uint32_t WHITE = strip.Color(255, 255, 255); // white

void setup() {
  // initialize serial communication
  Serial.begin(9600);
  // initialize board state to initial board state
  initializeBoard();
  // initialize neopixel strip
  strip.begin();
  strip.show();
  strip.setPin(BOARD_PIN);
  allLEDsOff();
  // print board state - this should be the initial board state
  printBoardState();
}

void loop() {
  // simulate a move at position (x, y)
  // changePos();
  // main code here
  mainLoop();
  printBoardState();
  delay(1000);
}

// main code here runs repeatedly
void mainLoop() {
  // test();  // for testing purposes only - to be removed

  // get the state of the board - comment out for testing purposes
  getBoardState();
  // check if the board state has changed
  int x1, y1;   // coordinates of the changed position
  if (!boardStateChanged(x1, y1)) {
    Serial.println("no change"); // send no change message to Raspberry Pi
    return;
  }
    
  // check correct turn
  if (!userTurn()) {
    // TODO:
    // send request for computer's moves to Raspberry Pi
    // get legal moves from Raspberry Pi
    // update the board state
    // update the previous board state
    // print board state
    return;
  }

  String usersMove = requestUserMove(x1, y1);
  if (usersMove == "error") {
    Serial.println("error"); // send error message to Raspberry Pi
    return;
  }
  highlightPath(usersMove);
  int x2, y2;  // coordinates of the second piece's position
  bool secondPieceMoved = false;

  // check if the board state has changed again - second piece moved on same turn
  while (!secondPieceMoved) {
    Serial.println("waiting for second piece to move...");
    updatePrevBoardState();
    delay(100);
    getBoardState();
    delay(100);
    secondPieceMoved = boardStateChanged(x2, y2);
  }

  // turn off leds
  allLEDsOff();
  
  // if piece x1, y1 is placed back on its original position
  if (x1 == x2 && y1 == y2) {
    Serial.println("piece moved back to original position");
    return;
  } 
  // if piece x1, y1 is placed on a different position
  // check if the move is legal
  if (!legalMove(x1, y1, x2, y2)) {
    Serial.println("illegal move");
    blinkLED(x2, y2, RED, 3);
    return;
  }
  // if the move is legal, update the board state send to Raspberry Pi
  updateBoard(x1, x2, y1, y2);
  // update the previous board state - last step
  updatePrevBoardState();
  delay(1000);
}

//=================================================================================================//
// functions to update/track the board state
//=================================================================================================//

void initializeBoard() {
  // initialize columns to output
  for (int i = 0; i < BOARD_SIZE; i++)
    pinMode(columnPins[i], OUTPUT);
  // initialize rows to input
  for (int i = 0; i < BOARD_SIZE; i++)
    pinMode(rowPins[i], INPUT);

  // initialize board state to initial board state
  for (int i = 0; i < BOARD_SIZE; i++)
    for (int j = 0; j < BOARD_SIZE; j++)
      board[i][j] = initBoard[i][j];
}

void getBoardState() {
  // loop through columns
  for (int i = 0; i < BOARD_SIZE; i++) {
    // set column to high
    digitalWrite(columnPins[i], HIGH);

    // loop through rows
    for (int j = 0; j < BOARD_SIZE; j++) {
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
bool boardStateChanged(int& x, int& y) {
  // check if the board state has changed since the last time the board state was checked
  for (int i = 0; i < BOARD_SIZE; i++)
    for (int j = 0; j < BOARD_SIZE; j++)
      if (board[i][j] != prevBoard[i][j]) {
        y = i;
        x = j;
        return true;
      }    
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

bool legalMove(int x1, int y1, int x2, int y2) {
  // send a request to the computer to check if the move is legal
  sendCommand("checkMove", "(" + String(x1) + "," + String(y1) + ") to (" + String(x2) +  "," + String(y2) + ")");
  // wait for a response from the computer
  while (Serial.available() == 0);
  String response = Serial.readStringUntil('\n'); // read the response from the computer
  if (response == "legal")
    return true;
  if (response == "illegal")
    return false;
  return false;
}

void updateBoard(int x1, int y1, int x2, int y2) {
  // send a request to the computer to update the board state
  sendCommand("updateBoard", "(" + String(x1) + "," + String(y1) + ") to (" + String(x2) +  "," + String(y2) + ")");
  // wait for a response from the computer
  while (Serial.available() == 0);
  String response = Serial.readStringUntil('\n'); // read the response from the computer
  if (response == "updated")
    return;
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

void allLEDsOn() {
  strip.fill(RED, 0, N_LEDS);
  strip.show();
}

// turn off all LEDs
void allLEDsOff() {
  strip.clear();
  strip.show();
}

void blinkLED(int x, int y, uint32_t color, int reps) {
  for (int i = 0; i < reps; i++) {
    setLED(x, y, color);
    delay(500);
    clearLED(x, y);
    delay(500);
  }
}

// turn on LED at (x, y)
void setLED(int x, int y, uint32_t color) {
  Serial.print("LED is on at: ");
  printPos(x, y);
  strip.setPixelColor(LED_matrix[y][x], color);
  strip.show();
}

// turn off a single LED
void clearLED(int x, int y) {
   Serial.print("LED is off at: ");
  printPos(x, y);
  strip.setPixelColor(LED_matrix[y][x], strip.Color(0, 0, 0, 0));
  strip.show();
}

void highlightPath(String move) {
  // parse commas separated string into x and y coordinates
  // e.g "1,2" -> x = 1, y = 2, "3,4" -> x = 3, y = 4
  // test: [(0,0)(0,1)(0,2)]
  while (move.indexOf(",") != -1) {
    int commaIndex = move.indexOf(",");
    int x = move.substring(commaIndex - 1, commaIndex).toInt();  // might need a switch statement here
    int y = move.substring(commaIndex + 1).toInt();
    setLED(x, y, GREEN);
    move = move.substring(commaIndex + 1);  // remove the first coordinate from the string
  }
}

//=================================================================================================//
// functions to print to serial monitor for debugging
//=================================================================================================//

// print the current board state to the serial monitor for debugging
void printBoardState() {
  Serial.println("Board state:");
  Serial.println("-----------");
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++)
      Serial.print(board[i][j]);
    Serial.println();
  }
  Serial.println("-----------");
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
  Serial.print("(");
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.println(")");
}

//=================================================================================================//
// Raspberry Pi communication functions
//=================================================================================================//
 
// send a request to the Raspberry Pi
void sendCommand(String req, String arg) {
   Serial.println("Cmd: " + req + " " + arg);
}

// receive a command from the Raspberry Pi
// TODO: add error checking for invalid commands
// TODO: add conditions for different commands
String receiveCommand() {
  // if there is data to read
  String data = "";
  while (Serial.available() == 0);     // Wait for data to be available on the serial port
  data = Serial.readStringUntil('\n'); // Read the message as String
  Serial.println("PI: " + data);
  return data;
}

// request the Raspberry Pi to make a move
String requestUserMove(int x, int y) {
  Serial.println("Requesting user move from PI...");
  // convert coordinates to a string
  sendCommand("makeUserMove", String(x) + "," + String(y));  // do we need to include coordinates of piece moved?
  // wait for a response from the Raspberry Pi
  return receiveCommand(); // this should be the coordinates of the piece moved as a string separated by a comma
}

//=================================================================================================//
// functions to simulate making a move on the board (for testing) - to be removed later
//=================================================================================================//
 
// simulate changing the board state
void changePos() {
  Serial.println("SIM: Enter coordinate to change (x,y): ");
  while (Serial.available() == 0);
  String position = Serial.readStringUntil('\n');
  // if position is not (x,y)
  if (position.indexOf(",") == -1) {
    Serial.println("Invalid position");
    return;
  }
  int commaIndex = position.indexOf(",");
  int x = position.substring(commaIndex - 1, commaIndex).toInt();
  int y = position.substring(commaIndex + 1).toInt();
  board[y][x] = !board[y][x];
  Serial.println("SIM: position change at "+ String(x) + "," + String(y) + " from " + !board[y][x] + " to " + board[y][x]);
}

// simulate the PI telling the Arduino to make a move
void test() {
  Serial.println("Testing Environment. Choose a command: ");
  Serial.println("1. 'off' for all LEDs off");
  Serial.println("2. 'on' for all LEDs on");
  Serial.println("3. 'c' to continue testing main program");
  while (Serial.available() == 0);
  String command = Serial.readStringUntil('\n');
  Serial.println("Command received: " + command);
  if (command == "off") {
    allLEDsOff();
  }
  else if (command == "on") {
    allLEDsOn();
  }
  else if (command == "c") {
    return;
  }
}