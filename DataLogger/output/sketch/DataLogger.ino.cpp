#include <Arduino.h>
#line 1 "c:\\Users\\OKU_DAN\\source\\Arduino\\DataLogger\\DataLogger.ino"
#line 1 "c:\\Users\\OKU_DAN\\source\\Arduino\\DataLogger\\DataLogger.ino"
/*#include "SD.h"
#include "LSM9DS1.h"
#include <MsTimer2.h>

#define SPI_CS_PIN 10

LSM_9DS1 imu;
String filename;
File file;
unsigned long count = 0;

void Interrupt(){
	file.close();
	file = SD.open(filename, FILE_WRITE);
}

void setup()
{
	pinMode(SPI_CS_PIN, OUTPUT);

	SD.begin(SPI_CS_PIN);

	for (int i = 1; i < 100;i++){
		filename = String(i) + ".txt";
		if (!SD.exists(filename))
		{
			break;
		}
		if(i >= 99){
			SD.remove(filename);
		}
	}

	//File file = SD.open(filename, FILE_WRITE);
	file = SD.open(filename, FILE_WRITE);

	file.println("SD has been opened");

	if (imu.Initialize(16,2000,16))
	{
		file.println("LSM9DS1 has been initialized");
		Wire.setClock(400000L);
	}else{
		file.println("ERROR : LSM9DS1 hasn't been initialized!!");
		while (1)
			;
	}

	file.println("Accel X(m/s*s),Accel Y(m/s*s),Accel Z(m/s*s),Gyro X(deg/s),Gyro Y(deg/s),Gyro Z(deg/s),Magnetic X(Gauss),Magnetic Y(Gauss),Magnetic Z(Gauss),Time(ms),");
	//file.close();
	MsTimer2::set(2000, Interrupt);
	MsTimer2::start();
}

void loop()
{
	bool flag = false;
	//File file;
	float x, y, z;
	String message = "";
	if (imu.ReadAcc(&x, &y, &z))
	{
		flag = true;
		//file = SD.open(filename, FILE_WRITE);
		message += String(x);
		message += ',';
		message += String(y);
		message += ',';
		message += String(z);
		message += ',';
	}

	if (imu.ReadGyr(&x, &y, &z))
	{
		if(!flag){
			flag = true;
			//file = SD.open(filename, FILE_WRITE);
			message += ",,,";
		}
		message += String(x);
		message += ',';
		message += String(y);
		message += ',';
		message += String(z);
		message += ',';
	}else{
		if(flag){
			message += ",,,";
		}
	}

	if (imu.ReadMag(&x, &y, &z))
	{
		if (!flag)
		{
			flag = true;
			//file = SD.open(filename, FILE_WRITE);
			message += ",,,,,,";
		}
		message += String(x);
		message += ',';
		message += String(y);
		message += ',';
		message += String(z);
		message += ',';
	}else{
		if(flag){
			message += ",,,";
		}
	}

	if(flag)
	{
		message += String(millis());
		file.println(message);
		count++;
		if(count > 100)
		{
			//file.close();
			//file = SD.open(filename, FILE_WRITE);
			count = 0;
		}
		//file.close();
	}
}*/


#include "LPS331AP.h"

#line 130 "c:\\Users\\OKU_DAN\\source\\Arduino\\DataLogger\\DataLogger.ino"
void setup();
#line 139 "c:\\Users\\OKU_DAN\\source\\Arduino\\DataLogger\\DataLogger.ino"
void loop();
#line 130 "c:\\Users\\OKU_DAN\\source\\Arduino\\DataLogger\\DataLogger.ino"
void setup()
{
	Serial.begin(115200);
	Serial.println("Init Sensor");
	while(!Initialize_LPS331AP())
		Serial.println("failed");
	Serial.println("done");
}

void loop(){
	float p;
	if(ReadPrs(&p)){
		Serial.println(p);
	}
}

