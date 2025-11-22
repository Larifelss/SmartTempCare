# Documentação de Interfaces, Protocolos e Módulos de Comunicação – SmartTempCare

Este documento reúne todas as informações relacionadas à comunicação do projeto SmartTempCare, incluindo protocolos utilizados, interfaces lógicas, tópicos MQTT, estrutura das mensagens e comportamento dos módulos envolvidos.

---

## 1. Arquitetura de Comunicação

O SmartTempCare utiliza comunicação **Wi-Fi + MQTT**, permitindo envio contínuo de dados do ESP32 para um broker público. A arquitetura segue o fluxo:

ESP32 → Wi‑Fi → Broker MQTT → Aplicação de Monitoramento

Além disso, o display OLED funciona como interface local e o buzzer/LED atuam como interface de alerta.

---

## 2. Interfaces do Sistema

### **2.1. Interface Local (Física)**

- **Display OLED SSD1306**: mostra a temperatura e o status (NORMAL / FEBRE).
- **LED Vermelho (GPIO 26)**: acende indicando febre.
- **Buzzer (GPIO 16)**: emite um alerta sonoro quando temperatura supera 37,5°C.

### **2.2. Interface de Comunicação com a Nuvem**

- **Wi-Fi**: utilizado para conexão à rede Wokwi-GUEST.
- **MQTT**: protocolo principal para envio de dados.

---

## 3. Protocolo MQTT

O projeto utiliza o broker público:

- **Servidor:** `test.mosquitto.org`
- **Porta:** `1883`
- **Client ID:** `ESP32Simulado`

### 3.1. Tópicos Utilizados

| Tipo       | Tópico              | Descrição                       |
| ---------- | ------------------- | ------------------------------- |
| Publicação | `temperatura/valor` | Envia a temperatura atual em °C |

### 3.2. Formato das Mensagens Publicadas

As mensagens enviadas são valores _float_ convertidos para string:

```
37.4
```

```
38.0
```

## 4. Funcionamento do Módulo de Comunicação

### **4.1. Conexão Wi‑Fi**

- Executada por `conectarWiFi()`.
- Tenta até obter `WL_CONNECTED`.
- Mostra IP no monitor serial.

### **4.2. Conexão MQTT**

- Função: `conectarMQTT()`.
- Repetição até conectar ao broker.
- Em caso de falha: aguarda 5s.

### **4.3. Publicação MQTT**

- Envio periódico a cada **2 segundos**.
- Realizado por `enviarTemperatura()`.
- Usa `client.publish()`.

### **4.4. Manutenção da Conexão**

- A função `client.loop()` é chamada continuamente.
- Garante funcionamento do cliente MQTT.

---

## 5. Estrutura das Camadas de Comunicação

| Camada     | Elemento                 | Função                            |
| ---------- | ------------------------ | --------------------------------- |
| Aplicação  | Código Arduino           | Processamento e lógica de negócio |
| Transporte | MQTT                     | Envio de telemetria               |
| Rede       | Wi‑Fi (802.11 b/g/n)     | Conectividade com broker          |
| Física     | Hardware real / simulado | Sensores e atuadores              |

---

## 6. Fluxo Completo da Comunicação MQTT

1. ESP32 conecta ao Wi‑Fi.
2. ESP32 se conecta ao broker MQTT.
3. Sensor NTC é lido.
4. Valor ADC é convertido em temperatura.
5. Temperatura é exibida no OLED.
6. Caso haja febre, LED e buzzer são acionados.
7. Temperatura enviada ao tópico `temperatura/valor` a cada 2 segundos.
