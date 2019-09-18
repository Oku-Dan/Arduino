#include "MPU9250_Raw.h"
#include "SD.h"

#define PACKET_SIZE 10
#define SPI_CS_PIN 10

typedef struct
{
  float magX;
  float magY;
  float magZ;
  float accX;
  float accY;
  float accZ;
  float gyroX;
  float gyroY;
  float gyroZ;
  unsigned long time;
} LogData;

LogData data[PACKET_SIZE];
unsigned int count = 0;
MPU9250_Raw mpu;
File file;

void setup() {
  Serial.begin(230400);
  mpu.Initialize();
  SD.begin(SPI_CS_PIN);
  if (SD.exists("data.txt"))
    SD.remove("data.txt");
}

void loop() {
  while(!mpu.UpDate());

  Serial.println(mpu.magX);

  count++;
  data[count % PACKET_SIZE - 1].magX = mpu.magX;
  data[count % PACKET_SIZE - 1].magY = mpu.magY;
  data[count % PACKET_SIZE - 1].magZ = mpu.magZ;

  data[count % PACKET_SIZE - 1].accX = mpu.accX;
  data[count % PACKET_SIZE - 1].accY = mpu.accY;
  data[count % PACKET_SIZE - 1].accZ = mpu.accZ;

  data[count % PACKET_SIZE - 1].gyroX = mpu.gyroX;
  data[count % PACKET_SIZE - 1].gyroY = mpu.gyroY;
  data[count % PACKET_SIZE - 1].gyroZ = mpu.gyroZ;

  data[count % PACKET_SIZE - 1].time = mpu.last_samplingTime;

  if(count % PACKET_SIZE >= PACKET_SIZE){
    file = SD.open("data.txt", FILE_WRITE);
    for (int i = 0; i < PACKET_SIZE;i++){
      file.print(data[i].magX);
      file.print(", ");
      file.print(data[i].magY);
      file.print(", ");
      file.print(data[i].magZ);
      file.print(", ");

      file.print(data[i].accX);
      file.print(", ");
      file.print(data[i].accY);
      file.print(", ");
      file.print(data[i].accZ);
      file.print(", ");

      file.print(data[i].gyroX);
      file.print(", ");
      file.print(data[i].gyroY);
      file.print(", ");
      file.print(data[i].gyroZ);
      file.print(", ");

      file.println(data[i].time);
    }
    file.close();
  }
}
