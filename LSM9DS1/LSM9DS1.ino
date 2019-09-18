#include "LSM9DS1.h"
#include <MadgwickAHRS.h>

#define UPDATE_RATE 100  //Hz

Madgwick MadgwickFilter;

unsigned long last = 0, now = 0;
float ax, ay, az;
float gx, gy, gz;
float mx, my, mz;

void setup()
{
  Serial.begin(230400);
  Serial.println("!");
  while(!Initialize_LSM9DS1());
  Serial.println("!");
  MadgwickFilter.begin(UPDATE_RATE);
}

void loop()
{
  now = micros();
  if (now - last > 1000000L / UPDATE_RATE)
  {
    last = now;
    while (ReadAcc(&ax,&ay,&az));
    while (ReadGyr(&gx,&gy,&gz));
    while (ReadMag(&mx,&my,&mz));
    MadgwickFilter.update(gx,gy,gz,ax,ay,az,mx,my,mz);
    Serial.print(MadgwickFilter.getYaw());
    Serial.print(" ");
    Serial.print(MadgwickFilter.getRoll());
    Serial.print(" ");
    Serial.print(MadgwickFilter.getPitch());
    Serial.println();
  }
}