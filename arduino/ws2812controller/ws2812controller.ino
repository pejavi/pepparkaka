/*
 Serial Call and Response in ASCII
 Language: Wiring/Ard'ino
 
 A signal in to the serial interface will be converted to the correct function
 */
#include <Adafruit_NeoPixel.h>
#define DEVICE 2
#define LEDS 45
#define WHITE
String inNumber="   ";    // string to hold input
String inCommand = "   ";    // string to hold input
String inR="   ";    // string to hold input
String inG="   ";    // string to hold input
String inB="   ";    // string to hold input
String inW="   ";
String inValue="   ";
uint8_t chnr = 0; // Char number
uint8_t state = HIGH;       // The input state
uint8_t value12 = LOW;
uint8_t blinking = 0;
uint8_t train = 0;
int counter=0;
int pwmcounter=0;
uint8_t blink [LEDS];
uint8_t pwm [LEDS];
uint8_t en [LEDS];
uint8_t r [LEDS];
uint8_t g [LEDS];
uint8_t b [LEDS];
uint8_t w [LEDS];
unsigned long time = 0;
#define PIN 2
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flagqs, add together as needed:
// NEO_KHZ800 800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
// NEO_KHZ400 400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
// NEO_GRB Pixels are wired for GRB bitstream (most NeoPixel products)
// NEO_RGB Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
// NEO_RGBW Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
#ifdef WHITE
   Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, PIN, NEO_GRBW + NEO_KHZ800);
#else
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, PIN, NEO_GRB + NEO_KHZ800);
#endif
void setup()
{
  //Setup PWM
  setPwmFrequency(9 , 1);
  setPwmFrequency(3 , 1);
  setPwmFrequency(6 , 1);
  // start serial port at 9600 bps and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  for(int ch = 0; ch<LEDS; ch++){
    blink[ch]=0;
    pwm[ch]=0;
    en[ch]=0;
  }
  inCommand="";
  inValue="";
  inR="";
  inG="";
  inB="";
  inW="";
  inNumber="";
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  //establishContact();  // send a byte to establish contact until receiver responds 
}
// Fill the dots one after the other with a color
void setColors() {
 for(uint8_t i=0; i<strip.numPixels(); i++) {
  #ifdef WHITE
    strip.setPixelColor(i, en[i]==0?strip.Color(0, 0, 0 ,0):strip.Color(r[i], g[i], b[i], w[i]));
  #else
    strip.setPixelColor(i, en[i]==0?strip.Color(0, 0, 0):strip.Color(r[i], g[i], b[i]));
  #endif
 }
 strip.show();
}
void toggleLED(uint8_t channel, uint8_t value){
        Serial.print(channel);
        Serial.print(F(":")); 
        if (channel>1 && channel<14){
          //en[channel]=en[channel]==0?1:0;
          en[channel]=value;
          if (en[channel]==1 && pwm[channel]==0){
            pwm[channel]=255;
          }
          Serial.print(en[channel]);
          Serial.print(F(":")); 
          Serial.print(pwm[channel]);
          Serial.print(F(":")); 
          Serial.println(blink[channel]);
          if (pwm[channel]==255 || en[channel]==0) 
            digitalWrite(channel, en[channel]==1?HIGH:LOW);
          else 
            analogWrite(channel, pwm[channel]);
        }
}

