#include "Wire.h"
#include "MPU9250_Raw.h"

MPU9250_Raw mpu;

void setup() {
  Serial.begin(230400);
  Wire.begin();
  Wire.setClock(400000);
  mpu.Initialize();
}

void loop() {
  while (!mpu.UpDate());
  Serial.print(mpu.magX);
  Serial.print(" ");
  Serial.print(mpu.magY);
  Serial.print(" ");
  Serial.print(mpu.magZ);
  Serial.print(" ");
  Serial.println(mpu.last_samplingTime);
}
