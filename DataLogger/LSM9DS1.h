#include <Wire.h>

#define LSM9DS1_M 0x1E
#define LSM9DS1_AG 0x6B

#define ACT_THS 0x04
#define ACT_DUR 0x05
#define INT_GEN_CFG_XL 0x06
#define INT_GEN_THS_X_XL 0x07
#define INT_GEN_THS_Y_XL 0x08
#define INT_GEN_THS_Z_XL 0x09
#define INT_GEN_DUR_XL 0x0A
#define REFERENCE_G 0x0B
#define INT1_CTRL 0x0C
#define INT2_CTRL 0x0D
#define WHO_AM_I_XG 0x0F
#define CTRL_REG1_G 0x10
#define CTRL_REG2_G 0x11
#define CTRL_REG3_G 0x12
#define ORIENT_CFG_G 0x13
#define INT_GEN_SRC_G 0x14
#define OUT_TEMP_L 0x15
#define OUT_TEMP_H 0x16
#define STATUS_REG_0 0x17
#define OUT_X_L_G 0x18
#define OUT_X_H_G 0x19
#define OUT_Y_L_G 0x1A
#define OUT_Y_H_G 0x1B
#define OUT_Z_L_G 0x1C
#define OUT_Z_H_G 0x1D
#define CTRL_REG4 0x1E
#define CTRL_REG5_XL 0x1F
#define CTRL_REG6_XL 0x20
#define CTRL_REG7_XL 0x21
#define CTRL_REG8 0x22
#define CTRL_REG9 0x23
#define CTRL_REG10 0x24
#define INT_GEN_SRC_XL 0x26
#define STATUS_REG_1 0x27
#define OUT_X_L_XL 0x28
#define OUT_X_H_XL 0x29
#define OUT_Y_L_XL 0x2A
#define OUT_Y_H_XL 0x2B
#define OUT_Z_L_XL 0x2C
#define OUT_Z_H_XL 0x2D
#define FIFO_CTRL 0x2E
#define FIFO_SRC 0x2F
#define INT_GEN_CFG_G 0x30
#define INT_GEN_THS_XH_G 0x31
#define INT_GEN_THS_XL_G 0x32
#define INT_GEN_THS_YH_G 0x33
#define INT_GEN_THS_YL_G 0x34
#define INT_GEN_THS_ZH_G 0x35
#define INT_GEN_THS_ZL_G 0x36
#define INT_GEN_DUR_G 0x37

///////////////////////////////
// LSM9DS1 Magneto Registers //
///////////////////////////////
#define OFFSET_X_REG_L_M 0x05
#define OFFSET_X_REG_H_M 0x06
#define OFFSET_Y_REG_L_M 0x07
#define OFFSET_Y_REG_H_M 0x08
#define OFFSET_Z_REG_L_M 0x09
#define OFFSET_Z_REG_H_M 0x0A
#define WHO_AM_I_M 0x0F
#define CTRL_REG1_M 0x20
#define CTRL_REG2_M 0x21
#define CTRL_REG3_M 0x22
#define CTRL_REG4_M 0x23
#define CTRL_REG5_M 0x24
#define STATUS_REG_M 0x27
#define OUT_X_L_M 0x28
#define OUT_X_H_M 0x29
#define OUT_Y_L_M 0x2A
#define OUT_Y_H_M 0x2B
#define OUT_Z_L_M 0x2C
#define OUT_Z_H_M 0x2D
#define INT_CFG_M 0x30
#define INT_SRC_M 0x31
#define INT_THS_L_M 0x32
#define INT_THS_H_M 0x33

////////////////////////////////
// LSM9DS1 WHO_AM_I Responses //
////////////////////////////////
#define WHO_AM_I_AG_RSP 0x68
#define WHO_AM_I_M_RSP 0x3D

#define ACC_SAMPLING_RATE 0b101 //011 : 119Hz / 100 : 238Hz / 101 : 476Hz / 110 : 952Hz
#define GYR_SAMPLING_RATE 0b101 //011 : 119Hz / 100 : 238Hz / 101 : 476Hz / 110 : 952Hz
#define MAG_SAMPLING_RATE 0b111 //011 : 5Hz / 100 : 10Hz / 101 : 20Hz / 110 : 40Hz / 111 : 80Hz

