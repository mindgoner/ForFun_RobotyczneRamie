// Load necessary libraries
#include <Servo.h>
#include <math.h>
#include <Wire.h>
#include <Sensor.h>
#include <LiquidCrystal_I2C.h>

// *****************************************************
// *                                                   *
// *          Configure default parameters             *
// *                                                   *
// *****************************************************

int buzzerPin = 2;
const int numberOfSensors = 8;
const int numberOfEngines = 11;
int sensorsOnline = 0;

// *****************************************************
// *                                                   *
// *     Initialize default classes and functions      *
// *                                                   *
// *****************************************************

void selectSensor(int i){
  Wire.beginTransmission(0x70);
  Wire.write(1 << i);
  Wire.endTransmission();
}

class ServoEngine{

  
  private:
    int enginePin = 0, engineAngle = 0;
    Servo engine;

    
  public:
  
    int setPin(int pin){
      if(pin != 0){
        this->engine.attach(pin);
        this->enginePin = pin;
        return 1;
      }else{
        this->engine.detach();
        return 0;
      }
    }
    
    int getPin(){
      return this->enginePin;
    }
    
    int setAngle(int targetAngle, int delayAfter = 0){
      int currentAngle = this->getAngle();
      if(targetAngle >= 0 && targetAngle <=180 && this->enginePin != 0){
        if(delayAfter == 0){
          this->engine.write(targetAngle);
          this->engineAngle = targetAngle;
        }else{

          // Delay defined, make moves a bit smoother.

          if(targetAngle > currentAngle){
            for(int a = currentAngle; a < targetAngle; a++){
              this->engine.write(a);
              this->engineAngle = a;
              delay(delayAfter);
            }  
          }else{
            for(int a = currentAngle; a > targetAngle; a--){
              this->engine.write(a);
              this->engineAngle = a;
              delay(delayAfter);
            }  
          }
          
        }
        return 1;  
      }else{
        return 0;
      }
    }
    
    int getAngle(){
      return this->engineAngle;
    }


};



Sensor Sensors[8] = {Sensor(Wire), Sensor(Wire), Sensor(Wire), Sensor(Wire), Sensor(Wire), Sensor(Wire), Sensor(Wire), Sensor(Wire)};
ServoEngine Engines[11] = {ServoEngine(), ServoEngine(), ServoEngine(), ServoEngine(), ServoEngine(), ServoEngine(), ServoEngine(), ServoEngine(), ServoEngine(), ServoEngine(), ServoEngine()};
LiquidCrystal_I2C screen(0x27,20,4);

void toneGiveSignal(int hms, int beep_type, int hi_delay, int af_delay){

  // Beep module
  // beep_type: 0 means error, 1 means success. Other values are customed frequences.
  // hms: how many signals
  
  if(beep_type == 0){
    beep_type = 200;  
  }else if(beep_type == 1){
    beep_type = 2500;  
  }
  for(int i = 1; i <= hms; i++){
    tone(buzzerPin, beep_type);
    delay(hi_delay);
    noTone(buzzerPin);
    delay(af_delay);
  }
}

void sensorsConfiguration(){
  do{
    screen.setCursor(0,0);
    screen.print("Loading sensors:");
    screen.setCursor(0,1);
    screen.print("[        ]      ");
    sensorsOnline = 0;

    // Initialize every sensor connected to TCA9548A I2C Multiplexer 
    for(int i=0; i<numberOfSensors;){

      // Display status (+ or -) on LCD screen:
      screen.setCursor(i+1,1);
      
      // Select specified sensor
      selectSensor(i);
      
      Sensors[i].begin();
      double calculationSucces = Sensors[i].calcGyroOffsets();
      Sensors[i].update();
      Serial.print("Sensor ");
      Serial.print(i);
      Serial.print(":\t");
      Serial.print(Sensors[i].getGyroAngleX());
      Serial.print("\t");
      Serial.print(Sensors[i].getGyroAngleY());
      Serial.print("\t");
      Serial.print(Sensors[i].getGyroAngleZ());
      Serial.print("\t[");
      Serial.print(calculationSucces);
      Serial.print("]\n");
      if(calculationSucces){
        sensorsOnline++;
        toneGiveSignal(1, 1, 100, 0);
        screen.print("+");
      }else{ // An error occured while configuring gyroOffsets
        toneGiveSignal(1, 0, 100, 0);
        screen.print("-");
      }
      i++;
    }
    delay(1000);
    if(sensorsOnline != numberOfSensors){
      toneGiveSignal(1, 0, 600, 50);
      screen.setCursor(0,0);
      screen.print("Sensors failure!");
      delay(2000);
    }else{
      toneGiveSignal(3, 1, 100, 50);
      screen.setCursor(0,0);
      screen.print("Sensors OK!     ");
    }
  }while(sensorsOnline != numberOfSensors);
}

