#ifndef game_h
#define game_h
#include <Arduino.h>
#include <Adafuit_NeoPixel.h>

// typedef Adafruit_Neopixel np;

class Game {
    public:
        // constructor
        Game(int boardPin, int* columnPins, int* rowPins);
        void getBoardState();
        void readBoard();

    private:
        int boardPin;
        int* columnPins;
        int* rowPins;       
        int** board;
        void setLED(int x, int y);
        void clearLED(int x, int y);
};

#endif
