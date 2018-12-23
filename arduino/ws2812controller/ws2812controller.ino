/*
 Serial Call and Response in ASCII
 Language: Wiring/Ard'ino
 
 A signal in to the serial interface will be converted to the correct function
 */
#include <Adafruit_NeoPixel.h>
int DEVICE = 2;
String inNumber = "";    // string to hold input
String inCommand = "";    // string to hold input
String inValue = "";    // string to hold input
int chnr = 0; // Char number
int state = HIGH;       // The input state
int value12 = LOW;
int blinking = 0;
int train = 0;
int number=0;
int counter=0;
int pwmcounter=0;
int blink [14];
int pwm [14];
int en [14];
unsigned long time = 0;

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
  for(int ch = 2; ch<14; ch++){
    blink[ch]=0;
    pwm[ch]=0;
    en[ch]=0;
  }
  pinMode(2, OUTPUT);   
  pinMode(3, OUTPUT);   
  pinMode(4, OUTPUT);   
  pinMode(5, OUTPUT);   
  pinMode(6, OUTPUT);   
  pinMode(7, OUTPUT);   
  pinMode(8, OUTPUT);   
  pinMode(9, OUTPUT);   
  pinMode(10, OUTPUT);   
  pinMode(11, OUTPUT);   
  pinMode(12, OUTPUT);  
  pinMode(13, OUTPUT);  
  pinMode(A0, INPUT);
  //establishContact();  // send a byte to establish contact until receiver responds 
}
void toggleLED(int channel, int value){
        Serial.print(channel);
        Serial.print(":"); 
        if (channel>1 && channel<14){
          //en[channel]=en[channel]==0?1:0;
          en[channel]=value;
          if (en[channel]==1 && pwm[channel]==0){
            pwm[channel]=255;
          }
          Serial.print(en[channel]);
          Serial.print(":"); 
          Serial.print(pwm[channel]);
          Serial.print(":"); 
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
      Serial.println("A0:0");
      blinking=16;
      time=0;
    }
    state=LOW;
  }
  else 
    if (state==LOW && train>0) {
      Serial.println("A0:1");
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
   for(int ch = 2; ch<14; ch++){
    if (blink[ch]){
       toggleLED(ch,en[ch]==0?1:0);
    }
       
   }
   counter=0;
  }
  counter++;
  // if we get a valid byte, read analog ins:
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (inChar == '?'){ //char 0-2
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
    else if (isDigit(inChar) && chnr>2 && chnr<5){ //char 3-4
      inNumber += (char)inChar;
      chnr++;
    }
    else if (isDigit(inChar) && chnr> 4 && chnr<8){ //char 5-7
      inValue += (char)inChar;
      chnr++;
    }
    else if (inChar == '\n') {
      if (inCommand=="PWM"){ 
        int fadeValue = inValue.toInt();
        int channel = inNumber.toInt();
        if (fadeValue > 255)
          fadeValue = 255;
        pwm[channel]=fadeValue;
        toggleLED(channel, 1);
      }
      else if (inCommand== "ENA"){
        int channel = inNumber.toInt();
        int value = inValue.toInt();
        toggleLED(channel, value>0?1:0);

      }
      else if (inCommand== "BLI"){
        int channel = inNumber.toInt();
        if (channel>0 && channel<14) blink[channel]=blink[channel]==0?1:0;
      }
      else if (inCommand== "STA"){
        for(int ch=2; ch<14; ch++){
          Serial.print(ch);
          Serial.print(":");
          Serial.print(en[ch]);
          Serial.print(":");
          Serial.print(pwm[ch]);
          Serial.print(":");
          Serial.println(blink[ch]);
        }
      }
      else if (inCommand== "TRN"){
        int fadeValue = inValue.toInt();
        int channel = inNumber.toInt();
        train = channel;
        Serial.print("TRN:");
        Serial.println(channel);
      }
      else if (inCommand== "NUM"){
        int fadeValue = inValue.toInt();
        int channel = inNumber.toInt();
        number = channel*20;
        Serial.print("NUM:");
        Serial.println(channel);
      }
      else if (inCommand== "BLU"){
        int fadeValue = inValue.toInt();
        int channel = inNumber.toInt();
        Serial.print("BLI:");
        blinking=channel;
        Serial.println(blinking);
        time=0;
      }
      else if (inCommand== "ID" || inCommand == "ID?"){
        Serial.print("PWMtool:");
        Serial.print(DEVICE);
      }
      else if (inCommand== "VER"){
        Serial.println("1.1");
      }
      else if (inCommand== "FRQ"){
        int value = inValue.toInt();
        int channel = inNumber.toInt();
        if (channel == 5 || channel == 6){
          if (value==1) {setPwmFrequency(6 , 1);
          Serial.println("PIN5&6:62500");}
          if (value==2) {setPwmFrequency(6 , 8);
          Serial.println("PIN5&6:7812");}
          if (value==3) {setPwmFrequency(6 , 64);
          Serial.println("PIN5&6:977");}
          if (value==4) {setPwmFrequency(6 , 256);
          Serial.println("PIN5&6:62.5k/256");}
          if (value==5) {setPwmFrequency(6 , 1024);
          Serial.println("PIN5&6:62.5K/1024");}
        }
        if (channel == 9 || channel == 10){
          if (value==1) {setPwmFrequency(9 , 1);
          Serial.println("PIN9&10:62500");}
          if (value==2) {setPwmFrequency(9 , 8);
          Serial.println("PIN9&10:7812");}
          if (value==3) {setPwmFrequency(9 , 64);
          Serial.println("PIN9&10:977");}
          if (value==4) {setPwmFrequency(9 , 256);
          Serial.println("PIN9&10:62.5k/256");}
          if (value==5) {setPwmFrequency(9 , 1024);
          Serial.println("PIN9&10:62.5K/1024");}
        }
        if (channel == 3 || channel == 11){
          if (value==1) {setPwmFrequency(3 , 1);
          Serial.println("PIN3&11:31250");}
          if (value==2) {setPwmFrequency(3 , 8);
          Serial.println("PIN3&11:31K/8");}
          if (value==3) {setPwmFrequency(3 , 32);
          Serial.println("PIN3&11:31K/32");}
          if (value==4) {setPwmFrequency(3 , 64);
          Serial.println("PIN3&11:31K/64");}
          if (value==5) {setPwmFrequency(3 , 128);
          Serial.println("PIN3&11:31K/128");}
          if (value==6) {setPwmFrequency(3 , 256);
          Serial.println("PIN3&11:31K/256");}
          if (value==7) {setPwmFrequency(3 , 1024);
          Serial.println("PIN3&11:31K/1024");}
        }
      }
      else if (inCommand== "?" || inCommand== "HEL"){
        Serial.println("ID - Returns ID of the device");
        Serial.println("VER - Returns the SW Version");
        Serial.println("PWMxxyyy - Sets PWM duty for ch xx to yyy(0-255)");
        Serial.println("ENAxxyyy - Enable pin xx, set to yyy (1 or 0)");
        Serial.println("BLIxx - Set blinking ch xx");
        Serial.println("FRQ - Sets the PWM FREQ");
        Serial.println("PWM PINs 9 and 6 are used, freq are not the same");
      }
      else{
        Serial.print("Unknown Command <");
        Serial.print(inCommand);
        Serial.println(">");
      }
      // clear the string for new input:
      inCommand="";
      inValue = "";
      inNumber = "";
      chnr=0;
      
    }
    else {
      Serial.println("Unknown command");
      inCommand="";
      inValue = "";
      inNumber = "";
      chnr=0;
    }
    
  }
  
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("0,0,0");   // send an initial string
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

