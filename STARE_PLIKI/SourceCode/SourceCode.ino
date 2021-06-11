#include <Servo.h>
#include <Wire.h>
#include <MPU6050_tockn.h>
#include <math.h>

class Bone{
  private:
    int bonePin = 0, boneAngle = 0;
    Servo boneServo;
  public:
    int setPin(int pin){
      if(pin != 0){
        this->boneServo.attach(pin);
        this->bonePin = pin;
        return 1;
      }else{
        return 0;
      }
    }
    int getPin(){
      return this->bonePin;
    }
    int setAngle(int angle){
      if(angle >= 0 && angle <=180 && this->bonePin != 0){
        this->boneServo.write(angle);
        this->boneAngle = angle;
        return 1;  
      }else{
        return 0;
      }
    }
    int getAngle(){
      return this->boneAngle;
    }
};

class Arm{
  public:
    Bone finger[5] = {{Bone()}};
};

Arm arm[1] = Arm();
MPU6050 mpu6050(Wire);

int prepareArms(){
  // Attach servo to Tower Pro (Grab-axis)
  arm[0].finger[0].setPin(2);
  arm[0].finger[1].setPin(3);
  arm[0].finger[2].setPin(4);
  arm[0].finger[3].setPin(5);
  arm[0].finger[4].setPin(6);

  // Set all servos at "zero" (90 degree) position.
  arm[0].finger[0].setAngle(0);
  arm[0].finger[1].setAngle(0);
  arm[0].finger[2].setAngle(0);
  arm[0].finger[3].setAngle(0);
  arm[0].finger[4].setAngle(0);
}
void setup() {
  Serial.begin(9600);
  Wire.begin();
  prepareArms();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
}

void serialControl(){
  int incomingByte;
  while(1==1){
    delay(100);
    if (Serial.available() > 0) {
      incomingByte = Serial.read();
      if(incomingByte == 49){
        arm[0].finger[0].setAngle(0);
      }else if(incomingByte == 50){
        arm[0].finger[0].setAngle(90);
      }else if(incomingByte == 51){
        arm[0].finger[0].setAngle(180);
      }
    }  
  }
}

  int incomingByte;
void loop() {
  //serialControl();
  //mpu6050.update();
  //int angle = mpu6050.getAngleY()+90;
  for(int a = 0; a<=3; a++){
    for(int i = 0; i<=180; i++){
      arm[0].finger[a].setAngle(i);
      delay(2);
    }
  }
  for(int a = 0; a<=3; a++){
    for(int i = 180; i>0; i--){
      arm[0].finger[a].setAngle(i);
      delay(2);
    }
  }
  
  
}