int disableEngines(){
  Engines[0].setPin(0); // Bark
  Engines[1].setPin(0); // Biceps rotator
  Engines[2].setPin(0); // T-pose
  Engines[3].setPin(0); // Elbow flex
  Engines[4].setPin(0); // Index
  Engines[5].setPin(0); // ForeArm rotator
  Engines[6].setPin(0); // Ring
  Engines[7].setPin(0); // Pinky
  Engines[8].setPin(0); // Middle
  Engines[9].setPin(0); // Thumb (not working)
  Engines[10].setPin(0); // Wrist
}

int engineSetHome(int delayAfter){

  Serial.println("Default position loaded!");
  
  Engines[0].setPin(3); // Bark
  Engines[1].setPin(4); // Biceps rotator
  Engines[2].setPin(5); // T-pose
  Engines[3].setPin(6); // Elbow flex
  Engines[4].setPin(7); // Index
  Engines[5].setPin(8); // ForeArm rotator
  Engines[6].setPin(9); // Ring
  Engines[7].setPin(10); // Pinky
  Engines[8].setPin(11); // Middle
  Engines[9].setPin(12); // Thumb (not working)
  Engines[10].setPin(13); // Wrist
  
  Engines[0].setAngle(0, delayAfter); // 0 default, 180 increased, default 0
  Engines[1].setAngle(75, delayAfter); // 0 left rotation, 180 right rotation, default 75
  Engines[2].setAngle(105, delayAfter); // 0 TPose inside, 180 outside, default 105
  Engines[3].setAngle(180, delayAfter); // 0 elbow flexed, 180 straight, default 180
  Engines[5].setAngle(80, delayAfter); // 0 down hand, 180 top hand, default 80
  Engines[4].setAngle(0, delayAfter); // 0 straight index, 180 flexed index, default 0
  Engines[6].setAngle(0, delayAfter); // 0 straight ring, 180 flexed ring, default 0
  Engines[7].setAngle(0, delayAfter); // 0 straight pinky, 180 flexed pinky, default 0
  Engines[8].setAngle(0, delayAfter); // 0 straight middle, 180 flexed middle, default 0
  Engines[9].setAngle(0, delayAfter); 
  Engines[10].setAngle(90, delayAfter); // 0 reverse wrist, 180 flexed wrist, default 90
}

int reverse_rotator(int a){
  return 180 - a;
}

void clearLCD(){
  screen.setCursor(0,0);
  screen.print("                ");
  screen.setCursor(0,1);
  screen.print("                ");
}

int debugSensor(int s){
    Serial.print("Sensor: ");
    Serial.print(s);
    Serial.print("\t");
    Serial.print(Sensors[s].getGyroAngleX());
    Serial.print("\t");
    Serial.print(Sensors[s].getGyroAngleY());
    Serial.print("\t");
    Serial.print(Sensors[s].getGyroAngleZ());
    Serial.print("\n");
}

double abso(double x){
  return (x)>0?x:(-x);
}


// *****************************************************
// *                                                   *
// *          ADDITIONAL PREDEFINED MOVES              *
// *                                                   *
// *****************************************************


int wave(){

  Serial.println("Waving!");
  screen.setCursor(0,0);
  screen.print("Waving!");
  
  
  Engines[5].setAngle(0, 0);
  Engines[0].setAngle(30, 0);
  Engines[3].setAngle(0, 5);
  delay(1000);
  for(int i = 0; i<=3; i++){
    Engines[2].setAngle(70, 10);
    Engines[2].setAngle(110, 10);
  }
  Engines[2].setAngle(105, 5);
  delay(1000);
  engineSetHome(5);
}

