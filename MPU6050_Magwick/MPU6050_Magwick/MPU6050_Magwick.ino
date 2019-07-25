#include <MadgwickAHRS.h>
#include <Wire.h>
#include "MPU6050_6Axis_MotionApps20.h"

#define OFFSET_ACCEL_X 819
#define OFFSET_ACCEL_Y 2570
#define OFFSET_ACCEL_Z 1260
#define OFFSET_GYRO_X 52
#define OFFSET_GYRO_Y 11
#define OFFSET_GYRO_Z 2

/*
//xda焦げてる方
#define OFFSET_ACCEL_X -3821
#define OFFSET_ACCEL_Y -847
#define OFFSET_ACCEL_Z 1257
#define OFFSET_GYRO_X 87
#define OFFSET_GYRO_Y 15
#define OFFSET_GYRO_Z -20
*/

MPU6050 mpu;
Madgwick MadgwickFilter;
unsigned long last = 0, now = 0;

VectorFloat g;
VectorInt16 Accel,Gyro;

void setup()
{
	Serial.begin(230400);
	Initialize();
	MadgwickFilter.begin(238);
}

void loop() {
	now = micros();
	DataProcessing();
	MadgwickFilter.updateIMU(g.x, g.y, g.z, Accel.x, Accel.y, Accel.z);
	if(now - last > 40000){
	Serial.print(MadgwickFilter.getYaw());
	Serial.print(" ");
	Serial.print(MadgwickFilter.getRoll());
	Serial.print(" ");
	Serial.print(MadgwickFilter.getPitch());
	Serial.print(" ");
	//Serial.println(now - last);
	Serial.println();
	last = now;
	}
}

void Initialize()
{
	Wire.begin();
	Wire.setClock(400000UL);
	pinMode(LED_BUILTIN, OUTPUT);

	mpu.initialize();
	while (!mpu.testConnection());

	mpu.setXAccelOffset(OFFSET_ACCEL_X);
	mpu.setYAccelOffset(OFFSET_ACCEL_Y);
	mpu.setZAccelOffset(OFFSET_ACCEL_Z);
	mpu.setXGyroOffset(OFFSET_GYRO_X);
	mpu.setYGyroOffset(OFFSET_GYRO_Y);
	mpu.setZGyroOffset(OFFSET_GYRO_Z);

	Wire.beginTransmission(0x68);
	Wire.write(0x1B);		//速度のフルスケール設定があるレジスタ
	Wire.write(0b00000000); //4,3bitを00に指定することで+-250degree/secまで測れる
	Wire.endTransmission();

	Wire.beginTransmission(0x68);
	Wire.write(0x1C);		//加速度のスケールレンジ設定があるレジスタ
	Wire.write(0b00000000); //4,3bitを00に指定することで+-2gまで測れる。gが重力加速度
	Wire.endTransmission();
}

void DataProcessing()
{
	digitalWrite(LED_BUILTIN, HIGH);

	mpu.getMotion6(&Accel.x, &Accel.y, &Accel.z, &Gyro.x, &Gyro.y, &Gyro.z);
	g.x = Gyro.x / 131.072;
	g.y = Gyro.y / 131.072;
	g.z = Gyro.z / 131.072;

	digitalWrite(13, LOW);
}
