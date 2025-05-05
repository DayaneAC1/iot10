#include <SPI.h>
#include <Ethernet.h>
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x04};
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
    }   
    else if (cmd == "desligar"){
      digitalWrite(LED, LOW);
    }
    else if (cmd == "ultra"){
      long dist = medirDistanciaCm();
      Serial.println(dist);
    }

    client.println("OK\n");      //simples ACK
    client.stop();
  }


}




