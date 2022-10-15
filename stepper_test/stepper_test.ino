#define bpm 20
#define steps_turn 200
#define gear_ratio 3.6 //(72/20)


int dir = 9;
int en = 10;
int pul = 8;
int delayTime

void setup() {
pinMode(pul, OUTPUT);
pinMode(en, OUTPUT);
pinMode(dir, OUTPUT);
digitalWrite(en, LOW);
digitalWrite(dir, HIGH);
digitalWrite(pul, LOW);

float steps_breath = steps_turn*gear_ratio;
float microseconds_rotation = 60000000/bpm;
delay_time = int(microseconds_rotation/steps_breath/2);

}

void loop() {
  digitalWrite(pul, HIGH);
  delayMicroseconds(delay_time);
  digitalWrite(pul, LOW);
  delayMicroseconds(delay_time);

}
