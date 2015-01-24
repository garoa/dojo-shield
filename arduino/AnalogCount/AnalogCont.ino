
const int timer = 300;
const int timer2 = 100;
const int analogPin = A0;
void setup(){
  Serial.begin(9600);//Monitorar pino analogico
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(analogPin, INPUT);
}
void loop(){
  int sensorValue = analogRead(analogPin);
  Serial.print("\n");
  Serial.print(sensorValue);
  if(sensorValue < 102){
    digitalWrite(8, LOW);// Apaga leds acessos
    digitalWrite(9, LOW);
    digitalWrite(7, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(9, HIGH);// Acende n˚1
    digitalWrite(10, HIGH);
}
  if(sensorValue >103){
    digitalWrite(8, LOW);// Apaga leds acessos
    digitalWrite(9, LOW);
    digitalWrite(7, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(8, HIGH);// Acende n˚2
    digitalWrite(9, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(11, HIGH);
  }
  if(sensorValue > 204){
    digitalWrite(8, LOW);// Apaga leds acessos
    digitalWrite(9, LOW);
    digitalWrite(13, LOW);
    digitalWrite(7, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(8, HIGH);// Acende n˚3
    digitalWrite(9, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
  }
  if(sensorValue > 306){
    digitalWrite(8, LOW);// Apaga leds acessos
    digitalWrite(9, LOW);
    digitalWrite(13, LOW);
    digitalWrite(7, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(13, HIGH);// Acende n˚4
    digitalWrite(7, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(10, HIGH);
  }
  if(sensorValue > 408){
    digitalWrite(8, LOW);// Apaga leds acessos
    digitalWrite(9, LOW);
    digitalWrite(13, LOW);
    digitalWrite(7, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(8, HIGH);// Acende n˚5
    digitalWrite(13, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
  }
  if(sensorValue > 510){
    digitalWrite(8, LOW);// Apaga leds acessos
    digitalWrite(9, LOW);
    digitalWrite(13, LOW);
    digitalWrite(7, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(8, HIGH);// Acende n˚6
    digitalWrite(13, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
    digitalWrite(12, HIGH);
  }
  if(sensorValue > 612){
    digitalWrite(8, LOW);// Apaga leds acessos
    digitalWrite(9, LOW);
    digitalWrite(13, LOW);
    digitalWrite(7, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(8, HIGH);// Acende n˚7
    digitalWrite(9, HIGH);
    digitalWrite(10, HIGH);
  }
  if(sensorValue > 714){
    
    digitalWrite(8, HIGH);// Acende n˚8, nao precisa apagar nenhum led
    digitalWrite(9, HIGH);
    digitalWrite(13, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);
    digitalWrite(12, HIGH);
  }
  if(sensorValue > 816){
    digitalWrite(6, LOW);// Apaga leds acessos
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(13, LOW);
    digitalWrite(7, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(13, HIGH);// Acende n˚9
    digitalWrite(7, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(10, HIGH);
  }
  if(sensorValue > 918){
    digitalWrite(8, LOW);// Apaga leds acessos
    digitalWrite(9, LOW);
    digitalWrite(13, LOW);
    digitalWrite(7, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(6, HIGH);// Acende ponto decimal
  }
}
  