#define SENSITIVITY_ACCELEROMETER_2 0.000061
#define SENSITIVITY_ACCELEROMETER_4 0.000122
#define SENSITIVITY_ACCELEROMETER_8 0.000244
#define SENSITIVITY_ACCELEROMETER_16 0.000732
#define SENSITIVITY_GYROSCOPE_245 0.00875
#define SENSITIVITY_GYROSCOPE_500 0.0175
#define SENSITIVITY_GYROSCOPE_2000 0.07
#define SENSITIVITY_MAGNETOMETER_4 0.00014
#define SENSITIVITY_MAGNETOMETER_8 0.00029
#define SENSITIVITY_MAGNETOMETER_12 0.00043
#define SENSITIVITY_MAGNETOMETER_16 0.00058

class LSM_9DS1{
	private :
		void AccInit(int scale);
		void GyrInit(int scale);
		void MagInit(int scale);
		float AccelSensitivity = SENSITIVITY_ACCELEROMETER_2;
		float GyroSensitivity = SENSITIVITY_GYROSCOPE_245;
		float MagnetSensitivity = SENSITIVITY_MAGNETOMETER_4;

	public : 
		bool Initialize(int AccelScale, int GyroScale, int MagnetScale);
		bool ReadAcc(float *x, float *y, float *z);
		bool ReadGyr(float *x, float *y, float *z);
		bool ReadMag(float *x, float *y, float *z);
};

void WriteByte(uint8_t add, uint8_t reg, uint8_t data){
	Wire.beginTransmission(add);
	Wire.write(reg);
	Wire.write(data);
	Wire.endTransmission();
}

uint8_t ReadByte(uint8_t add, uint8_t reg){
	Wire.beginTransmission(add);
	Wire.write(reg);
	Wire.endTransmission(false);
	Wire.requestFrom(add, (uint8_t)1);
	uint8_t data = Wire.read();
	return data;
}
void ReadBytes(uint8_t add, uint8_t reg, uint8_t *data, uint8_t count){
	byte retVal;
	Wire.beginTransmission(add);
	Wire.write(reg | 0x80);
	Wire.endTransmission(false);
	Wire.requestFrom(add, count);
	for (int i = 0; i < count;i++){
		data[i] = Wire.read();
	}
}

