# 1 "c:\\Users\\OKU_DAN\\source\\Arduino\\MPU6050_DMP\\MPU6050_DMP.ino"
# 1 "c:\\Users\\OKU_DAN\\source\\Arduino\\MPU6050_DMP\\MPU6050_DMP.ino"
# 2 "c:\\Users\\OKU_DAN\\source\\Arduino\\MPU6050_DMP\\MPU6050_DMP.ino" 2
# 3 "c:\\Users\\OKU_DAN\\source\\Arduino\\MPU6050_DMP\\MPU6050_DMP.ino" 2


/* 0 --> 167Hz 1 --> 125Hz

   * 4 --> 100Hz 9 --> 50Hz

     19 --> 25Hz            */
# 9 "c:\\Users\\OKU_DAN\\source\\Arduino\\MPU6050_DMP\\MPU6050_DMP.ino"
/* 

#define OFFSET_ACCEL_X 819

#define OFFSET_ACCEL_Y 2570

#define OFFSET_ACCEL_Z 1260

#define OFFSET_GYRO_X 52

#define OFFSET_GYRO_Y 11

#define OFFSET_GYRO_Z 2

*/
# 18 "c:\\Users\\OKU_DAN\\source\\Arduino\\MPU6050_DMP\\MPU6050_DMP.ino"
//xda焦げてる方
# 27 "c:\\Users\\OKU_DAN\\source\\Arduino\\MPU6050_DMP\\MPU6050_DMP.ino"
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
  Serial.println(now - last);
  last = now;
 }
}

void Initialize(){

 Wire.begin();
 Wire.setClock(400000UL);
 pinMode(13, 0x1);

 mpu.initialize();
 while (!mpu.testConnection());
 mpu.dmpInitialize();
 mpu.setDMPEnabled(true);

 mpu.setRate(9);
 mpu.setXAccelOffset(-3821);
 mpu.setYAccelOffset(-847);
 mpu.setZAccelOffset(1257);
 mpu.setXGyroOffset(87);
 mpu.setYGyroOffset(15);
 mpu.setZGyroOffset(-20);

 attachInterrupt(0, Interrupt, 3);
 datasize = mpu.dmpGetFIFOPacketSize();
}

void Interrupt(){
 DataReceivedFlag = true;
}

void DataProcessing(){
 digitalWrite(13, 0x1);
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
 digitalWrite(13, 0x0);
}
