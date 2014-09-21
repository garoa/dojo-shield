/*
  Animação sequencial
  
  Animação tipo luz sequencial usando os 6 segmentos que formam 
  o dígito zero: acende um segmento, deixa um tempo aceso, apaga
  e imediatamente acende o próximo, repetindo para sempre. 

  Este exemplo está em domínio público.
 */

const int PRIMEIRO = 8; // segmento A
const int ULTIMO = 13;  // segmento F

void setup() {
  for (int i=PRIMEIRO; i<=ULTIMO; i++) {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  for (int i=PRIMEIRO; i<=ULTIMO; i++) {
    digitalWrite(i, HIGH);
    delay(analogRead(A0));              
    digitalWrite(i, LOW);   
  }
}
