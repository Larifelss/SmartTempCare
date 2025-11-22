# Hardware Utilizado – SmartTempCare

Este documento descreve todo o hardware utilizado no projeto SmartTempCare, incluindo microcontrolador, sensores, atuadores, display e estrutura física.

---

## 1. Plataforma de Desenvolvimento

### **ESP32 DevKit V1**

- Microcontrolador de 32 bits
- Wi-Fi integrado (2.4 GHz)
- 2 núcleos
- Entradas analógicas utilizadas: **GPIO 36 (VP)**
- Saídas digitais utilizadas: **GPIO 16 (buzzer)**, **GPIO 26 (LED)**, **GPIO 21/22 (I2C)**
- Alimentação: 5V (USB) / 3.3V

---

## 2. Sensores

### **Sensor NTC (Termistor – Simulado)**

- Modelo: NTC genérico (Wokwi)
- Função: Leitura de temperatura corporal
- Tipo: Analógico
- Pinagem:
  - S → **GPIO 36 (VP)**
  - - → **3.3V**
  - – → **GND**
- Faixa simulada: ~25°C a 45°C
- Observação: no Wokwi, o comportamento do NTC é invertido, exigindo mapeamento de ADC para temperatura.

---

## 3. Atuadores

### **Buzzer Piezoelétrico**

- Tipo: Ativo (usado com função _tone()_ do ESP32)
- Pinagem:
  - - → **GPIO 16**
  - – → GND
- Função: Alerta sonoro quando temperatura > 37,5°C

### **LED Vermelho**

- Tipo: LED comum 5mm
- Pinagem:
  - - → **GPIO 26**
  - – → GND (via resistor no circuito real)
- Função: Indicação visual de febre

---

## 4. Display

### **OLED SSD1306 (I2C, 128x64)**

- Comunicação: I2C
- Endereço padrão: **0x3C**
- Pinagem no ESP32:
  - SDA → **GPIO 21**
  - SCL → **GPIO 22**
  - VCC → 3.3V
  - GND → GND
- Função: Exibir temperatura e status do paciente

---

## 5. Diagrama de Conexões

### Resumo das ligações

| Componente | Pino | ESP32   |
| ---------- | ---- | ------- |
| NTC        | S    | GPIO 36 |
| Buzzer     | +    | GPIO 16 |
| LED        | +    | GPIO 26 |
| OLED       | SDA  | GPIO 21 |
| OLED       | SCL  | GPIO 22 |
| Todos      | –    | GND     |

---

## 6. Alimentação

- Projeto simulado via USB no Wokwi
- Em hardware real:
  - Alimentação por USB 5V
  - Regulação interna do ESP32 para 3.3V
  - Opcional: Powerbank ou bateria Li-ion 3.7V + módulo TP4056

---

## 7. Observações Importantes

- No Wokwi, o NTC se comporta de forma invertida, e o mapeamento **map(ADC, 1000→2500, 45.0→25.0)** foi ajustado.
- O buzzer não toca continuamente, apenas em pulsos curtos.
- O LED permanece aceso enquanto a temperatura estiver acima do limite.
