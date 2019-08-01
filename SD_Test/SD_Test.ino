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
  Serial.print(",");
  Serial.print(mpu.magY);
  Serial.print(",");
  Serial.print(mpu.magZ);
  Serial.print(",");
  Serial.print(mpu.accX);
  Serial.print(",");
  Serial.print(mpu.accY);
  Serial.print(",");
  Serial.print(mpu.accZ);
  Serial.print(",");
  Serial.print(mpu.gyroX);
  Serial.print(",");
  Serial.print(mpu.gyroY);
  Serial.print(",");
  Serial.print(mpu.gyroZ);
  Serial.print(",");
  Serial.println(mpu.last_samplingTime);
}
