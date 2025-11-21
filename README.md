# README – SmartTempCare – Monitoramento de Temperatura Corporal via IoT (ESP32 + NTC + MQTT)

# Visão Geral do Projeto

O SmartTempCare é um protótipo IoT voltado ao monitoramento de temperatura corporal em tempo real, utilizando um ESP32, um sensor NTC (como substituto simulado do MAX30205) e um buzzer piezoelétrico para alertas locais.

O objetivo é simular um dispositivo capaz de:

Monitorar sinais vitais relacionados à temperatura corporal.
Detectar estados febris de forma automática.
Emitir alerta sonoro imediato.
Transmitir dados via MQTT para monitoramento remoto.
Permitir controle do buzzer via comandos MQTT.

O projeto foi desenvolvido e testado integralmente na plataforma de simulação Wokwi, alinhando-se ao ODS 3: Saúde e Bem-estar.

# Componentes Utilizados (Simulados)

| Componente                         | Função                                                                   |
| ---------------------------------- | ------------------------------------------------------------------------ |
| ESP32 DevKit V1                    | Microcontrolador com Wi-Fi integrado, responsável por processar os dados |
| NTC Temperature Sensor (analógico) | Simula o sensor corporal para medir temperatura                          |
| Buzzer piezoelétrico               | Atuador utilizado para emitir alerta sonoro em caso de febre             |
| Wokwi IoT Simulator                | Ambiente online para simulação do ESP32, sensores e atuadores            |
| Broker MQTT (Mosquitto)            | Responsável por receber as mensagens publicadas pelo ESP32               |

# Esquema da Montagem

Ligações do Sensor NTC ao ESP32

| NTC        | ESP32        |
| ---------- | ------------ |
| S (Signal) | GPIO 36 (VP) |
| + (VCC)    | 3.3V         |
| – (GND)    | GND          |

# Ligações do Buzzer

| Buzzer | ESP32   |
| ------ | ------- |
| +      | GPIO 16 |
| –      | GND     |

# Comunicação MQTT

O ESP32 publica as leituras de temperatura corporal e recebe comandos para controle remoto do buzzer.

Broker utilizado
Broker: test.mosquitto.org
Porta: 1883

# Tópicos MQTT

| Tipo       | Tópico                    | Função                                 |
| ---------- | ------------------------- | -------------------------------------- |
| Publicação | smarttempcare/temperatura | Envia leituras de temperatura corporal |
| Assinatura | smarttempcare/comandos    | Recebe comandos de controle do buzzer  |

# Comandos válidos

| Comando    | Ação             |
| ---------- | ---------------- |
| BUZZER_ON  | Liga o buzzer    |
| BUZZER_OFF | Desliga o buzzer |

# Funcionamento do Sistema

O sistema simula leituras entre 36°C e 40°C, faixa equivalente à temperatura corporal humana.

Fluxo geral do funcionamento:

1. O ESP32 lê o valor analógico do sensor NTC.
2. Converte esse valor para graus Celsius.
3. Caso a temperatura ultrapasse 37,5°C, o buzzer é ativado automaticamente.
4. A cada 5 segundos, a temperatura é publicada no tópico MQTT configurado.
5. O buzzer pode ser acionado ou desligado remotamente por comandos MQTT enviados ao tópico correspondente.

Esse fluxo simula o funcionamento de dispositivos de monitoramento corporal utilizados em cuidados domiciliares, clínicas e sistemas de telemedicina.