void LSM_9DS1::AccInit(int scale){

	uint8_t tempRegValue = 0;

	//	CTRL_REG5_XL (0x1F) (Default value: 0x38)
	//	[DEC_1][DEC_0][Zen_XL][Yen_XL][Zen_XL][0][0][0]
	//	DEC[0:1] - Decimation of accel data on OUT REG and FIFO.
	//		00: None, 01: 2 samples, 10: 4 samples 11: 8 samples
	//	Zen_XL - Z-axis output enabled
	//	Yen_XL - Y-axis output enabled
	//	Xen_XL - X-axis output enabled
	WriteByte(LSM9DS1_AG,CTRL_REG5_XL, 0b111000);

	// CTRL_REG6_XL (0x20) (Default value: 0x00)
	// [ODR_XL2][ODR_XL1][ODR_XL0][FS1_XL][FS0_XL][BW_SCAL_ODR][BW_XL1][BW_XL0]
	// ODR_XL[2:0] - Output data rate & power mode selection
	// FS_XL[1:0] - Full-scale selection
	// BW_SCAL_ODR - Bandwidth selection
	// BW_XL[1:0] - Anti-aliasing filter bandwidth selection
	// To disable the accel, set the sampleRate bits to 0.
	tempRegValue |= (ACC_SAMPLING_RATE & 0x07) << 5;
	switch (scale)
	{
	case 4:
		tempRegValue |= (0x2 << 3);
		AccelSensitivity = SENSITIVITY_ACCELEROMETER_4;
		break;
	case 8:
		tempRegValue |= (0x3 << 3);
		AccelSensitivity = SENSITIVITY_ACCELEROMETER_8;
		break;
	case 16:
		tempRegValue |= (0x1 << 3);
		AccelSensitivity = SENSITIVITY_ACCELEROMETER_16;
		break;
		// Otherwise it'll be set to 2g (0x0 << 3)
	}
	WriteByte(LSM9DS1_AG,CTRL_REG6_XL, tempRegValue);

	// CTRL_REG7_XL (0x21) (Default value: 0x00)
	// [HR][DCF1][DCF0][0][0][FDS][0][HPIS1]
	// HR - High resolution mode (0: disable, 1: enable)
	// DCF[1:0] - Digital filter cutoff frequency
	// FDS - Filtered data selection
	// HPIS1 - HPF enabled for interrupt function
	WriteByte(LSM9DS1_AG,CTRL_REG7_XL, 0);
}
void LSM_9DS1::GyrInit(int scale){
	uint8_t tempRegValue = 0;
	// CTRL_REG1_G (Default value: 0x00)
	// [ODR_G2][ODR_G1][ODR_G0][FS_G1][FS_G0][0][BW_G1][BW_G0]
	// ODR_G[2:0] - Output data rate selection
	// FS_G[1:0] - Gyroscope full-scale selection
	// BW_G[1:0] - Gyroscope bandwidth selection

	// To disable gyro, set sample rate bits to 0. We'll only set sample
	// rate if the gyro is enabled.
	tempRegValue = (GYR_SAMPLING_RATE & 0x07) << 5;
	switch (scale)
	{
	case 500:
		tempRegValue |= (0x1 << 3);
		GyroSensitivity = SENSITIVITY_GYROSCOPE_500;
		break;
	case 2000:
		tempRegValue |= (0x3 << 3);
		GyroSensitivity = SENSITIVITY_GYROSCOPE_2000;
		break;
		// Otherwise we'll set it to 245 dps (0x0 << 4)
	}
	WriteByte(LSM9DS1_AG, CTRL_REG1_G, tempRegValue);

	// CTRL_REG2_G (Default value: 0x00)
	// [0][0][0][0][INT_SEL1][INT_SEL0][OUT_SEL1][OUT_SEL0]
	// INT_SEL[1:0] - INT selection configuration
	// OUT_SEL[1:0] - Out selection configuration
	WriteByte(LSM9DS1_AG, CTRL_REG2_G, 0x00);

	// CTRL_REG3_G (Default value: 0x00)
	// [LP_mode][HP_EN][0][0][HPCF3_G][HPCF2_G][HPCF1_G][HPCF0_G]
	// LP_mode - Low-power mode enable (0: disabled, 1: enabled)
	// HP_EN - HPF enable (0:disabled, 1: enabled)
	// HPCF_G[3:0] - HPF cutoff frequency
	WriteByte(LSM9DS1_AG, CTRL_REG3_G, 0x00);

	// CTRL_REG4 (Default value: 0x38)
	// [0][0][Zen_G][Yen_G][Xen_G][0][LIR_XL1][4D_XL1]
	// Zen_G - Z-axis output enable (0:disable, 1:enable)
	// Yen_G - Y-axis output enable (0:disable, 1:enable)
	// Xen_G - X-axis output enable (0:disable, 1:enable)
	// LIR_XL1 - Latched interrupt (0:not latched, 1:latched)
	// 4D_XL1 - 4D option on interrupt (0:6D used, 1:4D used)
	tempRegValue = 0;
	WriteByte(LSM9DS1_AG, CTRL_REG4, 0b111000);

	// ORIENT_CFG_G (Default value: 0x00)
	// [0][0][SignX_G][SignY_G][SignZ_G][Orient_2][Orient_1][Orient_0]
	// SignX_G - Pitch axis (X) angular rate sign (0: positive, 1: negative)
	// Orient [2:0] - Directional user orientation selection
	WriteByte(LSM9DS1_AG, ORIENT_CFG_G, 0);
}
void LSM_9DS1::MagInit(int scale){
	uint8_t tempRegValue = 0;

	// CTRL_REG1_M (Default value: 0x10)
	// [TEMP_COMP][OM1][OM0][DO2][DO1][DO0][FAST_ODR][ST]
	// TEMP_COMP - Temperature compensation
	// OM[1:0] - X & Y axes op mode selection
	//	00:low-power, 01:medium performance
	//	10: high performance, 11:ultra-high performance
	// DO[2:0] - Output data rate selection
	// FAST_ODR - data rate higher than 80Hz (0:disable, 1:enable)
	// ST - Self-test enable
	tempRegValue |= (0b1 & 0x1) << 7;
	tempRegValue |= (0b11 & 0x3) << 5;
	tempRegValue |= (MAG_SAMPLING_RATE & 0x7) << 2;
	tempRegValue |= 0b00;
	WriteByte(LSM9DS1_M,CTRL_REG1_M, tempRegValue);

	// CTRL_REG2_M (Default value 0x00)
	// [0][FS1][FS0][0][REBOOT][SOFT_RST][0][0]
	// FS[1:0] - Full-scale configuration
	// REBOOT - Reboot memory content (0:normal, 1:reboot)
	// SOFT_RST - Reset config and user registers (0:default, 1:reset)
	tempRegValue = 0;
	switch (scale)
	{
	case 8:
		tempRegValue |= (0x1 << 5);
		MagnetSensitivity = SENSITIVITY_MAGNETOMETER_8;
		break;
	case 12:
		tempRegValue |= (0x2 << 5);
		MagnetSensitivity = SENSITIVITY_MAGNETOMETER_12;
		break;
	case 16:
		tempRegValue |= (0x3 << 5);
		MagnetSensitivity = SENSITIVITY_MAGNETOMETER_16;
		break;
		// Otherwise we'll default to 4 gauss (00)
	}
	WriteByte(LSM9DS1_M,CTRL_REG2_M, tempRegValue);

	// CTRL_REG3_M (Default value: 0x03)
	// [I2C_DISABLE][0][LP][0][0][SIM][MD1][MD0]
	// I2C_DISABLE - Disable I2C interace (0:enable, 1:disable)
	// LP - Low-power mode cofiguration (1:enable)
	// SIM - SPI mode selection (0:write-only, 1:read/write enable)
	// MD[1:0] - Operating mode
	//	00:continuous conversion, 01:single-conversion,
	//  10,11: Power-down
	WriteByte(LSM9DS1_M,CTRL_REG3_M, 0); // Continuous conversion mode

	// CTRL_REG4_M (Default value: 0x00)
	// [0][0][0][0][OMZ1][OMZ0][BLE][0]
	// OMZ[1:0] - Z-axis operative mode selection
	//	00:low-power mode, 01:medium performance
	//	10:high performance, 10:ultra-high performance
	// BLE - Big/little endian data
	tempRegValue = 0;
	tempRegValue = (0b11 & 0x3) << 2;
	WriteByte(LSM9DS1_M,CTRL_REG4_M, 0b1000);

	// CTRL_REG5_M (Default value: 0x00)
	// [0][BDU][0][0][0][0][0][0]
	// BDU - Block data update for magnetic data
	//	0:continuous, 1:not updated until MSB/LSB are read
	WriteByte(LSM9DS1_M,CTRL_REG5_M, 0);
}

