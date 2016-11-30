/*

  Autor: Pitanga <pitanga@members.fsf.org> com base em sketches preexistentes

  Com código de Mark Kriegsman, July 2012
  as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
  e Fire2012 with programmable Color Palette (ver exemplos da FastLED para comentários
  específicos)

  Este sketch é uma variante do WildSnakeGS que usa o LDR para a velocidade e, ainda
  por cima, controla uma fita de LED de acordo com a "temperatura corporal" da cobrinha!

  Para controlar a fita led (WS2811, mas poderia ser qualquer outra suportada pela lib)
  usamos a biblioteca FastLED.

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

  - O circuito também atualiza a paleta de cores do efeito "Fire" da fita
  LED de acordo com a leitura do LDR.


  Sobre como funciona a parte da cobrinha: ver exemplo WildSnakeGS
*/

#define DEBUG 1

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
int delay_max = 700;
int hue_frio = 235;
int hue_quente = 0;
int brilho_baixo = 50;
int brilho_alto = 200;
//para permitir colocar luz forçada além do maximo calibrado (ex. lanterna)
float extra_factor = 0.1;

int const QT_AMOSTRA = 100;
int const TEMPO_AMOSTRAGEM = 3000;


/* 
 * FastLED parameters and variables
 */
 
#include <FastLED.h>

#define LED_PIN     5
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS    30

#define FRAMES_PER_SECOND 60

bool gReverseDirection = false;

CRGB leds[NUM_LEDS];

// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100
#define COOLING  50

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 100

CRGBPalette16 gPal;


void setup() {
  //dojo shiels setup
  if (DEBUG){
    Serial.begin(9600);
  }

  for (int i = 0; i < SEGMENTS; i++) {
    pinMode(display[i], OUTPUT);
  }

  //FastLED setup
  delay(3000); // sanity delay
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

  //calibração
  ldr_min = amostragem(ldr, seg_baixo);
  digitalWrite(seg_meio, HIGH);
  delay(2000);
  digitalWrite(seg_meio, LOW);
  ldr_max = amostragem(ldr, seg_alto);

  //inicialização
  digitalWrite(display[tail % 7], HIGH);
}

void loop() {
  int valor_ldr = leitura_ldr();
  int valor_delay = map_delay(valor_ldr);
  int valor_hue = map_hue(valor_ldr);
  int valor_brilho = map_brilho(valor_ldr);

  if (DEBUG) {
    log_valores(valor_ldr, valor_delay, valor_hue, valor_brilho);
  }

  setColorPalette(valor_hue);
  FastLED.setBrightness(valor_brilho);
  Fire2012WithPalette(); // run simulation frame, using palette colors
  FastLED.show(); // display this frame
  digitalWrite(display[head % 7], HIGH);
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
  return (*(int *)x - * (int *)y);
}

int mediana(int amostra[], int qt_amostra) {
  qsort(amostra, qt_amostra, sizeof(int), cmp);
  return amostra[qt_amostra / 2];
}

int amostragem(int pino_leitura, int pino_sinal) {
  int amostra [QT_AMOSTRA];
  for (int i = 0; i < QT_AMOSTRA; i++) {
    if (i % 6 == 0) digitalWrite(pino_sinal, HIGH);
    amostra[i] = analogRead(pino_leitura);
    delay(TEMPO_AMOSTRAGEM / QT_AMOSTRA);
    if (i % 6 == 3) digitalWrite(pino_sinal, LOW);
  }
  digitalWrite(pino_sinal, LOW);
  return mediana(amostra, QT_AMOSTRA);
}

int leitura_ldr() {
  return (int)constrain(analogRead(ldr), ldr_min , ldr_max);
}

int map_delay(int leitura_ldr) {
  return (int)map(leitura_ldr, ldr_min, ldr_max, delay_max, delay_min);
}

int map_hue(int leitura_ldr) {
  return (int)map(leitura_ldr, ldr_min, ldr_max, hue_frio, hue_quente);
}

int map_brilho(int leitura_ldr) {
  return (int)map(leitura_ldr, ldr_min, ldr_max, brilho_baixo, brilho_alto);
}


void setColorPalette(int valor_hue)
{
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy( random());

  // Four different static color palettes are provided here, plus one dynamic one.
  //
  // The three static ones are:
  //   1. the FastLED built-in HeatColors_p -- this is the default, and it looks
  //      pretty much exactly like the original Fire2012.
  //
  //  To use any of the other palettes below, just "uncomment" the corresponding code.
  //
  //   2. a gradient from black to red to yellow to white, which is
  //      visually similar to the HeatColors_p, and helps to illustrate
  //      what the 'heat colors' palette is actually doing,
  //   3. a similar gradient, but in blue colors rather than red ones,
  //      i.e. from black to blue to aqua to white, which results in
  //      an "icy blue" fire effect,
  //   4. a simplified three-step gradient, from black to red to white, just to show
  //      that these gradients need not have four components; two or
  //      three are possible, too, even if they don't look quite as nice for fire.
  //
  // The dynamic palette shows how you can change the basic 'hue' of the
  // color palette every time through the loop, producing "rainbow fire".

  // Fourth, the most sophisticated: this one sets up a new palette every
  // time through the loop, based on a hue that changes every time.
  // The palette is a gradient from black, to a dark color based on the hue,
  // to a light color based on the hue, to white.
  //
  static uint8_t hue = valor_hue;
  CRGB darkcolor  = CHSV(hue, 255, 192); // pure hue, three-quarters brightness
  CRGB lightcolor = CHSV(hue, 128, 255); // half 'whitened', full brightness
  gPal = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White);

}

// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation,
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking.
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
void Fire2012WithPalette()
{
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < NUM_LEDS; j++) {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    byte colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette( gPal, colorindex);
    int pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (NUM_LEDS - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }
}

void log_valores(int v_ldr, int v_delay, int v_hue, int v_brilho) {
  Serial.print("ldr: ");
  Serial.print(v_ldr, DEC);
  Serial.print(", delay: ");
  Serial.print(v_delay, DEC);
  Serial.print(", hue: ");
  Serial.print(v_hue, DEC);
  Serial.print(", brilho: ");
  Serial.print(v_brilho, DEC);
  Serial.println("");
}

