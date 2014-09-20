/*
  Piscar
  Acende um LED por 1 segundo, apaga por 1 segundo, repetidamente.
 
  Este exemplo está em domínio público.
 */
 
// Pino 13 está ligado a um led na maioria dos Arduinos.
// dar um nome ao pino 13:
int led = 13;

// a função setup executa uma vez quando se pressiona RESET
void setup() {                
  // inicializar um pino digital como saída
  pinMode(led, OUTPUT);     
}

// a função loop é executada repetidamente, para sempre
void loop() {
  digitalWrite(led, HIGH);   // ligar o LED (HIGH é o nível de tensão "alta", ex. 5V)
  delay(1000);               // esperar 1 segundo
  digitalWrite(led, LOW);    // desligar o LED (LOW é o nível de tensão "baixa", 0V)
  delay(1000);               // esperar 1 segundo
}
