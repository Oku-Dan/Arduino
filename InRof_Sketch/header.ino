#define Stepper_R_1 5
#define Stepper_R_2 6
#define Stepper_L_1 7
#define Stepper_L_2 8
#define Stepper_T_1 4
#define Stepper_T_2 2
#define Servo_1 9
#define Servo_2 10
#define Releaser 11
#define Turbine 3
#define LineSenser_0 A3
#define LineSenser_1 A2
#define LineSenser_2 A1
#define LineSenser_3 A0

#define forward 0b01111000
#define backward 0b10110100
#define MINIMAM_INTERVAL 20000
#define MAXIMAM_INTERVAL 60000
#define LENGTH_ARM 80
#define ScanRange 120

#include <Servo.h>
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;
Servo servo_1,servo_2,releaser;
char direction_R, direction_L, direction_T, shift_R = 0, shift_L = 0,now_index = 0;
float distanceToBall,directionToBall;

void Step_R(){
	digitalWrite(Stepper_R_1, direction_R >> shift_R & 0b00000001);
	digitalWrite(Stepper_R_2, direction_R >> shift_R + 1 & 0b00000001);
	shift_R < 6 ? shift_R += 2 : shift_R = 0;
}

void Step_L(){
	digitalWrite(Stepper_L_1, direction_L >> shift_L & 0b00000001);
	digitalWrite(Stepper_L_2, direction_L >> shift_L + 1 & 0b00000001);
	shift_L < 6 ? shift_L += 2 : shift_L = 0;
}

void Run(){
	direction_R = forward;
	direction_L = forward;

	unsigned long time, last_R, last_L, last_S, interval_R, interval_L, interval_S;
	bool senser_R = false, senser_L = false,
		 last_senser_L = false, last_senser_R = false,
		 onRight = false,onRight_seted = false;
	int EdgeCounter = 0;

	direction_R = forward;
	direction_L = forward;
	last_R = 500;
	last_L = 500;
	last_S = 0;
	interval_R = MAXIMAM_INTERVAL;
	interval_L = MAXIMAM_INTERVAL;
	interval_S = 10000;

	while (1)
	{
		time = micros();

		if (digitalRead(LineSenser_0) && digitalRead(LineSenser_3))
		{
			if (EdgeCounter > 5)
				break;
			else
				EdgeCounter++;
		}

		if (time - last_S >= interval_S)
		{
			bool temp;
			last_senser_R = senser_R;
			senser_R = digitalRead(LineSenser_1);
			last_senser_L = senser_L;
			senser_L = digitalRead(LineSenser_2);

			if (senser_L && senser_R)
			{
				if (last_senser_L && !last_senser_R)
				{
					interval_L = interval_R * 0.98;
				}
				else if (!last_senser_L && last_senser_R)
				{
					interval_R = interval_L * 0.98;
				}
				else if (last_senser_L && last_senser_R)
				{
					if (interval_R > MINIMAM_INTERVAL && interval_L > MINIMAM_INTERVAL)
					{
						interval_R *= 0.98;
						interval_L *= 0.98;
					}
				}
			}
			if (!senser_L && senser_R)
			{
				onRight = false;
				onRight_seted = true;
				if (last_senser_L && last_senser_R)
				{
					interval_R = interval_L * 1.05;
				}
				else if (!last_senser_L && last_senser_R)
				{
					interval_R += (MAXIMAM_INTERVAL - interval_R) / 5;
					interval_L -= (interval_L - MINIMAM_INTERVAL) / 5;
				}
				else
				{
					interval_R = interval_L;
				}
			}
			if (senser_L && !senser_R)
			{
				onRight = true;
				onRight_seted = true;
				if (last_senser_L && last_senser_R)
				{
					interval_L = interval_R * 1.05;
				}
				else if (last_senser_L && !last_senser_R)
				{
					interval_L += (MAXIMAM_INTERVAL - interval_L) / 5;
					interval_R -= (interval_R - MINIMAM_INTERVAL) / 5;
				}
				else
				{
					interval_L = interval_R;
				}
			}
			if (!senser_L && !senser_R)
			{
				if (!onRight && onRight_seted)
				{
					interval_R = 1000000;
					interval_L = MINIMAM_INTERVAL;
				}
				else if (onRight && onRight_seted)
				{
					interval_R = MINIMAM_INTERVAL;
					interval_L = 1000000;
				}
				else{
					interval_R = MAXIMAM_INTERVAL;
					interval_L = MAXIMAM_INTERVAL;
				}
			}
		}

		if (time - last_R >= interval_R)
		{
			Step_R();
			last_R = time;
		}
		if (time - last_L >= interval_L)
		{
			Step_L();
			last_L = time;
		}
	}

	delay(300);
	EdgeCounter = 0;
	while (EdgeCounter < 5)
	{
		if (!digitalRead(LineSenser_0) && !digitalRead(LineSenser_3))EdgeCounter++;
		Step_R();
		Step_L();
		delayMicroseconds(MAXIMAM_INTERVAL);
		if (digitalRead(LineSenser_0))
			Step_R();
		if (digitalRead(LineSenser_3))
			Step_L();
		delayMicroseconds(MAXIMAM_INTERVAL);
	}
	return;
}

