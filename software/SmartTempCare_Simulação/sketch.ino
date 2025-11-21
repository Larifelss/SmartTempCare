#include <WiFi.h>
#include <PubSubClient.h>

// -------------------- CONFIGURAÇÕES GERAIS --------------------
// Wi-Fi (Wokwi)
const char* ssid = "Wokwi-GUEST"; 
const char* password = ""; 

// Broker MQTT externo público
const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;

// Client ID e tópico
const char* CLIENT_ID = "ESP32Simulado";
const char* MQTT_TOPIC_PUBLISH = "teste/esp32";

// -------------------- PINOS --------------------
const int sensorPin = 36; // Pino analógico do NTC (VP)
const int buzzerPin = 16; // Pino digital para LED/Buzzer

// -------------------- CONSTANTES --------------------
const float TEMPERATURA_ALERTA_C = 37.5; 
const int ADC_MIN = 1000;
const int ADC_MAX = 2500;

// -------------------- OBJETOS --------------------
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
const unsigned long DELAY_PUBLISH_MS = 20000; // 20 segundos

// -------------------- FUNÇÕES --------------------
void conectarWiFi() {
  Serial.print("Conectando ao Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void conectarMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect(CLIENT_ID)) {
      Serial.println(" conectado!");
    } else {
      Serial.print(" falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5s");
      delay(5000);
    }
  }
}

void enviarTemperatura(float temp) {
  String payload = String(temp, 2);
  if(client.publish(MQTT_TOPIC_PUBLISH, payload.c_str())) {
    Serial.print("Publicado: ");
    Serial.println(payload);
  } else {
    Serial.println("Falha na publicação MQTT");
  }
}

// -------------------- SETUP --------------------
void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  conectarWiFi();
  client.setServer(mqttServer, mqttPort);
}

// -------------------- LOOP --------------------
void loop() {
  if (!client.connected()) {
    conectarMQTT();
  }
  client.loop();

  unsigned long now = millis();

  // Simula leitura do sensor
  int valorADC = analogRead(sensorPin);
  float tempC = map(valorADC, ADC_MIN, ADC_MAX, 250, 450) / 10.0;

  // Alarme: LED/Buzzer se temperatura acima do limite
  if (tempC > TEMPERATURA_ALERTA_C) {
    digitalWrite(buzzerPin, HIGH);
    delay(200);
    digitalWrite(buzzerPin, LOW);
  }

  // Monitor serial
  Serial.print("Valor ADC: ");
  Serial.print(valorADC);
  Serial.print(" | Temperatura: ");
  Serial.print(tempC);
  Serial.println(" °C");

  // Publica temperatura a cada 20 segundos
  if (now - lastMsg > DELAY_PUBLISH_MS) {
    lastMsg = now;
    enviarTemperatura(tempC);
  }

  delay(500);
}
