# CAT14 - TASK14

### EXP
CAT14

### DATE
27/10/2026

### NAME OF EXP
BME280 Sensor Interfacing Using I2C and MQTT Using Custom Built ESP8266 API
(Verify the exact title against the questions PDF.)

### HARDWARE SETUP / PLATFORM
- Microcontroller: STM32F103C8T6 (Blue Pill)
- Sensor: BME280
- Wi-Fi module: ESP8266
- Communication: I2C, USART and MQTT
- Development platform: STM32CubeMX and Keil MDK-ARM

### TASK NO
TASK14

### Circuit Diagram
Refer to [Circuit_Diagram.png](Circuit_Diagram.png).

Configured connections:
- BME280 I2C1: PB6 (SCL), PB7 (SDA)
- ESP8266 USART1: PA9 (TX), PA10 (RX)
- Debug USART2: PA2 (TX), PA3 (RX)
- On-board LED: PC13

### Core Logic / Pseudocode / Code Base
1. Initialize the STM32 HAL, system clock and configured peripherals.
2. Initialize the BME280 sensor using the Bosch sensor driver over I2C1.
3. Configure the sensor for temperature, humidity and pressure measurements.
4. Initialize the ESP8266 using USART1 and connect to the configured Wi-Fi network.
5. Establish a TCP connection to the MQTT broker and connect the MQTT client.
6. Subscribe to the `stm32/control` topic.
7. Read BME280 measurements and publish environmental telemetry to `stm32/environment` every five seconds.
8. Process incoming MQTT messages. An `ON` command switches the PC13 LED on, while an `OFF` command switches it off.
9. Send diagnostic messages through USART2.

Implementation files:
- `Core/Src/main.c`
- `Core/Src/bme280.c`
- `Core/Src/ESPLib.c`
- `Core/Inc/bme280.h`
- `Core/Inc/ESPLib.h`

### INFERENCE
The implementation demonstrates environmental sensing with the BME280, wireless communication through the ESP8266, MQTT-based telemetry publishing and remote LED control through incoming MQTT messages.

Actual hardware observations and successful operation should be recorded after verification.
