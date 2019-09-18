#include <Wire.h>

// Connect INA226 with SCL (A5), SDA (A4)

const byte LPS331AP_ADDR = B1011100; // SA0 = GND
//const byte LPS331AP_ADDR = B1011101;// SA0 = VDD_IO

const byte LPS331AP_WHOAMI = 0x0f;
const byte LPS331AP_CTRL1 = 0x20;
const byte LPS331AP_CTRL2 = 0x21;
const byte LPS331AP_CTRL3 = 0x22;
const byte LPS331AP_P_LL = 0x28;
const byte LPS331AP_P_L = 0x29;
const byte LPS331AP_P_H = 0x2A;
const byte LPS331AP_T_L = 0x2B;
const byte LPS331AP_T_H = 0x2C;

unsigned long last_sampling_time = 0;

void LPS331AP_write(byte reg, byte val)
{
	Wire.beginTransmission(LPS331AP_ADDR);
	Wire.write(reg);
	Wire.write(val);
	Wire.endTransmission();
}

byte LPS331AP_read(byte reg)
{
	byte ret = 0;
	// request the registor
	Wire.beginTransmission(LPS331AP_ADDR);
	Wire.write(reg);
	Wire.endTransmission();

	// read
	Wire.requestFrom((unsigned int)LPS331AP_ADDR, 1);

	while (Wire.available())
	{
		ret = Wire.read();
	}

	return ret;
}

bool Initialize_LPS331AP()
{
	Wire.begin();
	byte res = LPS331AP_read(LPS331AP_WHOAMI);
	if(res != 0xBB){
		Serial.println(res,HEX);
		return false;
	}
	Serial.println(res, HEX);
	LPS331AP_write(LPS331AP_CTRL1, B11110000);
							//   |||||||+ SPI Mode selection
							//   ||||||+- DELTA_EN
							//   |||||+-- BDU: block data update
							//   ||||+--- DIFF_EN: interrupt circuit enable
							//   |+++---- ODR2, ODR1, ODR0 (25Hz)
							//   +------- PD: 0: power down, 1: active
	return true;
}

bool ReadPrs(float *pressure)
{
	unsigned long time = millis();
	if(time - last_sampling_time < 1000/25){
		return false;
	}

	long P;
	float p;

	P = LPS331AP_read(LPS331AP_P_H);

	last_sampling_time = time;

	P = (P << 8) | LPS331AP_read(LPS331AP_P_L);
	P = (P << 8) | LPS331AP_read(LPS331AP_P_LL);

	*pressure = P;
	*pressure = *pressure / 4096.0;
	return true;
}

bool ReadTmp(float *temperature){
	unsigned long time = millis();
	if(time - last_sampling_time < 1000/25){
		return false;
	}

	short T;
	float t;

	T = LPS331AP_read(LPS331AP_T_H);

	last_sampling_time = time;

	T = (T << 8) | LPS331AP_read(LPS331AP_T_L);
	*temperature = T;
	*temperature = 42.5 + *temperature / 480.0;
}