// Programa:   RFID - Desligar alarme com cartão de acesso
// Autor: Cristian Passos

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup(){
  Serial.begin(9600);
  SPI.begin();        // Inicia SPI bus
  mfrc522.PCD_Init(); // Inicia MFRC522
  Serial.println("Aproxime o seu cartão do leitor");
}

void loop() 
{
  // Look for new cards
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
  }
} 
 
void desligaAlarme()
{
  Serial.println("Alarme desligado!");
}
