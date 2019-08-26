#include <SparkFunLSM9DS1.h>

#define LSM9DS1_M 0x1E
#define LSM9DS1_AG 0x6B
#define DECLINATION -0.74 // déclinaison (en degrés) pour Japan.

LSM9DS1 imu;

uint16_t connectionState = 0;
bool GyrUpdated = false, AccUpdated = false, MagUpdated = false;

bool Initialize_LSM9DS1()
{
	imu.settings.device.commInterface = IMU_MODE_I2C;
	imu.settings.device.mAddress = LSM9DS1_M;
	imu.settings.device.agAddress = LSM9DS1_AG;
	imu.settings.accel.scale = 16;	//2,4,8,16
	imu.settings.gyro.scale = 500;			//245,500,2000
	imu.settings.mag.scale = 8;   			//4,8,12,16
	return imu.begin() != 0;
}

bool ReadAcc(float *ax,float *ay,float *az)
{
	if (!imu.accelAvailable())
	{
		return false;
	}
	imu.readAccel();
	*ax = imu.calcAccel(imu.ax);
	*ay = imu.calcAccel(imu.ay);
	*az = imu.calcAccel(imu.az);
	return true;
}

bool ReadGyr(float *gx, float *gy, float *gz){
	if (!imu.gyroAvailable())
	{
		return false;
	}
	imu.readGyro();
	*gx = imu.calcGyro(imu.gx);
	*gy = imu.calcGyro(imu.gy);
	*gz = imu.calcGyro(imu.gz);
	return true;
}

bool ReadMag(float *mx, float *my, float *mz)
{
	if (!imu.magAvailable())
	{
		return false;
	}
	imu.readMag();
	*mx = imu.calcMag(imu.mx);
	*my = imu.calcMag(imu.my);
	*mz = imu.calcMag(imu.mz);
	return true;
}

bool FusionAccGryMag(float *roll, float *pitch,float *heading)
{
	if (imu.accelAvailable())
		imu.readAccel();
	else
		return false;
	if(imu.gyroAvailable())
		imu.readGyro();
	else
		return false;
	if(imu.magAvailable())
		imu.readMag();
	else
		return false;
	
	*roll = atan2(imu.ay, imu.az);
	*pitch = atan2(-imu.ax, sqrt(imu.ay * imu.ay + imu.az * imu.az));

	if (imu.my == 0)
		*heading = (imu.mx < 0) ? PI : 0;
	else
		*heading = atan2(imu.mx, imu.my);

	*heading -= DECLINATION * PI / 180;

	if (*heading > PI)
		*heading -= (2 * PI);
	else if (*heading < -PI)
		*heading += (2 * PI);

	*heading *= 180.0 / PI;
	*pitch *= 180.0 / PI;
	*roll *= 180.0 / PI;
	return true;
}

void Calibrate_LSM9DS1(){
	if(Serial){
		Serial.println("Accel senser and Gyro senser will be calibrated.");
		Serial.println("Put it on stable palce.");
	}
	imu.calibrate(true);
	if (Serial)
	{
		Serial.println("Accel senser and Gyro senser has been calibrated.");
		Serial.println("Then, Magnetic senser will be calibrated.");
		Serial.println("Rotate it.");
	}
	imu.calibrateMag(true);
	if (Serial)
	{
		Serial.println("Magnetic senser has been calibrated.");
	}
}