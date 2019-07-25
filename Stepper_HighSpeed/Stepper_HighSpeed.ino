#define Stepper_R_1 5
#define Stepper_R_2 6
#define Stepper_L_1 7
#define Stepper_L_2 8
#define forward 0b01111000

char direction_R, direction_L, shift_R = 0, shift_L = 0;
int rps = 10;

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

void setup() {
  pinMode(Stepper_R_1,OUTPUT);
  pinMode(Stepper_R_2,OUTPUT);
  pinMode(Stepper_L_1,OUTPUT);
  pinMode(Stepper_L_2,OUTPUT);
  Serial.begin(9600);
  direction_R = forward;
  direction_L = forward;

  unsigned long timer,delaytime,lasttime = 0;
  char cs[100] = "334554321123322 33455432112322";
  
  
   for(int i = 0;i < sizeof(cs);i++){
    timer = micros();
    if(timer - lasttime > 300000UL){
      switch(cs[i]){
        case '1':
          rps = 440;
          break;
        case '2':
          rps = 440 * 1.122;
          break;
        case '3':
          rps = 440 * 1.26;
          break;
        case '4':
          rps = 440 * 1.335;
          break;
        case '5':
          rps = 440 * 1.5;
          break;
        case '6':
          rps = 440 * 1.68;
          break;
        case '7':
          rps = 440 * 1.89;
          break;
        case '8':
          rps = 440 * 2;
          break;
        case ' ':
          rps = 100000;
          break; 
      }
    }
    
    Step_L();
    Step_R();
    delaytime = 1000000UL / rps - (micros() - timer);
    if(delaytime < 0)Serial.println("Over Running!!");
    else delayMicroseconds(delaytime);
  }
}  

void loop() {
}
