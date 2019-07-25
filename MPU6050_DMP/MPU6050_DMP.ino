#include <Wire.h>
#include "MPU6050_6Axis_MotionApps20.h"

#define SENSER_RATE 9
/* 0 --> 167Hz 1 --> 125Hz
   * 4 --> 100Hz 9 --> 50Hz
     19 --> 25Hz            */


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
bool DataReceivedFlag = false;
byte datasize, buffer[256];
unsigned long last = 0,now = 0;
long countup = 0;

Quaternion q;
VectorFloat g;
VectorInt16 Accel, RealAccel, WorldAccel;

void setup()
{
	Serial.begin(230400);
	Initialize();
}

void loop()
{
	if(DataReceivedFlag){
		DataReceivedFlag = false;
		DataProcessing();

		//degree <-- euler[0] * 180/3.14
		//G <-- aa.x / 8192
		Serial.print(q.x);
		Serial.print(' ');
		Serial.print(q.y);
		Serial.print(' ');
		Serial.print(q.z);
		Serial.print(' ');
		Serial.print(q.w);
		Serial.print(' ');
		Serial.print(WorldAccel.x / 8192.0);
		Serial.print(' ');
		Serial.print(WorldAccel.y / 8192.0);
		Serial.print(' ');
		Serial.print(WorldAccel.z / 8192.0);
		Serial.print(' ');

		now = micros();
		//Serial.println(now - last);
    	Serial.println();
		last = now;
	}
}

void Initialize(){
	Wire.begin();
	Wire.setClock(400000UL);
	pinMode(LED_BUILTIN, OUTPUT);

	mpu.initialize();
	while (!mpu.testConnection());

	mpu.dmpInitialize();
	mpu.setDMPEnabled(true);

	mpu.setRate(SENSER_RATE);
	mpu.setXAccelOffset(OFFSET_ACCEL_X);
	mpu.setYAccelOffset(OFFSET_ACCEL_Y);
	mpu.setZAccelOffset(OFFSET_ACCEL_Z);
	mpu.setXGyroOffset(OFFSET_GYRO_X);
	mpu.setYGyroOffset(OFFSET_GYRO_Y);
	mpu.setZGyroOffset(OFFSET_GYRO_Z);

	attachInterrupt(0, Interrupt, RISING);
	datasize = mpu.dmpGetFIFOPacketSize();
}

void Interrupt(){
	DataReceivedFlag = true;
}

void DataProcessing(){
	digitalWrite(LED_BUILTIN, HIGH);

	int bytesToRead;
	do{
		bytesToRead = mpu.getFIFOCount();
	}while(bytesToRead < datasize);

	while (bytesToRead >= datasize)
	{
		mpu.getFIFOBytes(buffer, datasize);
		bytesToRead -= datasize;

		mpu.dmpGetAccel(&Accel, buffer);
		mpu.dmpGetQuaternion(&q, buffer);
		mpu.dmpGetGravity(&g, &q);
		mpu.dmpGetLinearAccel(&RealAccel, &Accel, &g);
		mpu.dmpGetLinearAccelInWorld(&WorldAccel, &RealAccel, &q);
	}
	digitalWrite(13, LOW);
}
