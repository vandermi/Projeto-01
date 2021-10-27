#define sensor 2; //PINO DIGITAL UTILIZADO PELO SENSOR
#define alarme 12; //PINO DIGITAL UTILIZADO PELO ALARME
 
void setup(){
  pinMode(sensor, INPUT_PULLUP); //DEFINE O PINO COMO ENTRADA / "_PULLUP" É PARA ATIVAR O RESISTOR INTERNO
  //DO ARDUINO PARA GARANTIR QUE NÃO EXISTA FLUTUAÇÃO ENTRE 0 (LOW) E 1 (HIGH)
  pinMode(alarme, OUTPUT);  //DEFINE O PINO COMO SAÍDA
  digitalWrite(alarme, LOW); //ALARME INICIA DESLIGADO
}
 
void loop(){
 
  if (digitalRead(sensor) == HIGH){ //SE A LEITURA DO PINO FOR IGUAL A HIGH
    //ALARME TOCA, OSCILANDO O BARULHO, BAIXANDO E AUMENTANDO.
    tone(alarme, 1000);
    delay(500);
    tone(alarme, 3000);
    delay(500); 
  }
  else{ //SENÃO
      digitalWrite(alarme, LOW); //ALARME CONTINUA DESLIGADO
  }
}
