# README – SmartTempCare – Monitoramento de Temperatura Corporal via IoT (ESP32 + NTC + OLED + LED + MQTT)

## Visão Geral do Projeto
O SmartTempCare é um sistema IoT desenvolvido para monitorar temperatura corporal em tempo real, utilizando um ESP32, sensor NTC, display OLED, LED de alerta e buzzer. O dispositivo detecta febre, exibe informações no display e envia os dados para a nuvem via MQTT.

Este projeto foi totalmente simulado no Wokwi e utiliza o broker público **test.mosquitto.org**.

---

## Componentes Utilizados
- **ESP32 DevKit V1** – Microcontrolador principal
- **Sensor NTC (simulado)** – Leitura de temperatura
- **Display OLED SSD1306 (I2C)** – Exibição de temperatura e status
- **LED Vermelho (GPIO 26)** – Indicação visual de febre
- **Buzzer (GPIO 16)** – Alerta sonoro
- **Broker MQTT (test.mosquitto.org)** – Comunicação de dados
- **Simulador Wokwi** – Ambiente de desenvolvimento

---

## Ligações dos Componentes
### Sensor NTC
| NTC | ESP32 |
|-----|--------|
| S   | GPIO 36 |
| +   | 3.3V |
| –   | GND |

### Buzzer
| Buzzer | ESP32 |
|--------|--------|
| +      | GPIO 16 |
| –      | GND |

### LED de Alerta
| LED | ESP32 |
|-----|--------|
| +   | GPIO 26 |
| –   | GND |

### Display OLED (SSD1306 – I2C)
| OLED | ESP32 |
|------|--------|
| SDA  | GPIO 21 |
| SCL  | GPIO 22 |
| VCC  | 3.3V |
| GND  | GND |

---

## Funcionamento do Sistema
1. Conecta ao Wi-Fi **Wokwi-GUEST**.
2. Inicializa o display OLED.
3. Conecta ao broker MQTT.
4. Lê continuamente o valor analógico do sensor NTC.
5. Converte o ADC em temperatura (°C) utilizando um mapa calibrado.
6. Verifica febre (temperatura > **37,5°C**):
   - Acende o LED vermelho.
   - Emite um bip rápido usando o buzzer.
7. Atualiza o display OLED com a temperatura atual e status (normal/febre).
8. Envia a temperatura via MQTT a cada **2 segundos**.

---

## Tópico MQTT Utilizado
| Tipo | Tópico | Função |
|------|--------|--------|
| Publicação | `temperatura/valor` | Envia temperatura em °C |

---

## Principais Constantes do Código
- `TEMPERATURA_ALERTA_C = 37.5`
- `sensorPin = 36`
- `buzzerPin = 16`
- `ledPin = 26`
- `DELAY_PUBLISH_MS = 2000`
- Servidor MQTT: **test.mosquitto.org**

---

## Lógica Geral
- Ler temperatura
- Atualizar display OLED
- Acionar alerta (LED + buzzer) quando necessário
- Publicar temperatura via MQTT
- Garantir reconexão automática ao Wi-Fi e MQTT

---

## Objetivo do Projeto
Simular um termômetro inteligente com alerta local e envio remoto de dados, aplicável em ambientes clínicos, domésticos ou acadêmicos. O sistema serve como base para projetos IoT de saúde, monitoramento contínuo e telemedicina.

