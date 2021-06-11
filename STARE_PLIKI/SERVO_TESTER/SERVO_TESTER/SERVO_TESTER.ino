#include <Servo.h>
#include <math.h>

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


ServoEngine Arm[11] = {ServoEngine(), ServoEngine(), ServoEngine(), ServoEngine(), ServoEngine(), ServoEngine(), ServoEngine(), ServoEngine(), ServoEngine(), ServoEngine(), ServoEngine()};

int prepareArms(int delayAfter){
  Arm[0].setPin(3); // Bark
  Arm[1].setPin(4); // Biceps rotator
  Arm[2].setPin(5); // T-pose
  Arm[3].setPin(6); // Elbow flex
  Arm[4].setPin(7); // Index
  Arm[5].setPin(8); // Forearm rotator
  Arm[6].setPin(9); // Ring
  Arm[7].setPin(10); // Pinky
  Arm[8].setPin(11); // Middle
  Arm[9].setPin(12); // Thumb (not working)
  Arm[10].setPin(13); // Wrist
  
  Arm[0].setAngle(0, delayAfter); // 0 default, 180 increased, default 0
  Arm[1].setAngle(90, delayAfter); // 0 left rotation, 180 right rotation, default 90
  Arm[2].setAngle(90, delayAfter); // 0 TPose inside, 180 outside, default 90
  Arm[3].setAngle(180, delayAfter); // 0 elbow flexed, 180 straight, default 180
  Arm[4].setAngle(0, delayAfter); // 0 straight index, 180 flexed index, default 0
  Arm[5].setAngle(90, delayAfter); // 0 down hand, 180 top hand, default 90
  Arm[6].setAngle(0, delayAfter); // 0 straight ring, 180 flexed ring, default 0
  Arm[7].setAngle(0, delayAfter); // 0 straight pinky, 180 flexed pinky, default 0
  Arm[8].setAngle(0, delayAfter); // 0 straight middle, 180 flexed middle, default 0
  Arm[9].setAngle(0, delayAfter); 
  Arm[10].setAngle(90, delayAfter); // 0 reverse wrist, 180 flexed wrist, default 90
}
int wave(){
  delay(3000);
  Arm[3].setAngle(0, 10);
  Arm[5].setAngle(0, 10);
  Arm[0].setAngle(30, 10);
  for(int i = 0; i<=3; i++){
    Arm[2].setAngle(70, 5);
    Arm[2].setAngle(110, 5);
  }
  Arm[2].setAngle(90, 5);
  prepareArms(5);
}
void middle_finger(){
    Arm[3].setAngle(0, 5);
    Arm[5].setAngle(180, 5);
    Arm[0].setAngle(30, 5);
    Arm[4].setAngle(180, 5);
    Arm[6].setAngle(180, 5);
    Arm[7].setAngle(180, 5);
    delay(3000);
    prepareArms(5);
}
void give_hand(){
  Arm[3].setAngle(130, 5);
  delay(2000);
  Arm[4].setAngle(50, 5);
  Arm[6].setAngle(50, 5);
  Arm[7].setAngle(50, 5);
  Arm[8].setAngle(50, 5);
  Arm[0].setAngle(60, 50);
}
void setup() {
  prepareArms(0);
  wave();
  delay(1000);
  middle_finger();
}

void loop() {
  
}