void RunToLine(){
	int EdgeCounter = 0;
	direction_R = forward;
	direction_L = forward;
	bool flag_R = false, flag_L = false;
	while (EdgeCounter < 5)
	{
		Step_R();
		Step_L();
		if (!digitalRead(LineSenser_0) && !digitalRead(LineSenser_0))
			EdgeCounter++;
		delayMicroseconds(MINIMAM_INTERVAL * 1.5);
	}
	while (EdgeCounter >= 0){
		if (!digitalRead(LineSenser_0)){
			Step_R();
			flag_R = true;
		}else
			flag_R = false;
		if (!digitalRead(LineSenser_3)){
			Step_L();
			flag_L = true;
		}else
			flag_L = false;
		if (!flag_R && !flag_L)
			EdgeCounter--;
		delayMicroseconds(MINIMAM_INTERVAL * 1.5);
	}
	return;
}

void RunExactly(float distance){
	if(distance > 0){
		direction_R = forward;
		direction_L = forward;
	}else{
		direction_R = backward;
		direction_L = backward;
	}

	for (int i = 0; i < abs(distance) / 1.35; i++)
	{
		Step_R();
		Step_L();
		delayMicroseconds(MAXIMAM_INTERVAL * 1.5);
	}
	return;
}

void PushBall(){
	for (int i = 90; i < 175; i++){
		releaser.write(i);
		delay(500 / 90);
	}
	releaser.write(90);
	return;
}

void Turn(int angle){
	if (angle > 0)
	{
		direction_R = forward;
		direction_L = backward;
	}
	else
	{
		direction_R = backward;
		direction_L = forward;
	}
	delay(300);
	for (size_t i = 0; i < 105; i++)
	{
		Step_R();
		Step_L();
		delayMicroseconds(MAXIMAM_INTERVAL * 1.5);
	}
	delay(300);
	return;
}

void TurnExactly(float angle){

	if (angle > 0)
	{
		direction_R = forward;
		direction_L = backward;
	}
	else
	{
		direction_R = backward;
		direction_L = forward;
	}
	delay(300);

	for (size_t i = 0; i < 105.0 * (float)abs(angle) / 90.0; i++)
	{
		Step_R();
		Step_L();
		delayMicroseconds(MAXIMAM_INTERVAL * 1.5);
	}
	delay(300);
	return;
}

void TurbineStart(){
	digitalWrite(Turbine,HIGH);
	return;
}

void TurbineStop(){
	digitalWrite(Turbine,LOW);
	return;
}

void GlabBall(){
	int pos;
	for (pos = 90; pos >= 1; pos -= 1)
	{
		servo_1.write(pos);
		delay(50);
	}
	for (pos = 45; pos <= 160; pos += 1)
	{
		servo_2.write(pos);
		delayMicroseconds(25000);
	}
	return;
}

void ReleaseBall(){
	int pos;
	for (pos = 1; pos <= 90; pos += 1)
	{
		servo_1.write(pos + 3);
		servo_2.write(160 - 115 * pos/90.0);
		delay(15);
	}
	return;
}

void TurnTable(int index){
	direction_T = now_index - index ? forward : backward;
	char shift = 0;

	for (int i = 0; i < abs(index - now_index) * 200 / 6 + 1; i++)
	{
		digitalWrite(Stepper_T_1, direction_T >> shift & 0b00000001);
		digitalWrite(Stepper_T_2, direction_T >> shift + 1 & 0b00000001);
		delay(60);
		shift < 6 ? shift += 2 : shift = 0;
	}

	now_index = index;
	return;
}

void Initialize() {
	servo_1.attach(Servo_1);
	servo_2.attach(Servo_2);
	releaser.attach(Releaser);
	pinMode(13,OUTPUT);
	pinMode(Turbine,OUTPUT);
	pinMode(LineSenser_0,INPUT);
	pinMode(LineSenser_1,INPUT);
	pinMode(LineSenser_2,INPUT);
	pinMode(LineSenser_3,INPUT);
	pinMode(Stepper_R_1,OUTPUT);
	pinMode(Stepper_R_2,OUTPUT);
	pinMode(Stepper_L_1,OUTPUT);
	pinMode(Stepper_L_2,OUTPUT);
	pinMode(Stepper_T_1,OUTPUT);
	pinMode(Stepper_T_2,OUTPUT);
	digitalWrite(Turbine,LOW);
	GlabBall();
	Wire.begin();
	sensor.init();
	sensor.setTimeout(80);
	TurnTable(1);
}

