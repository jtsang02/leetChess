#ifndef game_h
#define game_h
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// typedef Adafruit_Neopixel np;

class Game {
    public:
        // constructor
        Game(int boardPin, int* columnPins, int* rowPins);
        void getBoardState();
        void readBoard();
        void printBoard();
        void testLED(Adafruit_NeoPixel &strip, int N_LEDS, uint32_t c);

    private:
        int boardPin;
        int* columnPins;
        int* rowPins;       
        int** board;
        void setLED(int x, int y);
        void clearLED(int x, int y);
};

#endif
