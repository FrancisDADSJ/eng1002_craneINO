//Libraries used
//#include <LiquidCrystal.h>

#include <Adafruit_SSD1306.h> //display libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Wire.h> //Interfaces for the display
#include <SPI.h>




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
char motorStopMessage[]="MOTOR TURNED OFF";
char motorDirection1[]="ACW";
char motorDirection2[]="CW";
String directionMessage = "Direction: ";
String speedMessage1= "Speed: ";
char speedMessage2 = '%';
String rotationMessage1 = "Rotations: ";

//Delay definitions
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


void setup() {
    Serial.begin(9600);
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
    attachInterrupt(digitalPinToInterrupt(StopSwitch),stopMotor,FALLING);  
    display.setRotation(3); //values 1 - 4
    display.setTextSize(1); // from 1 to 8 [pixel 6x8 for size=1] with 1 pixel spacing
    display.setTextColor(WHITE, BLACK);
    display.clearDisplay();
    display.display();

  	timeNow = millis();
  	while(millis()<timeNow+lcdStartPeriod){}

}

void loop() {
  //noInterrupts();


  if(digitalRead(ClockwiseSwitch) == LOW){
      motorDirection = true;
  }
  else{}
  if(digitalRead(AntiClockwiseSwitch) == LOW){
     motorDirection = false;
  }
  else{}
  //Reading Motor Control Speed Potentiometer
  timeNow = millis();
  potValue = analogRead(PotPin);
  analogWrite(MotorEnable,potValue/4);
  //Direction Logic
 // motorDirection = digitalRead(MotorModePin);

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
  // Display Messages
  display.setCursor(0,20);
  display.println(speedMessage1+percentageSpeed+speedMessage2);
  display.display();

  display.setCursor(0,40);
  rotationsDisplay = rotationCount/2;
  display.print(rotationMessage1+rotationsDisplay);
  display.display();
  while(millis()< timeNow+period){}
  display.clearDisplay();
  
}
//Photodiode Control
void RotationCount(){
  if (motorDirection == true){
	rotationCount++;}
  else{rotationCount--;}
}

void stopMotor(){
 bool holder = true;
 do
    { 
      digitalRead(AntiClockwiseSwitch);
      display.clearDisplay();
      analogWrite(MotorEnable,0);
      display.setCursor(10,0); //aw
      display.println(motorStopMessage);
      display.display();
      display.setCursor(10,15); //aw
      display.println("Press SW2 or SW1 to turn it on");
      display.display();
      timeNow = millis();
      while(millis()< timeNow+period){}

      if(holder == true){

      holder = digitalRead(ClockwiseSwitch);

      if(holder == true){
        holder = digitalRead(AntiClockwiseSwitch);
        Serial.println("This has been written off 2");
        }
      else{}

      }
      else{}
      }
      while(holder ==true);
    }
    

