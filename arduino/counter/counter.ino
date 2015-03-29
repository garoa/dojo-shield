/*
  Contador decimal
  
  Conta de 0 a 9 em decimal infinitas vezes no display de 7 segmentos.
  A velocidade da contagem varia de acordo com o ajuste no potenciometro.
  Para referencia de como manipular os ports do arduino a fim de
  performance acesse:
  http://www.arduino.cc/en/Reference/PortManipulation
  
  Este exemplo está em domínio público.
 */

static const char port_b_codes[10] =
{
  0b00111111, // 0
  0b00000110, // 1
  0b00011011, // 2
  0b00001111, // 3
  0b00100110, // 4
  0b00101101, // 5
  0b00111101, // 6
  0b00000111, // 7
  0b00111111, // 8
  0b00100111, // 9
};
static const char port_d_codes[10] =
{
  0b00000000, // 0
  0b00000000, // 1
  0b10000000, // 2
  0b10000000, // 3
  0b10000000, // 4
  0b10000000, // 5
  0b10000000, // 6
  0b00000000, // 7
  0b10000000, // 8
  0b10000000, // 9
};

void setup() {
  DDRB = 0b00111111; //Configura pinos de 8 a 13 como saida
  DDRD = 0b11000000; //configura pinos 6 e 7 como saida
}

void loop() {
  for (int i=0; i<10; i++) {
    PORTB = port_b_codes[i];
    PORTD = port_d_codes[i];
    delay(analogRead(A0));                
  }
}
