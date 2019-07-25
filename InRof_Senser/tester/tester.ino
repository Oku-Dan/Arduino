#define LineSenser_0 23
#define LineSenser_1 24
#define LineSenser_2 25
#define LineSenser_3 26

void setup() {
  pinMode(LineSenser_0,INPUT);
  pinMode(LineSenser_1,INPUT);
  pinMode(LineSenser_2,INPUT);
  pinMode(LineSenser_3,INPUT);
  Serial.begin(115200);
}

void loop() {
  Serial.print(digitalRead(LineSenser_0) == HIGH ? "□" : "■");
  Serial.print(digitalRead(LineSenser_1) == HIGH ? "□" : "■");
  Serial.print(digitalRead(LineSenser_2) == HIGH ? "□" : "■");
  Serial.print(digitalRead(LineSenser_3) == HIGH ? "□" : "■");
  Serial.print('\n');
  delay(100);
}
