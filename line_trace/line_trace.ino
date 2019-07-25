#define Stepper_R_1 5
#define Stepper_R_2 6
#define Stepper_L_1 7
#define Stepper_L_2 8
#define Stepper_T_1 4
#define Stepper_T_2 18
#define LineSenser_0 A3
#define LineSenser_1 A2
#define LineSenser_2 A1
#define LineSenser_3 A0
#define forward 0b01111000
#define backward 0b10110100
#define MINIMAM_INTERVAL 20000
#define MAXIMAM_INTERVAL 60000

char direction_R, direction_L, direction_T, shift_R = 0, shift_L = 0;

void Initialize()
{
    pinMode(13, OUTPUT);
    pinMode(LineSenser_0, INPUT);
    pinMode(LineSenser_1, INPUT);
    pinMode(LineSenser_2, INPUT);
    pinMode(LineSenser_3, INPUT);
    pinMode(Stepper_R_1, OUTPUT);
    pinMode(Stepper_R_2, OUTPUT);
    pinMode(Stepper_L_1, OUTPUT);
    pinMode(Stepper_L_2, OUTPUT);
    pinMode(Stepper_T_1, OUTPUT);
    pinMode(Stepper_T_2, OUTPUT);
}

void Run()
{
    unsigned long time, last_R, last_L,last_S, interval_R, interval_L, interval_S;
    bool senser_R = false, senser_L = false,
         last_senser_L = false, last_senser_R = false,
         onRight = false;
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
        /*interval_Sごとに実行*/
        time = micros();

        if (digitalRead(LineSenser_0) && digitalRead(LineSenser_3)){
            if(EdgeCounter > 5)break;
            else EdgeCounter++;
        }

        if(time - last_S >= interval_S){
            bool temp;
            last_senser_R = senser_R;
            senser_R = digitalRead(LineSenser_1);

            last_senser_L = senser_L;
            senser_L = digitalRead(LineSenser_2);

            if(senser_L && senser_R)
            {
                if(last_senser_L && !last_senser_R){
                    interval_L = interval_R * 0.98;
                }
                else if (!last_senser_L && last_senser_R){
                    interval_R = interval_L * 0.98;
                }
                else if (last_senser_L && last_senser_R){
                    if (interval_R > MINIMAM_INTERVAL && interval_L > MINIMAM_INTERVAL){
                        interval_R *= 0.98;
                        interval_L *= 0.98;
                    }
                }
            }
            if(!senser_L && senser_R)
            {
                onRight = false;
                if (last_senser_L && last_senser_R)
                {
                    interval_R = interval_L * 1.05;
                }
                else if (!last_senser_L && last_senser_R){
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
            if(!senser_L && !senser_R){
                if (!onRight){
                    interval_R = 1000000;
                    interval_L = MINIMAM_INTERVAL;
                }else{
                    interval_R = MINIMAM_INTERVAL;
                    interval_L = 1000000;
                }
            }
        }

        if (time - last_R >= interval_R){
            Step_R();
            last_R = time;
        }
        if (time - last_L >= interval_L){
            Step_L();
            last_L = time;
        }
    }

    delay(500);
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

void Turn(int direction)
{
    if (direction > 0)
    {
        direction_R = backward;
        direction_L = forward;
    }
    else
    {
        direction_R = forward;
        direction_L = backward;
    }
    delay(300);
    for (size_t i = 0; i < 108; i++)
    {
        Step_R();
        Step_L();
        //delayMicroseconds(/*MAXIMAM_INTERVAL */);
        delay(60);
    }
    delay(300);
}

void Step_R()
{
    digitalWrite(Stepper_R_1, direction_R >> shift_R & 0b00000001);
    digitalWrite(Stepper_R_2, direction_R >> shift_R + 1 & 0b00000001);
    shift_R < 6 ? shift_R += 2 : shift_R = 0;
}

void Step_L()
{
    digitalWrite(Stepper_L_1, direction_L >> shift_L & 0b00000001);
    digitalWrite(Stepper_L_2, direction_L >> shift_L + 1 & 0b00000001);
    shift_L < 6 ? shift_L += 2 : shift_L = 0;
}

void setup()
{
    Initialize();
}

void loop()
{
    Run();
    delay(500);
    Turn(1);
    delay(500);
    Turn(-1);
    delay(500);
    Run();
    Turn(1);
    Turn(1);
    delay(500);
}
