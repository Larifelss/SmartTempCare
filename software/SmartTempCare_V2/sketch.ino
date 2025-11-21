#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// -------------------- CONFIGURAÇÕES GERAIS --------------------
const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;

const char* CLIENT_ID = "ESP32Simulado";
const char* TOPICO_TEMPERATURA = "temperatura/valor";

// -------------------- PINOS --------------------
const int sensorPin = 36;   // NTC
const int buzzerPin = 16;   // Buzzer
const int ledPin = 26;      // LED vermelho

// -------------------- CONSTANTES --------------------
const float TEMPERATURA_ALERTA_C = 37.5;

// -------------------- OBJETOS --------------------
WiFiClient espClient;
PubSubClient client(espClient);

#define LARGURA_OLED 128
#define ALTURA_OLED 64
#define RESET_OLED -1
Adafruit_SSD1306 tela(LARGURA_OLED, ALTURA_OLED, &Wire, RESET_OLED);

unsigned long lastMsg = 0;
const unsigned long DELAY_PUBLISH_MS = 2000;

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
  char valorStr[16];
  dtostrf(temp, 4, 1, valorStr);

  if (client.publish(TOPICO_TEMPERATURA, valorStr)) {
    Serial.print("Publicado valor puro: ");
    Serial.println(valorStr);
  } else {
    Serial.println("Falha na publicação MQTT");
  }
}

float lerTemperaturaNTC(int adc) {
  // Inversão do NTC do Wokwi
  return map(adc, 1000, 2500, 450, 250) / 10.0;
}

void mostrarTela(float tempC, bool febre) {
  tela.clearDisplay();
  tela.setTextSize(1);
  tela.setTextColor(SSD1306_WHITE);
  tela.setCursor(0,0);

  tela.print("Temperatura: ");
  tela.print(tempC, 1);
  tela.println(" C");

  tela.print("Status: ");
  if (febre) {
    tela.println("FEBRE!");
  } else {
    tela.println("NORMAL");
  }

  tela.display();
}

// -------------------- SETUP --------------------
void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);

  conectarWiFi();
  client.setServer(mqttServer, mqttPort);

  // Inicializa OLED
  if(!tela.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Falha ao inicializar OLED");
    while(true);
  }
  tela.clearDisplay();
  tela.display();
}

// -------------------- LOOP --------------------
void loop() {
  if (!client.connected()) {
    conectarMQTT();
  }
  client.loop();

  unsigned long now = millis();

  int valorADC = analogRead(sensorPin);
  float tempC = lerTemperaturaNTC(valorADC);

  // Verifica febre
  bool febre = tempC > TEMPERATURA_ALERTA_C;

  // LED e buzzer
  if (febre) {
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1000); // 1 kHz
    delay(200);
    noTone(buzzerPin);
  } else {
    digitalWrite(ledPin, LOW);
  }

  // Atualiza OLED
  mostrarTela(tempC, febre);

  // Serial Monitor
  Serial.print("Temp: ");
  Serial.print(tempC, 1);
  Serial.print(" °C | Status: ");
  Serial.println(febre ? "FEBRE" : "NORMAL");

  // Publica MQTT
  if (now - lastMsg > DELAY_PUBLISH_MS) {
    lastMsg = now;
    enviarTemperatura(tempC);
  }

  delay(400);
}
