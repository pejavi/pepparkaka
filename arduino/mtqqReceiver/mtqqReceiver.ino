/*
 Serial Call and Response in ASCII
 Language: Wiring/Ard'ino
 
 A signal in to the serial interface will be converted to the correct function
 */
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID       "BoboLina2G"
#define WLAN_PASS       "Lerenius1"
WiFiClient client;

#define AIO_SERVER      "192.168.1.5"
#define AIO_SERVERPORT  1883
#define USER ""
#define PW ""
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT);
Adafruit_MQTT_Subscribe pumpT = Adafruit_MQTT_Subscribe(&mqtt, "pump/t");
Adafruit_MQTT_Subscribe krukT = Adafruit_MQTT_Subscribe(&mqtt, "krukhus/t");
Adafruit_MQTT_Subscribe vinH = Adafruit_MQTT_Subscribe(&mqtt, "vin/h");
Adafruit_MQTT_Subscribe brightnessMQTT = Adafruit_MQTT_Subscribe(&mqtt, "display/brightness");
#define LEDS 20
#define WHITE_
uint8_t r [LEDS];
uint8_t g [LEDS];
uint8_t b [LEDS];
#ifdef WHITE
  uint8_t w [LEDS];
#endif
int counter=0;
uint8_t setLed=0;
uint8_t brightness=2;
#define PIN D3
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
// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
//void MQTT_connect();

void setup()
{
  // start serial port at 9600 bps and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  for(int led = 0; led<LEDS; led++){
    r[led]=0;
    g[led]=0;
    b[led]=0;
    #ifdef WHITE
    w[led]=0;
    #endif
  }
  strip.begin();
  
  //strip.show(); // Initialize all pixels to 'off
  setColors();

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&pumpT);
  mqtt.subscribe(&krukT);
  mqtt.subscribe(&vinH); 
  mqtt.subscribe(&brightnessMQTT); 
}
// Fill the dots one after the other with a color
void setColors() {
 for(uint8_t i=0; i<strip.numPixels(); i++) {
  #ifdef WHITE
    strip.setPixelColor(i, strip.Color(r[i], g[i], b[i], w[i]));
  #else
    strip.setPixelColor(i, strip.Color(r[i], g[i], b[i]));
  #endif
 }
 strip.show();
}

