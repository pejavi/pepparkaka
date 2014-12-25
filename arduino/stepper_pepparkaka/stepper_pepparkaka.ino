/*
   BYJ48 Stepper motor code
   Connect :
   IN1 >> D8
   IN2 >> D9
   IN3 >> D10
   IN4 >> D11
   VCC ... 5V Prefer to use external 5V Source
   Gnd
   written By :Mohannad Rawashdeh
  http://www.instructables.com/member/Mohannad+Rawashdeh/
     28/9/2013
  */
//Propeller
#define P1  8
#define P2  9
#define P3  10
#define P4  11
//Radar
#define R1  4
#define R2  5
#define R3  6
#define R4  7
//Styrpinnar
#define PROP_CTRL 12
#define RADAR_LEFT 2
#define RADAR_RIGHT 3
//TEST
#define TEST 13

#define PROP  0
#define RADAR 1
#define LEFT 0
#define RIGHT 1

int propSteps = 0;
int radarSteps = 0;
boolean Direction = true;
unsigned long prop_last_time;
unsigned long radar_last_time;
unsigned long currentMillis ;
int prop_steps_left=0;
int prop_laps=0;
int oldPropState = LOW;
int propState = LOW;
int radar_steps_left=0;
int radarLeftState = LOW;
int oldRadarLeftState = LOW;
int radarRightState = LOW;
int oldRadarRightState = LOW;

void setup()
{
  Serial.begin(115200);
  // Propeller
  pinMode(P1, OUTPUT); 
  pinMode(P2, OUTPUT); 
  pinMode(P3, OUTPUT); 
  pinMode(P4, OUTPUT);
  // Radar
  pinMode(R1, OUTPUT); 
  pinMode(R2, OUTPUT); 
  pinMode(R3, OUTPUT); 
  pinMode(R4, OUTPUT);
  // CTRL
  pinMode(PROP_CTRL, INPUT);
  pinMode(RADAR_LEFT, INPUT);
  pinMode(RADAR_RIGHT,INPUT);
  
  pinMode(TEST,OUTPUT);
  digitalWrite(TEST,HIGH);
}

void loop()
{
  currentMillis = micros();
  
  // Turn Prop if we should
  if(prop_steps_left > 0) {
    if(currentMillis-prop_last_time>=1000){
      stepper(1,PROP);
      prop_last_time=micros();
      prop_steps_left--;
    }    
  }
  propState = digitalRead(PROP_CTRL);
  if (propState != oldPropState) {
    prop_steps_left=4095*3;
    oldPropState = propState;
    Serial.println("Prop activated");
    delay(2);
  }

  // Turn Radar if we should
  if(radar_steps_left > 0) {
    if(currentMillis-radar_last_time>=1000){
      stepper(1,RADAR);
      radar_last_time=micros();
      radar_steps_left--;
    }    
  }
  radarLeftState = digitalRead(RADAR_LEFT);
  if (radarLeftState != oldRadarLeftState) {
    radar_steps_left=511;
    oldRadarLeftState = radarLeftState;
    Direction = LEFT;
    Serial.println("Radar left activated");
    delay(20);
  }
  radarRightState = digitalRead(RADAR_RIGHT);
  if (radarRightState != oldRadarRightState) {
    radar_steps_left=511;
    oldRadarRightState = radarRightState;
    Direction = RIGHT;
    Serial.println("Radar right activated");
    delay(20);
  }  
}

void stepper(int xw, int type){
  int IN1=R1;
  int IN2=R2;
  int IN3=R3;
  int IN4=R4;
  int Steps = radarSteps;
  //If propeller:
  if (type == PROP){
    IN1=P1;
    IN2=P2;
    IN3=P3;
    IN4=P4;
    Steps = propSteps;
  }
  
  for (int x=0;x<xw;x++){
   switch(Steps){
   case 0:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, HIGH);
   break; 
   case 1:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, HIGH);
   break; 
   case 2:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
   break; 
   case 3:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, HIGH);
     digitalWrite(IN4, LOW);
   break; 
   case 4:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
   case 5:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, HIGH);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
     case 6:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
   case 7:
     digitalWrite(IN1, HIGH); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, HIGH);
   break; 
   default:
     digitalWrite(IN1, LOW); 
     digitalWrite(IN2, LOW);
     digitalWrite(IN3, LOW);
     digitalWrite(IN4, LOW);
   break; 
}
SetDirection(type);
}
}

void SetDirection(int type){
  if(type == RADAR) {
    if(Direction==1){ radarSteps++; }
    if(Direction==0){ radarSteps--; }
    if(radarSteps>7){ radarSteps=0; }
    if(radarSteps<0){ radarSteps=7; }
  } else {
    propSteps++;
    if(propSteps>7){ propSteps=0; }
    if(propSteps<0){ propSteps=7; }
  }
}

