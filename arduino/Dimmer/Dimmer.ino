/* Dimmer.ino

Usar o potenciômetro para aumentar ou diminuir o brilho
de um LED ligado a um pino que suporta PWM.

*/

int led = 11; 
int pot = A0;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  int valor = analogRead(pot) / 4; // 1023 -> 255
  analogWrite(led, valor);
}
