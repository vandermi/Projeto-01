// libraries
#include <MKRGSM.h>
#include "arduino_secrets.h"
#include <SPI.h> //RFID
#include <MFRC522.h> //RFID



int valor=0; //Inicia a variável sensor de luminosidade
#define LUM  // pino do sensor de luminosidade
#define pPin  //pino do sensor de presença
#define sensor 2; //PINO DIGITAL UTILIZADO PELO SENSOR
#define alarme 12; //PINO DIGITAL UTILIZADO PELO ALARME

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

const char PINNUMBER[]     = SECRET_PINNUMBER;
const char GPRS_APN[]      = SECRET_GPRS_APN;
const char GPRS_LOGIN[]    = SECRET_GPRS_LOGIN;
const char GPRS_PASSWORD[] = SECRET_GPRS_PASSWORD;

// INICIALIZA INSTÂNCIAS DAS CLASSES GSM,GRRS E GSMServer
GPRS gprs; 
GSM gsmAccess; 
GSMServer server(80); // PORTA PADRÃO DA REQUISIÇÃO


const unsigned long __TIMEOUT__ = 10 * 1000;

void setup() {
  
 
  // initialize serial communications and wait for port to open:
  Serial.begin(9600);
  pinMode(sensor, INPUT_PULLUP); //DEFINE O PINO COMO ENTRADA / "_PULLUP" É PARA ATIVAR O RESISTOR INTERNO
  pinMode(alarme, OUTPUT);  //DEFINE O PINO COMO SAÍDA
  digitalWrite(alarme, LOW); //ALARME INICIA DESLIGADO
  int detector_presenca = 0;
   pinMode(LUM, INPUT); //Pino que recebe o sinal e envia de luminosidade
    
  // connection state
  SPI.begin();        // Inicia SPI bus
  mfrc522.PCD_Init(); // Inicia MFRC522
  Serial.println("Aproxime o seu cartão do leitor");
  bool connected = false;

  // Start GSM shield

  // If your SIM has PIN, pass it as a parameter of begin() in quotes

  while (!connected) {

    if ((gsmAccess.begin(PINNUMBER) == GSM_READY) &&

        (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) {

      connected = true;

    } else {

      Serial.println("Not connected");

      delay(1000);

    }

  }

  Serial.println("Connected to GPRS network");

  // start server

  server.begin();

  //Get IP.

  IPAddress LocalIP = gprs.getIPAddress();

  Serial.println("Server IP address=");

  Serial.println(LocalIP);
}

void loop() {
   if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
    //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();

  if (conteudo.substring(1) == "codigo chaveiro") //UID 1 - Chaveiro
  {
    Serial.println("Chaveiro aceito !");
    Serial.println();
    Serial.println("Desligando alarme...");
    desligaAlarme();
  }
 
  if (conteudo.substring(1) == "codigo cartao") //UID 2 - Cartao
  {
    Serial.println("Cartão aceito !");
    Serial.println();
    Serial.println("Desligando alarme...");
    desligaAlarme();
    // se aceito: 

    GSMClient client = server.available();

  if (client) {

    while (client.connected()) {

      if (client.available()) {

        Serial.println("Receiving request!");

        bool sendResponse = false;

        while (int c = client.read()) {

          if (c == -1) {
            break;
          } else if (c == '\n') {
            sendResponse = true;
          }
        }

        if (sendResponse) {

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("<p>SISTEMA SAF</p>");

          client.println();

          client.println("<html>");


          client.print("Sensor de luminosidade: ");
          client.print(LUM);
          client.print(" is ");
          client.print(analogRead(LUM));

          client.print("Sensor de Presença: ");
          client.print(pPin);
          client.print(" is ");
          client.print( digitalRead(pPin););


          client.print("Sensor magnético: ");
          client.print(sensor);
          client.print(" is ");
          client.print(digitalRead(sensor););

          client.println("</html>");

          delay(1000);

          client.stop();

        }

      }

    }

  }



  }



}