void loop()
{
  value12 = analogRead(A0);
  time++;
  if (value12<100 && train>0) {
    if (state==HIGH){
      Serial.println(F("A0:0"));
      blinking=16;
      time=0;
    }
    state=LOW;
  }
  else 
    if (state==LOW && train>0) {
      Serial.println(F("A0:1"));
      state=HIGH;
    }
  if (blinking > 0){
    if (time==5000){
     time=0;
     blinking--;
     if (blinking == 0){
       digitalWrite(8, LOW);
       digitalWrite(7, LOW);
       digitalWrite(2, HIGH);
     }
     else if (blinking%2==0) {
       digitalWrite(8, HIGH);
       digitalWrite(7, LOW);
       digitalWrite(2, LOW);
     }
     else{
       digitalWrite(8, LOW);
       digitalWrite(7, HIGH);
       digitalWrite(2, LOW);
     }
    }
  }
  if (counter==10000){
   for(uint8_t ch = 0; ch<LEDS; ch++){
    if (blink[ch]){
       //toggleLED(ch,en[ch]==0?1:0);
       en[ch]=en[ch]==0?1:0;
       setColors();
    }
       
   }
   counter=0;
  }
  counter++;
  // if we get a valid byte, read analog ins:
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (inChar == '?' && chnr<3){ //char 0-2
      inCommand += (char)inChar;
      chnr++;
    }
    else if (isAlpha(inChar) && chnr < 3) { //char 0-2
      inCommand += (char)inChar;
      chnr++;
    }
    else if (isAlpha(inChar)) { //char 3+
      chnr++;
    }
    else if (isDigit(inChar) && chnr>=3 && chnr<=4){ //char 3-4
      inNumber += (char)inChar;
      chnr++;
    }
    else if (isDigit(inChar) && chnr>=5 && chnr<=7){ //char 5-7
      inR += (char)inChar;
      chnr++;
    }
    else if (isDigit(inChar) && chnr>=8 && chnr<=10){ //char 8-10
      inG += (char)inChar;
      chnr++;
    }
    else if (isDigit(inChar) && chnr>=11  && chnr<=13){ //char 11-13
      inB += (char)inChar;
      chnr++;
    }
    else if (isDigit(inChar) && chnr>=14  && chnr<=16){ //char 14-16
      inW += (char)inChar;
      chnr++;
    }
    else if (inChar == '\n') {
      if (inCommand=="RGB"){ 
        uint8_t channel = inNumber.toInt();
        if (channel<LEDS){
         r[channel] = inR.toInt();
         g[channel] = inG.toInt();
         b[channel] = inB.toInt();
         #ifdef WHITE
          w[channel] = inW.toInt();
         #endif
         en[channel] = 1;
         setColors();
        }
      }
      else if (inCommand== "ENA"){
        uint8_t ch = inNumber.toInt();
        //int value = inValue.toInt();
        //toggleLED(channel, value>0?1:0);
        if (ch<LEDS) en[ch]=1;
        setColors();
      }
      else if (inCommand== "DIS"){
        uint8_t ch = inNumber.toInt();
        //int value = inValue.toInt();
        //toggleLED(channel, value>0?1:0);
        if (ch<LEDS) en[ch]=0;
        setColors();
      }
      else if (inCommand== "BLI"){
        uint8_t channel = inNumber.toInt();
        if (channel>=0 && channel<LEDS){
          blink[channel]=blink[channel]==0?1:0;
        }
      }
      else if (inCommand== "STA"){
        for(uint8_t ch=0; ch<LEDS; ch++){
          Serial.print(ch);
          Serial.print(F(":"));
          Serial.print(en[ch]);
          Serial.print(F(":"));
          Serial.print(r[ch]);
          Serial.print(F(":"));
          Serial.print(g[ch]);
          Serial.print(F(":"));
          Serial.print(b[ch]);
          Serial.print(F(":"));
          Serial.print(w[ch]);
          Serial.print(F(":"));
          Serial.println(blink[ch]);
        }
      }
      else if (inCommand== "TRN"){
        uint8_t fadeValue = inValue.toInt();
        uint8_t channel = inNumber.toInt();
        train = channel;
        Serial.print(F("TRN:"));
        Serial.println(channel);
      }
      else if (inCommand== "BLU"){
        int fadeValue = inValue.toInt();
        int channel = inNumber.toInt();
        Serial.print(F("BLI:"));
        blinking=channel;
        Serial.println(blinking);
        time=0;
      }
      else if (inCommand== "ID" || inCommand == "ID?"){
        Serial.print(F("RGBW:"));
        Serial.print(LEDS);
        Serial.print(F(":"));
        Serial.println(DEVICE);
      }
      else if (inCommand== "VER"){
        Serial.println(F("1.1"));
      }
      else if (inCommand== "?" || inCommand== "HEL"){
        Serial.println(F("ID - Returns ID of the device"));
        Serial.println(F("VER - Returns the SW Version"));
        Serial.println(F("PWMxxyyy - Sets PWM duty for ch xx to yyy(0-255)"));
        Serial.println(F("ENAxx - Enable ch xx"));
        Serial.println(F("DISxx - Disable ch xx"));
        Serial.println(F("RGBchrrrgggbbbwww - set color on ch xx"));
        Serial.println(F("BLIxx - Set blinking ch xx"));
      }
      else{
        Serial.print(F("Unknown Command <"));
        Serial.print(inCommand);
        Serial.println(F(">"));
      }
      // clear the string for new input:
      inCommand="";
      inValue = "";
      inNumber = "";
      inB = "";
      inG = "";
      inR = "";
      #ifdef WHITE
        inW = "";
      #endif
      chnr=0;
    }
    else {
      //Serial.println("Unknown command");
      //inCommand="";
      //inValue = "";
      //inNumber = "";
      //chnr=0;
    }
    
  }
  
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println(F("0,0,0"));   // send an initial string
    delay(300);
  }
}
/**
 * Divides a given PWM pin frequency by a divisor.
 * 
 * The resulting frequency is equal to the base frequency divided by
 * the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
 *        256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
 *        128, 256, and 1024.
 * 
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired on timer0
 *   - Pins 9 and 10 are paired on timer1
 *   - Pins 3 and 11 are paired on timer2
 * 
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 3, 5, 6, or 11 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function
 *     incorrectly.
 * 
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *   http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1235060559/0#4
 */
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}

