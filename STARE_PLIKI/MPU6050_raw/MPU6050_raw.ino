#include "MPU6050.h"
#include "Wire.h"

// 
MPU6050 accelgyro, accelgyro_two;

int gx, gy, gz;
int ax, ay, az;

#define OUTPUT_READABLE_ACCELGYRO

void TCA9548A(uint8_t bus)
{
  Wire.beginTransmission(0x70);  // TCA9548A address is 0x70
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}

void setup() {
    Wire.begin();
    
    Serial.begin(38400);
    Serial.println("Initializing I2C devices...");

    TCA9548A(0);
    accelgyro.initialize();
    TCA9548A(1);
    accelgyro_two.initialize();
}

void loop() {
  TCA9548A(0);
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.println(az);
    delay(100);
}