bool LSM_9DS1::Initialize(int AccelScale, int GyroScale, int MagnetScale){
	Wire.begin();

	//=====接続確認=====
	uint8_t mTest = ReadByte(LSM9DS1_M,WHO_AM_I_M);
	uint8_t agTest = ReadByte(LSM9DS1_AG,WHO_AM_I_XG);
	uint16_t whoAmICombined = (agTest << 8) | mTest;
	if (whoAmICombined != ((WHO_AM_I_AG_RSP << 8) | WHO_AM_I_M_RSP)){
		return false;
	}

	//=====センサ初期化=====
	AccInit(AccelScale);
	GyrInit(GyroScale);
	MagInit(MagnetScale);

	return true;
}

bool LSM_9DS1::ReadAcc(float *x, float *y, float *z){
	uint8_t status = ReadByte(LSM9DS1_AG,STATUS_REG_1);
	if(status & 0b01){
		uint8_t temp[6];
		ReadBytes(LSM9DS1_AG, OUT_X_L_XL, temp, 6);

		int16_t temp_;

		temp_ = (temp[1] << 8) | temp[0];
		*x = temp_ * AccelSensitivity;

		temp_ = (temp[3] << 8) | temp[2];
		*y = temp_ * AccelSensitivity;

		temp_ = (temp[5] << 8) | temp[4];
		*z = temp_ * AccelSensitivity;
		return true;
	}else{
		return false;
	}
}

bool LSM_9DS1::ReadGyr(float *x, float *y, float *z){

	uint8_t status = ReadByte(LSM9DS1_AG,STATUS_REG_1);
	if((status & 0b10) >> 1)
	{
		uint8_t temp[6];
		ReadBytes(LSM9DS1_AG, OUT_X_L_G, temp, 6);

		int16_t temp_;

		temp_ = (temp[1] << 8) | temp[0];
		*x = temp_ * GyroSensitivity;

		temp_ = (temp[3] << 8) | temp[2];
		*y = temp_ * GyroSensitivity;

		temp_ = (temp[5] << 8) | temp[4];
		*z = temp_ * GyroSensitivity;
		return true;
	}
	else
	{
		return false;
	}
}

bool LSM_9DS1::ReadMag(float *x, float *y, float *z){
	uint8_t status = ReadByte(LSM9DS1_M, STATUS_REG_1);
	if (status & 1)
	{
		uint8_t temp[6];
		ReadBytes(LSM9DS1_M, OUT_X_L_M, temp, 6);

		int16_t temp_;

		temp_ = (temp[1] << 8) | temp[0];
		*x = temp_ * GyroSensitivity;

		temp_ = (temp[3] << 8) | temp[2];
		*y = temp_ * GyroSensitivity;

		temp_ = (temp[5] << 8) | temp[4];
		*z = temp_ * GyroSensitivity;
		return true;
	}
	else
	{
		return false;
	}
}