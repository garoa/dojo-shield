/*
  Cobrinha
  
  Animação com dois segmentos acesos de cada vez, 
  fazendo trajeto em formato de "8" no display. 

  Este exemplo está em domínio público.
 */

//               A  B  G  E   D   C   G  F 
int display[] = {8, 9, 7, 12, 11, 10, 7, 13};
const int PASSOS = 8;

void setup() {
  for (int i=7; i<=13; i++) {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  int cauda = display[PASSOS-1];
  digitalWrite(cauda, HIGH);
  for (int i=0; i<PASSOS; i++) {
    digitalWrite(display[i], HIGH);
    delay(analogRead(A0));              
    digitalWrite(cauda, LOW);
    cauda = display[i];   
  }
}
