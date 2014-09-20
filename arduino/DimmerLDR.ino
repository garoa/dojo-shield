#include <stdlib.h>

int seg_alto = 8; 
int seg_meio = 7; 
int seg_baixo = 11; 
int ldr = A1;
int ldr_min = 1023;
int ldr_max = 0;

int const QT_AMOSTRA = 100;
int const TEMPO_AMOSTRAGEM = 3000;

int cmp(const void *x, const void *y) {
  return (*(int *)x - * (int *)y);
}

int mediana(int amostra[], int qt_amostra) {
  qsort(amostra, qt_amostra, sizeof(int), cmp);
  return amostra[qt_amostra/2];
}

int amostragem(int pino_sinal, int pino_leitura) {
  int amostra [QT_AMOSTRA];
  for (int i=0; i<QT_AMOSTRA; i++){
    digitalWrite(pino_sinal, HIGH);
    amostra[i] = analogRead(pino_leitura);
    delay(TEMPO_AMOSTRAGEM/QT_AMOSTRA);
    digitalWrite(pino_sinal, LOW);
  }
  return mediana(amostra, QT_AMOSTRA);    
}
  

void setup() {
  pinMode(seg_alto, OUTPUT);
  pinMode(seg_meio, OUTPUT);
  pinMode(seg_baixo, OUTPUT);
  ldr_min = amostragem(seg_baixo, ldr);
  digitalWrite(seg_meio, HIGH);
  delay(TEMPO_AMOSTRAGEM);
  digitalWrite(seg_meio, LOW);
  ldr_max = amostragem(seg_alto, ldr);
}

void loop() {
  int valor = map(analogRead(ldr), ldr_min, ldr_max, 0, 255);
  analogWrite(seg_baixo, valor);
  delay(10);
}
