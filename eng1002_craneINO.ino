//Libraries used
//#include <LiquidCrystal.h>
#include <Adafruit_SSD1306.h> //display libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Wire.h> //Interfaces for the display
#include <SPI.h>
//LoadCell definition
#define LoadCellPin A6
//Motor pin definitions
#define MotorEnable 5
#define MotorInput1 4
#define MotorInput2 3
#define PotPin A7
#define ClockwiseSwitch 12
#define AntiClockwiseSwitch 11
#define StopSwitch 13
//Photodiode definitions
#define PhotoDiodePin 2
Adafruit_SH1107 display = Adafruit_SH1107(64,128,&Wire);
String motorStopMessage1="MOTOR TURNED OFF";
String motorStopMessage2="Press SW3 to POWER ON";
char motorDirection1[]="ACW";
char motorDirection2[]="CW";
String directionMessage = "Direction: ";
String speedMessage1= "Speed: ";
char speedMessage2 = '%';
String rotationMessage1 = "Rotations: ";

//Delay variables
int period = 10;
int lcdStartPeriod = 200;
unsigned long timeNow = 0;
//Motor variables                      
int motorSpeed = 0;
int count=0;
int potValue = 0;
int percentageSpeed = 0;
bool motorDirection = true;
int rotationCount = 0;
int rotationsDisplay = 0;
int countStop = 0;
int holder = 0;
//LoadCell Amp values
int loadCellByte = 0;
double loadCellVoltage = 0;
double loadCellVoltageConvConstant = 0.003226;
String messageLoad = " Load: ";
String loadValueMesssge="";
String newtonSymbol = " N";
float loadInNewtons = 0.0;
float gravitationalAcceleration= 9.81;
float massConvConstant = 1.89;
float mass=0.0;
// Comparable variables for output

//int upperSpeed;//for upper boundary of speed variation
//int lowerSpeed;//for lower boundary of speed variation
//float currentSpeed;// store current speed value
//float oldSpeed;// store previous speed value
float upperMass;// for upper boundary of load variation
float lowerMass;// for lower boundary of load variation
float currentMass;// store current mass value
float oldMass;// store previous mass value
// Serial Place holder
String speedHolder = "s";


String loadValueMessage;


String serialLoad;
String serialSpeed;
String serialDirection;
String serialRotation;
void setup() {
    Serial.begin(115200);
    delay(500);
    display.begin(0x3C,true);
    pinMode(MotorEnable,OUTPUT);
    pinMode(MotorInput1,OUTPUT);
    pinMode(MotorInput2,OUTPUT);
    pinMode(PotPin,INPUT);
  	pinMode(ClockwiseSwitch,INPUT);
    pinMode(AntiClockwiseSwitch,INPUT);
  	pinMode(PhotoDiodePin,INPUT);
    pinMode(13,INPUT);
  	attachInterrupt(digitalPinToInterrupt(PhotoDiodePin),RotationCount,RISING);
    display.setRotation(3); //values 1 - 4
    display.setTextSize(1); // from 1 to 8 [pixel 6x8 for size=1] with 1 pixel spacing
    display.setTextColor(WHITE, BLACK);
    display.clearDisplay();
    display.display();

  	timeNow = millis();
  	while(millis()<timeNow+lcdStartPeriod){}

}

void loop() {
 
  if(digitalRead(StopSwitch)==1){

  if(digitalRead(ClockwiseSwitch) == LOW){
      motorDirection = true;
      String rotationDirectionHolder1 = "d0" ;
      Serial.println(rotationDirectionHolder1);
  }
  else{}
  if(digitalRead(AntiClockwiseSwitch) == LOW){
     motorDirection = false;
     String rotationDirectionHolder2 = "d1" ;
     Serial.println(rotationDirectionHolder2);
  }
  else{}
  //Reading Motor Control Speed Potentiometer
  timeNow = millis();
  potValue = analogRead(PotPin);
  analogWrite(MotorEnable,potValue/4);
  //Direction Logic
  if(motorDirection == true){
  	digitalWrite(MotorInput1,LOW);
  	digitalWrite(MotorInput2,HIGH);
    //clockwise
    display.setCursor(13,0); //cw 
    display.println(directionMessage+motorDirection2);
    display.display();
  }
  else{
  //anticlockwise
  digitalWrite(MotorInput1,HIGH);
  digitalWrite(MotorInput2,LOW);
   	display.setCursor(13,0); //aw
    display.println(directionMessage+motorDirection1);
    display.display();
    
  }
  //Speed Calculation
  percentageSpeed = ((potValue/4.0)/255.0)*100;
  
  String serialPercentageSpeed = speedHolder+percentageSpeed;

  Serial.println(serialPercentageSpeed);

  // Display Messages
  display.setCursor(0,20);
  display.println(speedMessage1+percentageSpeed+speedMessage2);
  display.display();
  //Display Load
 // display.setCursor(0,)
  CheckAndPrintCraneLoad();
  display.setCursor(0,40);
  rotationsDisplay = rotationCount/2;
  display.print(rotationMessage1+rotationsDisplay);
  display.display();
  while(millis()< timeNow+period){}
  display.clearDisplay();
  }
else{
      holder = 1;
      timeNow = millis();
      while(millis()<timeNow+250){}

  do{
    
      display.clearDisplay();
      analogWrite(MotorEnable,0);
      display.setCursor(10,0); //aw
      display.println(motorStopMessage1+motorStopMessage2);
      display.display();
      holder = digitalRead(StopSwitch);
      timeNow = millis();
      while(millis()<timeNow+250){}
      }
      while(holder == 1);
}

}
//Photodiode Control
void RotationCount(){
  if (motorDirection == true){
	rotationCount++;}
  else{rotationCount--;}
  String rotationHolder ="r";
  String serialCount = rotationHolder+rotationCount;
  Serial.println(serialCount);
}

void CheckAndPrintCraneLoad(){
  
loadCellByte = analogRead(LoadCellPin);
loadCellVoltage = loadCellByte * loadCellVoltageConvConstant;
currentMass = massConvConstant * loadCellVoltage;
upperMass = currentMass+0.25;
lowerMass = currentMass-0.25;
if(currentMass!=oldMass){
  
  if((oldMass<=lowerMass)||(oldMass >=upperMass)){
    String loadHolder = "l";
    loadInNewtons = (gravitationalAcceleration * currentMass) ;
    oldMass = currentMass; //Stores the current mass as old mass for the next check
    loadValueMessage = String(loadInNewtons,2);
    String serialLoad = loadHolder+loadValueMessage;
    Serial.println(serialLoad);
 	  
    }
  display.setCursor(0,50); //aw
    display.println(messageLoad+loadValueMessage+newtonSymbol);
    display.display();}
    
}



