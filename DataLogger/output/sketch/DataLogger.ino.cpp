#include <Arduino.h>
#line 1 "c:\\Users\\OKU_DAN\\source\\Arduino\\DataLogger\\DataLogger.ino"
#line 1 "c:\\Users\\OKU_DAN\\source\\Arduino\\DataLogger\\DataLogger.ino"
//#include "SD.h"
#include "LSM9DS1.h"
#define file Serial

//#define SPI_CS_PIN 7

float x, y, z;
//File file;
bool flag;

#line 11 "c:\\Users\\OKU_DAN\\source\\Arduino\\DataLogger\\DataLogger.ino"
void setup();
#line 35 "c:\\Users\\OKU_DAN\\source\\Arduino\\DataLogger\\DataLogger.ino"
void loop();
#line 11 "c:\\Users\\OKU_DAN\\source\\Arduino\\DataLogger\\DataLogger.ino"
void setup()
{
	Serial.begin(230400);

	Serial.println("LSM9DS1 is being initialized......");
	if (Initialize_LSM9DS1())
	{
		Serial.println("LSM9DS1 has been initialized");
	}else{
		Serial.println("ERROR : LSM9DS1 hasn't been initialized!!");
	}

	//Calibrate_LSM9DS1();

	/*
	SD.begin(SPI_CS_PIN);
	if (SD.exists("data.txt")){
		SD.remove("data.txt");
	}

	file = SD.open("data.txt", FILE_WRITE);*/
	file.println("Accel X(m/s*s),Accel Y(m/s*s),Accel Z(m/s*s),Gyro X(deg/s),Gyro Y(deg/s),Gyro Z(deg/s),Magnetic X(Gauss),Magnetic Y(Gauss),Magnetic Z(Gauss),Time(ms),");
}

void loop()
{/*
	flag = false;
	if (ReadAcc(&x, &y, &z))
	{
		flag = true;
		//file = SD.open("data.txt", FILE_WRITE);
		file.print(x);
		file.print(',');
		file.print(y);
		file.print(',');
		file.print(z);
		file.print(',');
	}

	if (ReadGyr(&x, &y, &z))
	{
		if(!flag){
			//file = SD.open("data.txt", FILE_WRITE);
			file.print(",,,");
		}
		flag = true;
		file.print(x);
		file.print(',');
		file.print(y);
		file.print(',');
		file.print(z);
		file.print(',');
	}else{
		if(flag){
			file.print(",,,");
		}
	}

	if (ReadMag(&x, &y, &z))
	{
		if (!flag)
		{
			//file = SD.open("data.txt", FILE_WRITE);
			file.print(",,,,,,");
		}
		flag = true;
		file.print(x);
		file.print(',');
		file.print(y);
		file.print(',');
		file.print(z);
		file.print(',');
	}else{
		if(flag){
			file.print(",,,");
		}
	}

	if(flag){
		file.println(millis());
		//file.close();
	}
	*/
	while(!FusionAccGryMag(&x,&y,&z))
		;
	file.print(x);
	file.print(',');
	file.print(y);
	file.print(',');
	file.println(z);
}

