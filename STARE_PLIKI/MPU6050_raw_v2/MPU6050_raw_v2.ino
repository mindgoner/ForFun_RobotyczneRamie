
#include <MPU6050_MinervaEdit.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Configure default parameters
int buzzerPin = 2;
int numberOfSensors = 8;
int sensorsOnline;

// Create objects
MPU6050ME sensors[8] = {MPU6050ME(Wire), MPU6050ME(Wire), MPU6050ME(Wire), MPU6050ME(Wire), MPU6050ME(Wire), MPU6050ME(Wire), MPU6050ME(Wire), MPU6050ME(Wire)};
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

void selectSensor(uint8_t sensorId){

  // Switching between sensors connected to TCA9548A I2C Multiplexer 
  // by sending one 4-bit number (0-7) 

  
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
      Wire.beginTransmission(0x70);
      Wire.write(1 << i);
      Wire.endTransmission();

      sensors[i].begin();
      double calculationSucces = sensors[i].calcGyroOffsets(true, 0, 0);
      sensors[i].update();
      if(calculationSucces){ // An error occured while configuring gyroOffsets
        toneGiveSignal(1, 0, 100, 0);
        screen.print("-");
      }else{ // Sensor loaded successful
        sensorsOnline++;
        toneGiveSignal(1, 1, 100, 0);
        screen.print("+");
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

void setup() {
  Serial.begin(9600);
  Wire.begin();
  screen.init();
  screen.backlight();
  screen.setCursor(0,0);
  sensorsConfiguration();
    
}

void loop() {
  selectSensor(2);
  sensors[2].update();
  Serial.print("gX : ");Serial.print(sensors[2].getGyroAngleX());
  Serial.print("\tgY : ");Serial.print(sensors[2].getGyroAngleY());
  Serial.print("\tgZ : ");Serial.println(sensors[2].getGyroAngleZ());
  delay(100);
}
