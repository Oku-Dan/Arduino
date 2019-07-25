#include "MPU9150_9Axis_MotionApps41.h"

#define SENSER_RATE 9
/* 0 --> 167Hz 1 --> 125Hz
   * 4 --> 100Hz 9 --> 50Hz
     19 --> 25Hz            */

MPU9150 mpu;
bool DataReceivedFlag = false;
byte datasize, buffer[256];
unsigned long last = 0, now = 0;
long countup = 0;

Quaternion q;
VectorFloat g;
float ypr[3];
int16_t Accel[3], Gyro[3], Magnet[3];

void setup()
{
	Serial.begin(230400);
	Initialize();
}

void loop()
{
	if (DataReceivedFlag)
	{
		DataReceivedFlag = false;
		DataProcessing();

		//degree <-- euler[0] * 180/3.14
		//G <-- aa.x / 8192
		/*
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
		last = now;*/
		Serial.print(ypr[0] * 180 / 3.14);
		Serial.print(' ');
		Serial.print(ypr[1] * 180 / 3.14);
		Serial.print(' ');
		Serial.println(ypr[2] * 180 / 3.14);
	}
}

void Initialize()
{
	Wire.begin();
	Wire.setClock(400000UL);
	pinMode(LED_BUILTIN, OUTPUT);

	mpu.initialize();
	while (!mpu.testConnection());

	mpu.dmpInitialize();
	mpu.setDMPEnabled(true);

	mpu.setRate(SENSER_RATE);

	attachInterrupt(0, Interrupt, RISING);
	datasize = mpu.dmpGetFIFOPacketSize();
}

void Interrupt()
{
	DataReceivedFlag = true;
}

void DataProcessing()
{
	digitalWrite(LED_BUILTIN, HIGH);

	int bytesToRead;
	do
	{
		bytesToRead = mpu.getFIFOCount();
	} while (bytesToRead < datasize);

	while (bytesToRead >= datasize)
	{
		mpu.getFIFOBytes(buffer, datasize);
		bytesToRead -= datasize;

		//mpu.dmpGetAccel(&Accel, buffer);
		mpu.dmpGetQuaternion(&q, buffer);
		mpu.dmpGetGravity(&g, &q);
		//mpu.dmpGetLinearAccel(&RealAccel, &Accel, &g);
		//mpu.dmpGetLinearAccelInWorld(&WorldAccel, &RealAccel, &q);
		//mpu.dmpGetMag(&Magnet, buffer);
		//mpu.dmpGetEuler(euler, &q);
		mpu.dmpGetYawPitchRoll(ypr, &q, &g);
	}
	digitalWrite(13, LOW);
}
