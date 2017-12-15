/*******************
 * KakServer
 * 
 * En webserver för att hantera styrningen av
 * LEDar i pepparkaksbaket. Tanken är att servern
 * sitter i pepparkaksbygget och sedan styr de
 * olika 
 * 
 *******************/
#include <Ticker.h>

Ticker flipper;
const int NoOfLeds = 10;

typedef struct Colors {
  uint8 red;
  uint8 green;
  uint8 blue;  
} Color;

Color leds[10];

int count = 0;

void flip()
{
  int state = digitalRead(1);  // get the current state of GPIO1 pin
  digitalWrite(1, !state);     // set pin to the opposite state
  
  ++count;
  // when the counter reaches a certain value, start blinking like crazy
  if (count == 20)
  {
    flipper.attach(0.1, flip);
  }
  // when the counter reaches yet another value, stop blinking
  else if (count == 120)
  {
    flipper.detach();
  }
}

void setup() {
  pinMode(1, OUTPUT);
  digitalWrite(1, LOW);
  
  // flip the pin every 0.3s
  flipper.attach(0.3, flip);
}

void loop() {
}
