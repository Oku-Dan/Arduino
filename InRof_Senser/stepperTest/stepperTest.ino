#define Stepper_R_1 5
#define Stepper_R_2 6
#define Stepper_L_1 7
#define Stepper_L_2 8
#define Stepper_T_1 4
#define Stepper_T_2 18
//#define ms 20

bool dir = true;
unsigned long ms = 20;

void setup() {
  pinMode(Stepper_R_1,OUTPUT);
  pinMode(Stepper_R_2,OUTPUT);
  pinMode(Stepper_L_1,OUTPUT);
  pinMode(Stepper_L_2,OUTPUT);
  pinMode(Stepper_T_1,OUTPUT);
  pinMode(Stepper_T_2,OUTPUT);
}

void loop(){
  if(dir){
    digitalWrite(Stepper_R_1,LOW);
    digitalWrite(Stepper_R_2,LOW);
    digitalWrite(Stepper_L_1,LOW);
    digitalWrite(Stepper_L_2,LOW);
    digitalWrite(Stepper_T_1,LOW);
    digitalWrite(Stepper_T_2,LOW);
    delay(ms);
    digitalWrite(Stepper_R_1,LOW);
    digitalWrite(Stepper_R_2,HIGH);
    digitalWrite(Stepper_L_1,LOW);
    digitalWrite(Stepper_L_2,HIGH);
    digitalWrite(Stepper_T_1,LOW);
    digitalWrite(Stepper_T_2,HIGH);
    delay(ms);
    digitalWrite(Stepper_R_1,HIGH);
    digitalWrite(Stepper_R_2,HIGH);
    digitalWrite(Stepper_L_1,HIGH);
    digitalWrite(Stepper_L_2,HIGH);
    digitalWrite(Stepper_T_1,HIGH);
    digitalWrite(Stepper_T_2,HIGH);
    delay(ms);
    digitalWrite(Stepper_R_1,HIGH);
    digitalWrite(Stepper_R_2,LOW);
    digitalWrite(Stepper_L_1,HIGH);
    digitalWrite(Stepper_L_2,LOW);
    digitalWrite(Stepper_T_1,HIGH);
    digitalWrite(Stepper_T_2,LOW);
    delay(ms);
  }else{
    digitalWrite(Stepper_R_1,LOW);
    digitalWrite(Stepper_R_2,LOW);
    digitalWrite(Stepper_L_1,LOW);
    digitalWrite(Stepper_L_2,LOW);
    digitalWrite(Stepper_T_1,LOW);
    digitalWrite(Stepper_T_2,LOW);
    delay(ms);
    digitalWrite(Stepper_R_1,HIGH);
    digitalWrite(Stepper_R_2,LOW);
    digitalWrite(Stepper_L_1,HIGH);
    digitalWrite(Stepper_L_2,LOW);
    digitalWrite(Stepper_T_1,HIGH);
    digitalWrite(Stepper_T_2,LOW);
    delay(ms);
    digitalWrite(Stepper_R_1,HIGH);
    digitalWrite(Stepper_R_2,HIGH);
    digitalWrite(Stepper_L_1,HIGH);
    digitalWrite(Stepper_L_2,HIGH);
    digitalWrite(Stepper_T_1,HIGH);
    digitalWrite(Stepper_T_2,HIGH);
    delay(ms);
    digitalWrite(Stepper_R_1,LOW);
    digitalWrite(Stepper_R_2,HIGH);
    digitalWrite(Stepper_L_1,LOW);
    digitalWrite(Stepper_L_2,HIGH);
    digitalWrite(Stepper_T_1,LOW);
    digitalWrite(Stepper_T_2,HIGH);
    delay(ms);
  }
}