void loop()
{
  
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();
  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  if ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &pumpT) {
      unsigned char * inChar = pumpT.lastread; 
      String in = "";
      uint8_t ch = 0;
      while (ch<4){
       //if(*inChar == '\0')
       //  break;
       if(isDigit(*inChar) || (in.length()==0 && *inChar == '-') || *inChar == '.'){
         in += (char)*inChar;
       }
       else break;
       inChar++;
       ch++;
      }
      if (ch>0) {
        float value = in.toFloat();
        Serial.print(F("Got pump temp: "));
        Serial.print((char *)pumpT.lastread);
        Serial.print("=>");
        Serial.println(value);
        /*if (value<LEDS){
          r[value]=10;
        }*/
        if (value>0) for(uint8_t led = 0; led<LEDS; led++){
          r[led]=0;
          g[led]=0;
          b[led]=led<value-0.5?brightness:0;
          #ifdef WHITE
          w[led]=0;
          #endif
        }
        else for(uint8_t led = 0; led<LEDS; led++){
          r[led]=0;
          g[led]=0;
          b[led]=led%2==0?(brightness<100?100:brightness):0;
          #ifdef WHITE
          w[led]=0;
          #endif
        }
        setColors();
      }
      else Serial.println((char *)pumpT.lastread);
    }
    else if (subscription == &krukT) {
      unsigned char * inChar = krukT.lastread; 
      String in = "";
      uint8_t ch = 0;
      while (ch<4){
       //if(*inChar == '\0')
       //  break;
       if(isDigit(*inChar) || (in.length()==0 && *inChar == '-') || *inChar == '.'){
         in += (char)*inChar;
       }
       else break;
       inChar++;
       ch++;
      }
      if (ch>0) {
        float value = in.toFloat();
        Serial.print(F("Got kruk temp: "));
        Serial.print((char *)krukT.lastread);
        Serial.print("=>");
        Serial.println(value);
        /*if (value<LEDS){
          r[value]=10;
        }*/
        if (value>0) for(uint8_t led = 0; led<LEDS; led++){
          r[led]=0;
          g[led]=led<value-0.5?brightness:0;
          b[led]=0;
          #ifdef WHITE
          w[led]=0;
          #endif
        }
        else for(uint8_t led = 0; led<LEDS; led++){
          r[led]=0;
          g[led]=led%2==0?(brightness<100?100:brightness):0;
          b[led]=0;
          #ifdef WHITE
          w[led]=0;
          #endif
        }
        setColors();
      }
      else Serial.println((char*) inChar);
    }
    else if (subscription == &vinH) {
      unsigned char * inChar = vinH.lastread; 
      String in = "";
      uint8_t ch = 0;
      while (ch<4){
       //if(*inChar == '\0')
       //  break;
       if(isDigit(*inChar) || (in.length()==0 && *inChar == '-') || *inChar == '.'){
         in += (char)*inChar;
       }
       else break;
       inChar++;
       ch++;
      }
      if (ch>0) {
        float value = in.toFloat()/5.0;
        Serial.print(F("Got vin humidity: "));
        Serial.print((char *)vinH.lastread);
        Serial.print("/5=>");
        Serial.println(value);
        /*if (value<LEDS){
          r[value]=10;
        }*/
        for(uint8_t led = 0; led<LEDS; led++){
          r[led]=led<value-0.5?(value>80?100:brightness):0;
          g[led]=0;
          b[led]=0;
          #ifdef WHITE
          w[led]=0;
          #endif
        }
        setColors();
      }
      else Serial.println((char *)inChar);
    }
    else if (subscription == &brightnessMQTT) {
      unsigned char * inChar = brightnessMQTT.lastread; 
      String in = "";
      uint8_t ch = 0;
      while (ch<4){
       //if(*inChar == '\0')
       //  break;
       if(isDigit(*inChar)){
         in += (char)*inChar;
       }
       else break;
       inChar++;
       ch++;
      }
      if (ch>0) {
        uint8_t value = in.toInt();
        Serial.print(F("Got Brightness: "));
        Serial.print((char *)brightnessMQTT.lastread);
        Serial.print("=>");
        Serial.println(value);
        /*if (value<LEDS){
          r[value]=10;
        }*/
        if (value>255) value=255;
        if (value<0) value=0;
        brightness=value;
        for(uint8_t led = 0; led<LEDS; led++){
          if (r[led]>0) r[led]=brightness;
          if (g[led]>0) g[led]=brightness;
          if (b[led]>0) b[led]=brightness;
          #ifdef WHITE
          if (w[led]>0) w[led]=brightness;
          #endif
        }
        setColors();
      }
      else Serial.println((char *)brightnessMQTT.lastread);
    }
    else
      Serial.println("No message");
  }
  //if(! mqtt.ping()) {
  //  mqtt.disconnect();
  //  Serial.println("Ping failed");
  //}
  /*if(counter>=100){
    counter=0;
    Serial.print("Setting color on led ");
    Serial.println(setLed);
    for(int led = 0; led<LEDS; led++){
      r[led]=led==setLed?100:0;
      g[led]=0;
      b[led]=0;
      #ifdef WHITE
      w[led]=0;
      #endif
    }
    setColors();
    setLed++;
    if (setLed==LEDS){
      setLed=0;
    }
  }
  else
    counter++;
    */
}
// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 1;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       mqtt.disconnect();
       retries--;
       if (retries == 0) {
         Serial.println("MQTT not connected");
         return;
       }
       else{
        Serial.println("Retrying MQTT connection in 5 seconds...");
        delay(5000);  // wait 5 seconds
       }
  }
  Serial.println("MQTT Connected!");
}
