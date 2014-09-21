const int STEPS = 60;
const int POT_SCALE = 20;
const int LED = 11; 
const int POT = A0;

int step = 0;

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  step++;
  step %= STEPS;
  float angle = 2 * PI * step / STEPS; 
  analogWrite(LED, int(sin(angle)*127 + 127));
  delay(analogRead(POT)/POT_SCALE;
}
