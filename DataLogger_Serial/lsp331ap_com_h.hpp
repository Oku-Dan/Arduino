/*
Code for communicating LPS331AP.
produced by Chiharu M. on 2019/09/15
*/
/*
Usages:
    - run SetupLPS331AP() once at start
    - values change rate is (press / temp) = (12.5Hz / 12.5Hz) *can be changed by REG1
    - can get temp-val[degree] from ReadTemp()
    - can get press-value[mbar] from read_press()
*/

#ifndef INCLUDE_LPS331AP
#define INCLUDE_LPS331AP

#include <Arduino.h>
#include <Wire.h>

/*
Module Address
LPS331AP address can be changed by SA0 pin when using I2C.
*/
#define LPS331AP_ADDR0 0b1011100 //SA0=L(GND)
#define LPS331AP_ADDR1 0b1011101 //SA0=H(VDD)

// Using Address (*change by SA0 pin condition)
uint8_t lps331apAddress = LPS331AP_ADDR1; //SA0=L(GND)

// Register Address
#define LPS331AP_WHO_AM_I_REG 0x0f
#define LPS331AP_CTRL_REG1 0x20
#define LPS331AP_RES_CONF 0x10
#define LPS331AP_TEMP_OUT_L 0x2b
#define LPS331AP_TEMP_OUT_H 0x2c
#define LPS331AP_PRESS_OUT_XL 0x28
#define LPS331AP_PRESS_OUT_L 0x29
#define LPS331AP_PRESS_OUT_H 0x2a

// Register Setup
#define LPS331AP_WHO_AM_I_DEFAULT 0xbb
#define LPS331AP_CTRL_REG1_PD 0x00			// In need of power-down, set @0x20 register to this value
#define LPS331AP_CTRL_REG1_SETUP 0b11110100 // power:on, prs:12.5Hz, tmp:12.5Hz, BDU:no update during reading
#define LPS331AP_RES_CONF_SETUP 0x7a		// Suggested value in datasheet.
//#define LPS331AP_RES_CONF_SETUP 0x6a

void writeRegister(uint8_t address, uint8_t destinationRegister, uint8_t value)
{
	Wire.beginTransmission(address);
	Wire.write(destinationRegister);
	Wire.write(value);
	Wire.endTransmission();
}

uint8_t readRegister(int address, uint8_t distinationRegister)
{
	uint8_t data;

	Wire.beginTransmission(address);
	Wire.write(distinationRegister);
	Wire.endTransmission(false);
	Wire.requestFrom(address, (uint8_t)1);
	if (Wire.available() > 0)
	{
		data = Wire.read();
	}
	else
	{
		Serial.println("can not read");
		while (true)
			;
	}
	Wire.endTransmission();

	return data;
}

class LPS331AP
{
public:
	void SetupLPS331AP();
	float ReadPress();
	float ReadTemp();
};

void LPS331AP::SetupLPS331AP()
{
	uint8_t whoAmI = readRegister(lps331apAddress, LPS331AP_WHO_AM_I_REG);

	if (whoAmI != LPS331AP_WHO_AM_I_DEFAULT)
	{
		Serial.print("LPS331AP UNAVAILABLE.");
		Serial.println(whoAmI, BIN);
		while (true)
			;
	}

	// Power down befor changing CONF
	writeRegister(lps331apAddress, LPS331AP_CTRL_REG1, LPS331AP_CTRL_REG1_PD);
	// Change CONF
	writeRegister(lps331apAddress, LPS331AP_RES_CONF, LPS331AP_RES_CONF_SETUP);
	// Power on and apply REG1
	writeRegister(lps331apAddress, LPS331AP_CTRL_REG1, LPS331AP_CTRL_REG1_SETUP);
}

float LPS331AP::ReadTemp()
{
	uint8_t tempOutL = readRegister(lps331apAddress, LPS331AP_TEMP_OUT_L);
	uint8_t tempOutH = readRegister(lps331apAddress, LPS331AP_TEMP_OUT_H);
	float tempVal = 42.5 + (int16_t)(tempOutH << 8 | tempOutL) / 480.0;

	return tempVal;
}

float LPS331AP::ReadPress()
{
	uint8_t pressOutH = readRegister(lps331apAddress, LPS331AP_PRESS_OUT_H);
	uint8_t pressOutL = readRegister(lps331apAddress, LPS331AP_PRESS_OUT_L);
	uint8_t pressOutXL = readRegister(lps331apAddress, LPS331AP_PRESS_OUT_XL);
	float pressVal = ((uint32_t)pressOutH << 16 | (uint32_t)pressOutL << 8 | (uint32_t)pressOutXL) / 4096.0;

	return pressVal;
}

#endif