/* Dimmer.ino

Usar o LDR para aumentar ou diminuir o brilho de um LED ligado a um pino
que suporta PWM (pino 11 neste exemplo).

Este sketch demonstra uma técnica de calibração do sensor usando a mediana
dos valores lidos em dois momentos logo após o reset. 

Para o usuário final, o circuito funciona assim:

- Após o reset, o segmento inferior do display pisca por 3 segundos. Neste
  momento o usuário deve reduzir a incidência de luz no LDR para calibrar
  o nível inferior de luminosidade.

- Em seguida o segmento central acende por 3 segundos para indicar que a
  calibragem do nível inferior encerrou e o usuário deve se preparar para
  a calibragem do nível superior.

- O segmento superior do display pisca por 3 segundos para que o usuário
  aumente a incidência de luz no LDR e assim calibre o nível superior de 
  luminosidade.

- Após as duas calibragens, o circuito passa a ajustar continuamente o 
  brilho do segmento inferior do display confore a luz incidente no LDR.

Para uma descrição mais detalhada, veja comentário ao final do código.

*/

#include <stdlib.h>

int seg_alto = 8; 
int seg_meio = 7; 
int seg_baixo = 11; 
int ldr = A1;
int ldr_min = 0;
int ldr_max = 0;

int const QT_AMOSTRA = 100;
int const TEMPO_AMOSTRAGEM = 3000;

int cmp(const void *x, const void *y) {
  return (*(int *)x - *(int *)y);
}

int mediana(int amostra[], int qt_amostra) {
  qsort(amostra, qt_amostra, sizeof(int), cmp);
  return amostra[qt_amostra/2];
}

int amostragem(int pino_leitura, int pino_sinal) {
  int amostra [QT_AMOSTRA];
  for (int i=0; i<QT_AMOSTRA; i++){
    if (i % 6 == 0) digitalWrite(pino_sinal, HIGH);
    amostra[i] = analogRead(pino_leitura);
    delay(TEMPO_AMOSTRAGEM/QT_AMOSTRA);
    if (i % 6 == 3) digitalWrite(pino_sinal, LOW);
  }
  digitalWrite(pino_sinal, LOW);
  return mediana(amostra, QT_AMOSTRA);    
}
  
void setup() {
  pinMode(seg_alto, OUTPUT);
  pinMode(seg_meio, OUTPUT);
  pinMode(seg_baixo, OUTPUT);
  ldr_min = amostragem(ldr, seg_baixo);
  digitalWrite(seg_meio, HIGH);
  delay(TEMPO_AMOSTRAGEM);
  digitalWrite(seg_meio, LOW);
  ldr_max = amostragem(ldr, seg_alto);
}

void loop() {
  int valor = constrain(analogRead(ldr), ldr_min, ldr_max);
  valor = map(valor, ldr_min, ldr_max, 0, 255);
  analogWrite(seg_baixo, valor);
}


/* Como funciona

- Para calibrar o limite inferior do LDR, a função `setup` chama `amostragem`
  informando um `pino_leitura` e um `pino_sinal`. O `pino_leitura` é `A1`
  (onde está o LDR no Garoa Dojo Shield). Na primeira chamada o `pino_sinal`
  é 11 (segmento inferior do display no Garoa Dojo Shield).

- A função `amostragem` pisca por 3 segundos o `pino_sinal` (segmento 
  inferior) para informar o usuário de que está sendo feita a leitura do
  limite inferior. Neste momento o usuário deve reduzir a incidência de luz
  no LDR. Enquanto pisca `pino_sinal`, são colhidas 100 amostras
  de valores no LDR. Uma vez colhidas as amostras, a `pino_sinal` é desligado
  e é calculada a mediana (valor central do array de amostras, ordenado pela
  função `qsort` da biblioteca padrão do AVR - pesquise AVR stdlib.h).

- A função `setup` armazena o resultado da `amostragem` em `ldr_min`, e 
  acende o segmento central do display para indicar ao usuário que a 
  calabração do valor inferior terminou e vai começar a calibração do valor
  superior. Após 3 segundos, a função `amostragem` é chamada novamente, desta
  vez com o `pino_sinal` ligado ao segmento superior do display.

- O `pino_sinal` (segmento superior) pisca por 3 segundos, é feita a coleta
  de 100 amostras e devolvida a mediana.

- `setup` armazena o novo resultado em `ldr_max`.

- Durante o `loop`, os valores lidos do LDR são limitados por `constrain` à
  faixa de `ldr_min` até `ldr_max`, e convertidos por `map` a um valor
  proporcional entre 0 e 255 (mínimo e máximo do PWM). O valor convertido
  é usado para controlar o led do segmento inferior.

*/
