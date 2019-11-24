
#include "SD.h"
#include "LSM9DS1.h"
#include "lsp331ap_com_h.hpp"
#include <MsTimer2.h>

#define SPI_CS_PIN 10
#define LED_PIN 2

#define SD_WRITE_RATE 1
#define LED_RATE 3
#define PRESS_SENSOR_RATE 25

LSM_9DS1 imu;
LPS331AP lps;
String filename;
File file;
unsigned long last_pressure_read = 0,lsat_write_time = 0,lsat_LED_time = 0;
bool LED_state = false;

void SDwrite(){
	file.close();
	file = SD.open(filename, FILE_WRITE);
}

void Interrupt(){
	if(!LED_state)
	{
		digitalWrite(LED_PIN, HIGH);
		LED_state = true;
	}
	else{
		digitalWrite(LED_PIN, LOW);
		LED_state = false;
	}
}

void setup()
{
	pinMode(SPI_CS_PIN, OUTPUT);
	pinMode(LED_PIN, OUTPUT);

	MsTimer2::set(100, Interrupt);
	MsTimer2::start();

	if(!SD.begin(SPI_CS_PIN)){
		while (1)
			;
	}

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

	file = SD.open(filename, FILE_WRITE);

	//file.println("SD has been opened");

	MsTimer2::stop();
	MsTimer2::set(1000, Interrupt);
	MsTimer2::start();

	if (imu.Initialize(16,2000,16))
	{
		//file.println("LSM9DS1 has been initialized");
		Wire.setClock(400000L);
	}else{
		file.println("ERROR : LSM9DS1 hasn't been initialized!!");
		while (1)
			;
	}

	lps.SetupLPS331AP();

	file.println("Time(ms),Accel X(m/s*s),Accel Y(m/s*s),Accel Z(m/s*s),Gyro X(deg/s),Gyro Y(deg/s),Gyro Z(deg/s),Magnetic X(Gauss),Magnetic Y(Gauss),Magnetic Z(Gauss),Pressure (hPa),Temprature (deg)");
	
	delay(200);
	MsTimer2::stop();
}

void loop()
{
	float x, y, z;
	String message = "";
	bool flag = false;
	unsigned long time = millis();

	message += String(time);

	int DataToRead = imu.availableFIFO();
	for (int i = 0; i < DataToRead;i++)
	{
		flag = true;
		imu.ReadAccFIFO(&x, &y, &z);
		message += ',';
		message += String(x);
		message += ',';
		message += String(y);
		message += ',';
		message += String(z);
		message += ',';

		imu.ReadGyrFIFO(&x, &y, &z);
		message += String(x);
		message += ',';
		message += String(y);
		message += ',';
		message += String(z);
		message += ',';

		if(i < DataToRead - 1){
			message += ",,,";
			file.println(message);
			message = "";
		}
	}

	if (imu.ReadMag(&x, &y, &z))
	{
		if(!flag){
			message += ",,,,,,,";
			flag = true;
		}
		message += String(x);
		message += ',';
		message += String(y);
		message += ',';
		message += String(z);
		message += ',';
	}else{
			message += ",,,";
	}

	if(time - last_pressure_read > 1000 / PRESS_SENSOR_RATE)
	{
		last_pressure_read = time;
		message += String(lps.ReadPress());
		message += ',';
		message += String(lps.ReadTemp());
		message += ',';
	}

	if (flag)
	{
		file.println(message);
	}

	if (time - lsat_write_time > 1000 / SD_WRITE_RATE)
	{
		last_pressure_read = time;
		SDwrite();
	}

	if (time - lsat_LED_time > 1000 / LED_RATE)
	{
		lsat_LED_time = time;
		Interrupt();
	}

	/*
	if(imu.ReadAcc(&x, &y, &z)){
		flag = true;
		message += String(x);
		message += ',';
		message += String(y);
		message += ',';
		message += String(z);
		message += ',';
	}
	else
	{
		message += ",,,";
	}

	if(imu.ReadGyr(&x, &y, &z)){
		flag = true;
		message += String(x);
		message += ',';
		message += String(y);
		message += ',';
		message += String(z);
		message += ',';
	}
	else
	{
		message += ",,,";
	}

	if (imu.ReadMag(&x, &y, &z))
	{
		flag = true;
		message += String(x);
		message += ',';
		message += String(y);
		message += ',';
		message += String(z);
		message += ',';
	}
	else
	{
		message += ",,,";
	}

	if(flag){
		message += String(millis());
		file.println(message);
	}*/
}