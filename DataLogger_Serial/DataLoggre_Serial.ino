#include "LSM9DS1_ALT.h"
#include "lsp331ap_com_h.hpp"

LSM_9DS1 imu;
LPS331AP lps;
unsigned long last_pressure_read = 0;

void setup()
{
	Serial.begin(230400);
	Serial.println("Serial has been opened");

	/*
	if (imu.Initialize(16,2000,16))
	{
		Serial.println("LSM9DS1 has been initialized");
	}else{
		Serial.println("ERROR : LSM9DS1 hasn't been initialized!!");
		while (1)
			;
	}*/
	Wire.begin();

	lps.SetupLPS331AP();

	Serial.println("Time(ms),Accel X(m/s*s),Accel Y(m/s*s),Accel Z(m/s*s),Gyro X(deg/s),Gyro Y(deg/s),Gyro Z(deg/s),Magnetic X(Gauss),Magnetic Y(Gauss),Magnetic Z(Gauss),Pressure (hPa),Temprature (deg)");
}

void loop()
{
	float x, y, z;
	String message = "";
	bool flag = false;
	unsigned long time = millis();

	/*
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
			Serial.println(message);
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
	}*/

	if(time - last_pressure_read > 1000/25){
		flag = true;
		last_pressure_read = time;
		message += String(lps.ReadPress());
		message += ',';
		message += String(lps.ReadTemp());
		message += ',';
	}

	if (flag)
	{
		Serial.println(message);
	}
}