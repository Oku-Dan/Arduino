#include <SD.h>

File file;
unsigned int count[32], counter = 0;
unsigned long time[32];
float x[32], y[32], z[32];

void setup() {
  Serial.begin(230400);
  SD.begin(4);
  if (SD.exists("data.txt"))
    SD.remove("data.txt");
}

void loop() {
  count[counter % 32] = counter;
  time[counter % 32] = micros();
  x[counter % 32] = random(0, 100);
  y[counter % 32] = random(0, 100);
  z[counter% 32] = random(0, 100);
  if(counter % 32 == 31){
    WriteDatas();
  }
  counter++;
}

void WriteDatas(){
  file = SD.open("data.txt", FILE_WRITE);
  for (int i = 0; i < 32;i++){
    file.print("No.");
    file.print(count[i]);
    file.print(" ");
    file.print(time[i]);
    file.print(" : ");
    file.print(x[i]);
    file.print(" ");
    file.print(y[i]);
    file.print(" ");
    file.print(z[i]);
    if(i < 1023)file.println();
    else file.println("---------------------------");
  }
  file.close();
}