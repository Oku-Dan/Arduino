#include "Wire.h"
#include "MPU9250_Raw.h"
#include "SD.h"

MPU9250_Raw mpu;
File file;

void setup() {
  Serial.begin(230400);
  Wire.begin();
  Wire.setClock(400000);
  mpu.Initialize();
  mpu.mag_offset_x = 233;
  mpu.mag_offset_y = 200.5;
  mpu.mag_offset_z = 959.6;
  SD.begin(7);
  if (SD.exists("data.txt"))
    SD.remove("data.txt");
}

void loop() {
  while (!mpu.UpDate());
  file = SD.open("data.txt", FILE_WRITE);
  file.print(mpu.magX);
  file.print(",");
  file.print(mpu.magY);
  file.print(",");
  file.print(mpu.magZ);
  file.print(",");
  file.print(mpu.accX);
  file.print(",");
  file.print(mpu.accY);
  file.print(",");
  file.print(mpu.accZ);
  file.print(",");
  file.print(mpu.gyroX);
  file.print(",");
  file.print(mpu.gyroY);
  file.print(",");
  file.print(mpu.gyroZ);
  file.print(",");
  file.println(mpu.last_samplingTime);
  file.close();
}
