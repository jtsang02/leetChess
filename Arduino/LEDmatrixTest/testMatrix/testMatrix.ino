/*
   Code for Arduino microcontroller to receive commands
*/
#include <Adafruit_NeoPixel.h>

#define N_LEDS 64     // number of individual LEDs in one neopixel strip
#define BOARD_PIN 12 // pin for the neopixel strip
String msg = "starting...";          // string to read and print serial commands

// create neopixel object
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
// Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, BOARD_PIN, NEO_GRB + NEO_KHZ800); // tkcad testing
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, BOARD_PIN, NEO_GRB + NEO_KHZ800); // physical testing

// set default color
// uint32_t c = strip.Color(0, 255, 0, 0); // physical testing
uint32_t GREEN = strip.Color(255, 0, 0);     // red
uint32_t RED = strip.Color(0, 255, 0);       // green
uint32_t BLUE = strip.Color(0, 0, 255);      // blue
uint32_t YELLOW = strip.Color(255, 255, 0);  // yellow
uint32_t PURPLE = strip.Color(255, 0, 255);  // purple
uint32_t CYAN = strip.Color(0, 255, 255);    // cyan
uint32_t WHITE = strip.Color(255, 255, 255); // white
// set default color
uint32_t c = CYAN;

// map 8 x 8 matrix to 64 LEDs
const int matrix[8][8] = {
    {0, 1, 2, 3, 4, 5, 6, 7},
    {15, 14, 13, 12, 11, 10, 9, 8},
    {16, 17, 18, 19, 20, 21, 22, 23},
    {31, 30, 29, 28, 27, 26, 25, 24},
    {32, 33, 34, 35, 36, 37, 38, 39},
    {47, 46, 45, 44, 43, 42, 41, 40},
    {48, 49, 50, 51, 52, 53, 54, 55},
    {63, 62, 61, 60, 59, 58, 57, 56}};


void setup()
{
  // Initialization
  Serial.begin(9600); // Communication rate of the Bluetooth Module

  // initialize adafruitNeoPixels
  strip.begin(); // in initialize neoPixel strip object (req'd)
  strip.show();  // turn off all pixels in strip

  strip.setPin(BOARD_PIN);
  delay(100);
  Serial.println(msg);
}

void loop()
{
  // To read message received from other Bluetooth Device
  if (Serial.available() > 0) {
                            // Check if there is data coming
    msg = Serial.readStringUntil('\n'); // Read the message as String
    Serial.println("PI Command: " + msg);
  }

  // ********************  COMMANDS  ********************
  // test using serial commands, final won't use serial
  // ********************  COMMANDS  ********************

//   blinkLEDs();
//   setLED(4, 5);
//   delay(1000);
//   clearLED(4, 5);
  
 if (msg == "off")
 {
   allOff();
 }

 if (msg == "on")
 {
   allOn();
 }

 // input coordinate of board to turn on (x,y)
 if (msg == "0 1 on")
 {
   setLED(0, 1);
 }

 if (msg == "0 1 off")
 {
   clearLED(0, 1);
 }
}

//=================================================================================================//
// functions to update pattern on board
//=================================================================================================//
void blinkLEDs()
{
  allOn();
  delay(1000);
  allOff();
  delay(1000);
}

static void allOff()
{
  strip.clear();
  strip.show();
}

static void allOn()
{
  for (uint16_t i = 0; i < N_LEDS; i++)
  {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

// light up a single LED
void setLED(int x, int y)
{
  strip.setPixelColor(matrix[x][y], c);
  strip.show();
}

// turn off a single LED
void clearLED(int x, int y)
{
  strip.setPixelColor(matrix[x][y], strip.Color(0, 0, 0, 0));
  strip.show();
}
