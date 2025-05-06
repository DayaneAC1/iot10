#include <SPI.h>
#include <Ethernet.h>
#include "Adafruit_LiquidCrystal.h"
#include "dht h"
const int pinoDHT11 = A2;
dht DHT; 
Adafruit_LiquidCrystal lcd(0);
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xF6};
IPAddress ip(192,168,1,11);     // coloque um IP livre
EthernetServer server(5000);
const int LED = 7;
//pinos do sensor HC-SR04
const int TRIG_PIN = 4;
const int ECHO_PIN = 3;

//velocidade do som: 343 m/s -> 0.0343 cm/μ
const float SOUND_SPEED = 0.0343;


void setup() {
  pinMode(LED, OUTPUT);
  Ethernet.begin(mac, ip);      // pode usar Ethernet.begin(mac) p/ DHCP
  server.begin();

  Serial.begin(9600);        // deve bater com o Python
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  if (!lcd.begin(16, 2)) {
    Serial.println("Não foi possível reiniciar o lcd. Por favor verifique os fios");
    //while(1);
  }
  Serial.println("O lcd foi conectado");
}

long medirDistanciaCm(){
  // 1) Limpa o TRIG
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // 2) Dispara pulso de 10μs
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // 3) Mede duração do pulso do retorno
  long duracao = pulseIn(ECHO_PIN, HIGH, 30000);
  //timeout de 30ms para não travar indefinido

  // 4)Converte para centimetros
  long distancia = (duracao * SOUND_SPEED) / 2.0;
  return distancia;
}


void loop() {
  EthernetClient client = server.available();
  if (client){
    String cmd = client.readStringUntil('\n');  //espera linha
    cmd.trim();
    if (cmd == "ligar"){
      digitalWrite(LED, HIGH);
      client.println("Ligado");
    }   
    else if (cmd == "desligar"){
      digitalWrite(LED, LOW);
      client.println("Desligado");
    }
    else if (cmd == "distancia"){
      long dist = medirDistanciaCm();
      client.println(dist);
    }
    DHT.read11(pinoDHT11); //LÊ AS INFORMAÇÕES DO SENSOR
    client.print("Umidade: "); //IMPRIME O TEXTO NA SERIAL
    client.print(DHT.humidity); //IMPRIME NA SERIAL O VALOR DE UMIDADE MEDIDO
    client.print("%"); //ESCREVE O TEXTO EM SEGUIDA
    client.print(" / Temperatura: "); //IMPRIME O TEXTO NA SERIAL
    client.print(DHT.temperature, 0); //IMPRIME NA SERIAL O VALOR DE UMIDADE MEDIDO E REMOVE A PARTE DECIMAL
    client.println("*C"); //IMPRIME O TEXTO NA SERIAL
    delay(2000); //INTERVALO DE 2 SEGUNDOS * NÃO DIMINUIR ESSE VALOR

    client.stop();
  }


}






