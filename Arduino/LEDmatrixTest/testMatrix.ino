/*
   Code for Arduino microcontroller to receive commands
*/
#include <Adafruit_NeoPixel.h>

#define N_LEDS 8 * 8    // number of individual LEDs in one neopixel strip
#define BOARD_PIN 12    // pin for the neopixel strip
String msg;             // string to read and print serial commands

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

// set default color
uint32_t c = strip.Color(255, 0, 0); // tkcad testing
// uint32_t c = strip.Color(0, 255, 0, 0); // physical testing

void setup() {
  // Initialization
  Serial.begin(9600); // Communication rate of the Bluetooth Module
  msg = "";

  // initialize adafruitNeoPixels
  strip.begin(); // in initialize neoPixel strip object (req'd)
  strip.show();  // turn off all pixels in strip

  strip.setPin(BOARD_PIN);
  delay(100);
}

void loop() {
  // To read message received from other Bluetooth Device
  if (Serial.available() > 0) {                            // Check if there is data coming
    msg = Serial.readString(); // Read the message as String
    Serial.println("Command: " + msg);
  }

  // ********************  COMMANDS  ********************
  // test using serial commands, final won't use serial
  // ********************  COMMANDS  ********************

  if (msg == "off") {
    allOff();
  }

  if (msg == "on") {
    allOn();
  }

  // input coordinate of board to turn on (x,y)
  if (msg == "4 5") {
    setLED(4, 5);
  }

  if (msg == "4 5 off") {
    clearLED(4, 5);
  }
}

//=================================================================================================//
// functions to update pattern on board
//=================================================================================================//
static void allOff() {
  strip.clear();
  strip.show();
}

static void allOn(){
  for (uint16_t i = 0; i < N_LEDS; i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

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

// light up a single LED 
void setLED(int x, int y) {
  strip.setPixelColor(matrix[x][y], c);
  strip.show();
}

// turn off a single LED
void clearLED(int x, int y) {
  strip.setPixelColor(matrix[x][y], strip.Color(0, 0, 0, 0));
  strip.show();
}