bool ScanBall()
{
	uint16_t minDistance = 10000;
	int counter;

	RunExactly(-50);
	TurnExactly(ScanRange / 2);
	direction_R = backward;
	direction_L = forward;

	int edge1, edge2;
	bool flag = false;
	delay(100);
	unsigned long time;
	for (int i = 0; i <= ScanRange * 105 / 90; i++)
	{
		time = millis();
		uint16_t distance = sensor.readRangeSingleMillimeters();
		if(distance < minDistance){
			minDistance = distance;
			counter = i;
			edge1 = i;
			flag = true;
		}
		if(flag && abs(distance - minDistance) < 20){
			edge2 = i;
		}else{
			flag = false;
		}
		Step_R();
		Step_L();
		int delaytime = 60 - (millis() - time);
		if(delaytime > 0)delay(delaytime);
	}
	delay(200);
	
	if(minDistance < 500 && minDistance > 10){
		distanceToBall = minDistance;
		directionToBall = -(edge1 + edge2) / 2 * 90 / 105 + ScanRange / 2;
		return true;
	}
	else
	{
		TurnExactly(ScanRange / 2);
		RunExactly(50);
		return false;
	}
}

void KeepBall(){
	TurnTable((now_index + 1) % 6 + 1);

	direction_R = forward;
	direction_L = backward;

	delay(100);
	unsigned long time;
	for (int i = 0; i <= (directionToBall + ScanRange / 2) * 105 / 90; i++)
	{
		time = millis();
		Step_R();
		Step_L();
		int delaytime = 60 - (millis() - time);
		if(delaytime > 0)
			delay(delaytime);
	}
	delay(200);
	RunExactly(-50);
	ReleaseBall();
	TurbineStart();
	if (50 + distanceToBall - LENGTH_ARM > 0){
		direction_R = forward;
		direction_L = forward;
	}else{
		direction_R = backward;
		direction_L = backward;
	}

	for (int i = 0; i < abs(50 + distanceToBall - LENGTH_ARM) / 1.35; i++)
	{
		Step_R();
		Step_L();
		delayMicroseconds(MAXIMAM_INTERVAL * 1.5);
	}
	delay(4000);

	direction_R = backward;
	direction_L = backward;

	for (int i = 0; i < 30 / 1.35; i++)
	{
		Step_R();
		Step_L();
		delayMicroseconds(MAXIMAM_INTERVAL * 1.5);
	}
	delay(2000);
	GlabBall();
	TurbineStop();
	delay(2000);
	RunExactly(-(distanceToBall - LENGTH_ARM) + 30);
	TurnExactly(-directionToBall);
}

void FirstKeepBall(){

	direction_R = forward;
	direction_L = backward;

	delay(100);
	unsigned long time;
	for (int i = 0; i <= (directionToBall + ScanRange / 2) * 105 / 90; i++)
	{
		time = millis();
		Step_R();
		Step_L();
		int delaytime = 60 - (millis() - time);
		if(delaytime > 0)delay(delaytime);
	}
	
	delay(200);
	RunExactly(-50);
	ReleaseBall();
	TurbineStart();
	if (50 + distanceToBall - LENGTH_ARM > 0)
	{
		direction_R = forward;
		direction_L = forward;
	}
	else
	{
		direction_R = backward;
		direction_L = backward;
	}

	for (int i = 0; i < abs(50 + distanceToBall - LENGTH_ARM) / 1.35; i++)
	{
		Step_R();
		Step_L();
		delayMicroseconds(MAXIMAM_INTERVAL * 1.5);
	}
	delay(6000);

	direction_R = backward;
	direction_L = backward;

	for (int i = 0; i < 30 / 1.35; i++)
	{
		Step_R();
		Step_L();
		delayMicroseconds(MAXIMAM_INTERVAL * 1.5);
	}
	int pos;
	for (pos = 90; pos >= 1; pos -= 1)
	{
		servo_1.write(pos);
		delay(50);
	}
	for (pos = 45; pos <= 80; pos += 1)
	{
		servo_2.write(pos);
		delayMicroseconds(25000);
	}
	TurbineStop();
	delay(2000);
	RunExactly(-(distanceToBall - LENGTH_ARM + 50));
	TurnExactly(-directionToBall);
}

void RunExactlyOnLine(float distance){
	if (distance > 0)
	{
		direction_R = forward;
		direction_L = forward;

		for (int i = 0; i < abs(distance) / 1.35; i++)
		{
			if (digitalRead(LineSenser_2))
				Step_R();
			if (digitalRead(LineSenser_1))
				Step_L();
			delayMicroseconds(MAXIMAM_INTERVAL * 1.5);
		}
	}
	else
	{
		direction_R = backward;
		direction_L = backward;

		for (int i = 0; i < abs(distance) / 1.35; i++)
		{
			if (digitalRead(LineSenser_1))
				Step_R();
			if (digitalRead(LineSenser_2))
				Step_L();
			delayMicroseconds(MAXIMAM_INTERVAL * 1.5);
		}
	}
	return;
}