int say_hi(){
  Serial.println("Give me a hand!");
  Engines[0].setAngle(100, 0);
  Engines[3].setAngle(100, 0);
  delay(3000);
  
  delay(3000);
  engineSetHome(5);
}

int i_dont_know(){
    Engines[3].setAngle(30, 0);
    Engines[10].setAngle(0, 5);
    Engines[1].setAngle(120, 5);
    Engines[5].setAngle(150, 5); // 0 down hand, 180 top hand, default 100
    delay(5000);
    engineSetHome(5);
}

int give_me_a_coin(){
   Engines[3].setAngle(75, 0);
   Engines[5].setAngle(180, 5);
   delay(5000);
   Engines[5].setAngle(0, 5);
   delay(2000);
   engineSetHome(5);
}

// *****************************************************
// *                                                   *
// *     FREESTYLE - USE SENSORS TO CONTROL ARM        *
// *                                                   *
// *****************************************************

void freestyle_arm(){
  
  toneGiveSignal(1, 1, 500, 0);
  sensorsConfiguration();

  Serial.println("Freestyle Arm activated!");
  
  screen.setCursor(0,0);
  screen.print("Freestyle Mode!");
  
  double calc;
  while(1==1){

    for(int s = 0; s < sensorsOnline; s++){
      selectSensor(s);
      Sensors[s].update();  
    }
    calc = reverse_rotator(abso(Sensors[1].getGyroAngleZ()) * 1.35 - abso(Sensors[0].getGyroAngleZ())); // Elbow  flexed position
    debugSensor(1);
    Engines[3].setAngle(calc, 10);
    
  }
}
void freestyle_hand(){
  
  toneGiveSignal(1, 1, 500, 0);
  sensorsConfiguration();
  Engines[5].setAngle(0, 0);
  Engines[0].setAngle(0, 0);
  Engines[3].setAngle(10, 0);
  delay(1000);
  Engines[0].setAngle(90, 0);
  Serial.println("Freestyle Hand activated!");
  
  screen.setCursor(0,0);
  screen.print("Freestyle Mode!");
  
  double calc;
  while(1==1){

    for(int s = 3; s < sensorsOnline; s++){
      selectSensor(s);
      Sensors[s].update();  
    }
    debugSensor(4);
    //Engines[3].setAngle((Sensors[3].getGyroAngleY()), 0); // thumb
    Engines[4].setAngle((Sensors[4].getGyroAngleY()), 0); // 0 straight index, 180 flexed index, default 0
    Engines[6].setAngle((Sensors[6].getGyroAngleY()), 0); // 0 straight ring, 180 flexed ring, default 0
    Engines[7].setAngle((Sensors[7].getGyroAngleY()), 0); // 0 straight pinky, 180 flexed pinky, default 0
    Engines[8].setAngle((Sensors[5].getGyroAngleY()), 0); // 0 straight middle, 180 flexed middle, default 0
    Engines[2].setAngle((105 + (-1 * Sensors[4].getGyroAngleZ())), 0);
    
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Robotic Arm v1.0 by Bartosz Bieniek");
  Wire.begin();
  
  screen.init();
  screen.backlight();
  screen.setCursor(0,0);
  
  toneGiveSignal(1, 1, 2000, 0);
  delay(1000);
  engineSetHome(0);
}

void loop() {
  byte incomingByte;
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    delay(1000);
    clearLCD();
    if(incomingByte == 48){ // DEC = 0
      disableEngines();
    }else if(incomingByte == 49){ // DEC = 1
      wave();
    }else if(incomingByte == 50){ // DEC = 2
      say_hi();
    }else if(incomingByte == 51){ // DEC = 3
      i_dont_know();
    }else if(incomingByte == 52){ // DEC = 4
      give_me_a_coin();
    }else if(incomingByte == 56){ // DEC = 8
      freestyle_arm();
    }else if(incomingByte == 57){ // DEC = 9
      freestyle_hand();
    }
  }
}
