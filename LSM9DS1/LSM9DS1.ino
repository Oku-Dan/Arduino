#include <SparkFunLSM9DS1.h>
#include <MadgwickAHRS.h>

#define LSM9DS1_M 0x1E
#define LSM9DS1_AG 0x6B
#define UPDATE_RATE 100  //Hz

LSM9DS1 imu;
Madgwick MadgwickFilter;

uint16_t connectionState = 0;
unsigned long last = 0, now = 0;
bool GyrUpdated = false, AccUpdated = false, MagUpdated = false;

void setup()
{
  Serial.begin(230400);

  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;
  if (connectionState == 0)
  {
    connectionState = imu.begin();
    while (connectionState == 0)
    {
      Serial.println("Failed to communicate with LSM9DS1.");
      Serial.println("Double-check wiring.");
      Serial.println("Default settings in this sketch will "
                     "work for an out of the box LSM9DS1 "
                     "Breakout, but may need to be modified "
                     "if the board jumpers are.");
      Serial.print("Connection Status: ");
      Serial.println(imu.begin());
      delay(1000);
      connectionState = imu.begin();
      Serial.println("------------------------------------------------------\n");
    }if(connectionState!=1){
      Serial.print("connectionState: ");
      Serial.println(connectionState);
    }
  }

  MadgwickFilter.begin(UPDATE_RATE);
}


void loop()
{
  now = micros();
  if (now - last > 1000000L / UPDATE_RATE)
  {
    last = now;
    imu.readGyro();
    imu.readAccel();
    imu.readMag();
    MadgwickFilter.update(
      imu.calcGyro(imu.gx), imu.calcGyro(imu.gy), imu.calcGyro(imu.gz), 
      imu.calcAccel(imu.ax), imu.calcAccel(imu.ay), imu.calcAccel(imu.az), 
      imu.calcMag(imu.mx), imu.calcMag(imu.my), imu.calcMag(imu.mz));
    Serial.print(MadgwickFilter.getYaw());
    Serial.print(" ");
    Serial.print(MadgwickFilter.getRoll());
    Serial.print(" ");
    Serial.print(MadgwickFilter.getPitch());
    Serial.println();
  }
}