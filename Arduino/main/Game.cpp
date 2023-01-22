#include "Game.h"

// constructor
Game::Game(int boardPin, int* columnPins, int* rowPins) {   
    this->boardPin = boardPin;                   // initialize board pin
    this->columnPins = columnPins;               // initialize column pins
    this->rowPins = rowPins;                     // initialize row pins

    // initialize columns to output
    for (int i = 0; i < sizeof(columnPins) / sizeof(int); i++)
        pinMode(columnPins[i], OUTPUT);

    // initialize rows to input
    for (int i = 0; i < sizeof(rowPins) / sizeof(int); i++)
        pinMode(rowPins[i], INPUT);

    getBoardState();                        // get initial board state

}

void Game::getBoardState() {

    Serial.println("Getting board state...");
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

// check the grid for changes
void Game::readBoard() {
    Serial.println("Reading board...");
    for (int i = 0; i < sizeof(columnPins) / sizeof(int); i++) {
        digitalWrite(columnPins[i], HIGH);
        for (int j = 0; j < sizeof(rowPins) / sizeof(int); j++) {
            if (digitalRead(rowPins[j]) == HIGH)
                setLED(i, j); // TODO: update to setLED(x, y) function
            else
                clearLED(i, j); // TODO: update to clearLED(x, y) function
        }
        digitalWrite(columnPins[i], LOW);
    }
}

void Game::printBoard() {
    Serial.println("Printing board...");
    for (int i = 0; i < sizeof(columnPins) / sizeof(int); i++) {
        for (int j = 0; j < sizeof(rowPins) / sizeof(int); j++) {
            Serial.print(board[i][j]);
            Serial.print(' ');
        }
        Serial.println();
    }
}

// test LED
void Game::testLED(Adafruit_NeoPixel &strip, int N_LEDS, uint32_t c) {
    Serial.println("Testing LED...");
    strip.fill(c, 0, N_LEDS);
    strip.show();
}

// turn on LED at (x, y)
void Game::setLED(int x, int y) {
  Serial.print("LED is on at: ");
  Serial.print(x);
  Serial.print(',');
  Serial.print(y);
  Serial.println();
  // strip.setPixelColor(matrix[x][y], c);
  // strip.show();
}

// turn off a single LED
void Game::clearLED(int x, int y) {
   Serial.print("LED is off at: ");
   Serial.print(x);
   Serial.print(',');
   Serial.print(y);
   Serial.println();
  // strip.setPixelColor(matrix[x][y], strip.Color(0, 0, 0, 0));
  // strip.show();
}