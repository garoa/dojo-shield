/*

Autor: Pitanga <pitanga@members.fsf.org> com base em sketches preexistentes

Este sketch é uma variante do WildSnakeGS que usa o LDR para a velocidade!

A ideia é simples: a cobra é um répti, portanto um animal pecilotermo...

Na presença da luz do sol, a temperatura e o metabolismo dos pecilotermos aumentam,
portanto eles ficam mais ágeis. Na ausência da luz do sol, ficam mais frios e 
lentos, ou até dormindo!

Para o usuário final, o circuito funciona assim:

- Após o reset, o segmento inferior do display pisca por 3 segundos. Neste
  momento o usuário deve reduzir a incidência de luz no LDR para calibrar
  o nível inferior de luminosidade.

- Em seguida o segmento central acende por 1 segundo para indicar que a
  calibragem do nível inferior encerrou e o usuário deve se preparar para
  a calibragem do nível superior.

- O segmento superior do display pisca por 3 segundos para que o usuário
  aumente a incidência de luz no LDR e assim calibre o nível superior de 
  luminosidade.

- Após as duas calibragens, o circuito passa a ajustar continuamente a
  velocidade da cobrinha conforme a luz incidente no LDR.


Sobre como funciona a parte da cobrinha:
  
A two-segment snake slithers aimlessly over the display. On intersections, 
the next step is chosen at random.

Most of the logic is encoded in the `moves` array which lists possible next
steps for each current position and direction. Snake head position and 
direction is coded like pictured below, i.e. when the snake head is at the 
middle segment going right, the code is 6, going left in the same place is 
code 13.

       >:0
       <:7
      -----
^:5  |     | v:1
v:12 | >:6 | ^:8
      -----
^:4  | <:13| v:2
v:11 |     | ^:9
      -----
       <:3
       >:10

To understand this diagram, read:

> as a right arrow
< as a left arrow
v as a down arrow
^ as an up arrow

*/

// {first_choice, second_choice*}
// * -1 means there is no second_choice
int moves[][2] = {
  {1, -1},  // 0
  {13, 2},  // 1
  {3, -1},  // 2
  {4, -1},  // 3
  {6, 5},   // 4
  {0, -1},  // 5
  {2, 8},   // 6
  {12, -1}, // 7
  {7, -1},  // 8
  {13, 8},  // 9
  {9, -1},  // 10
  {10, -1}, // 11
  {6, 11},  // 12
  {5, 11}   // 13
};

/*     A
     -----
  F |     | B
    |  G  |
     -----
  E |     | C
    |  D  |
     ----- 
*/

//                A   B  C  D  E   F   G
int display[] = {8, 9, 10, 11, 12, 13, 7}; // Garoa Dojo Shield
const int SEGMENTS = 7;

int head = 1; // segment B
int tail = 0; // segment A

// variáveis para calibração
int seg_alto = 8; 
int seg_meio = 7; 
int seg_baixo = 11; 
int ldr = A1;
int ldr_min = 0;
int ldr_max = 0;
int delay_min = 10;
int delay_max = 500;
//para permitir colocar luz forçada além do maximo calibrado (ex. lanterna)
float extra_factor = 0.1;

int const QT_AMOSTRA = 100;
int const TEMPO_AMOSTRAGEM = 3000;

void setup() {
  Serial.begin(9600);
  for (int i=0; i<SEGMENTS; i++) {
    pinMode(display[i], OUTPUT);
  }

  //calibração
  ldr_min = amostragem(ldr, seg_baixo);
  digitalWrite(seg_meio, HIGH);
  delay(1000);
  digitalWrite(seg_meio, LOW);
  ldr_max = amostragem(ldr, seg_alto);

  digitalWrite(display[tail % 7], HIGH);
}

void loop() {
  digitalWrite(display[head % 7], HIGH);
  int valor_delay = leitura_ldr();
  Serial.println(valor_delay);
  delay(valor_delay);
  digitalWrite(display[tail % 7], LOW);
  tail = head;
  int* choices = moves[head];
  if (choices[1] == -1) {
    head = choices[0];  // no second choice
  } else {
    head = choices[random(2)];  // random move
  }
}

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
  


int leitura_ldr() {
  return (int)map(constrain(analogRead(ldr), ldr_min , ldr_max*(1+extra_factor)), ldr_min, ldr_max*(1+extra_factor), delay_max, delay_min);
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

