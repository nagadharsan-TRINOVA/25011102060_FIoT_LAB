# IoT Lab – Embedded Systems Experiments (8051 and STM32)

Lab experiments for **25CS2741 Fundamentals of Internet of Things + Lab** (B.Tech CSE (IoT), Semester 3, Shiv Nadar University Chennai) – Register No. **25011102060**.

The experiments start with bare-metal register programming on the 8051 (GPIO, keypad, timers, UART, LCD), move to STM32 microcontrollers with STM32CubeMX and the HAL library, and finish with networked sensing: an STM32 reads a BME280 sensor and serves or publishes the data over Wi-Fi through an ESP8266, using HTTP and MQTT, with Node-RED dashboards.

Every task folder contains its own **README.md** with the experiment details, hardware used, circuit diagram, core logic, pseudocode, code base and inference, together with the project files.

## Experiments

### 8051 (AT89S52) – Bare-Metal Embedded C, Keil µVision

| # | Experiment | Key concepts | Project |
|---|---|---|---|
| [Cat 2](8051/CAT2/) | Hardware bring-up and bare-metal LED blink | Board identification, SFR bit access (`sbit`), software delay, ~1 Hz blink | [`TASK_A`](8051/CAT2/TASK_A_8051DevBoardBringUp/), [`TASK_B`](8051/CAT2/TASK_B_ledBlink_SFR_based%20IO%20access/) |
| [Cat 3](8051/CAT3/) | Button-controlled four-mode LED pattern generator | GPIO input (P2.6), debouncing, state machine, running / binary / alternating / flash modes | [`projectFiles`](8051/CAT3/projectFiles/) |
| [Cat 4](8051/CAT4/) | HMI with matrix keypad, LEDs and seven-segment display | Keypad scanning, seven-segment codes, LED patterns, edge detection | [`projectFiles`](8051/CAT4/projectFiles/) |
| [Cat 5](8051/CAT5/) | Hardware timer and interrupt-based timing | Timer 0 Mode 1 preload, interrupt-driven time base, software PWM, stepper sequencing | [`TASK_A`](8051/CAT5/TASK_A%20configuringTimerAndInterrupt/), [`TASK_B`](8051/CAT5/TASK_B%20pwmPulseGeneration/), [`TASK_C`](8051/CAT5/TASK_C%20stepperMotor/) |
| [Cat 6](8051/CAT6/) | UART driver and serial debugging | SCON/TMOD, Timer 1 baud-rate generation (9600-8-N-1), TI/RI flags, echo | [`TASK_A`](8051/CAT6/TASK_A%20uartInitializationAndTransmission/), [`TASK_B`](8051/CAT6/TASK_B%20uartEcho/) |
| [Cat 7](8051/CAT7/) | 16×2 LCD driver, dynamic display and keypad control | HD44780 4-bit interface, DDRAM addressing, cursor positioning, keypad-driven movement | [`TASK_A`](8051/CAT7/TASK_A%20LCD%20Init%20And%20Char%20Display/), [`TASK_B`](8051/CAT7/TASK_B%20Cursor%20Positioning%20and%20Dynamic%20Value%20Update/), [`TASK_C`](8051/CAT7/TASK_C%204x4%20Keypad-Based%20Character%20Movement/) |

Cat 5 `TASK_B` holds five variants of the software PWM fade (`TASK_B.1` to `TASK_B.5`).

### STM32 – STM32CubeMX + HAL, Keil MDK-ARM

| # | Experiment | Board | Key concepts | Project |
|---|---|---|---|---|
| [Cat 8](STM32/CAT8/) | GPIO bring-up and switch-controlled LED | Blue Pill (STM32F103C8) | GPIO input/output, pull-ups, non-blocking debounce, `HAL_GetTick()`, short vs long press | [`TASK8A`](STM32/CAT8/TASK8A/), [`TASK8B`](STM32/CAT8/TASK8B/), [`TASK8C`](STM32/CAT8/TASK8C/) |
| [Cat 9](STM32/CAT9/) | UART command-controlled LED | Blue Pill (STM32F103C8) | USART with HAL, timer-driven transmit, interrupt receive, line-based command parsing | [`TASK9A`](STM32/CAT9/TASK9A/), [`TASK9B`](STM32/CAT9/TASK9B/) |
| [Cat 10](STM32/CAT10/) | STM32–Wemos (ESP8266) Wi-Fi modem interface using AT commands | Blue Pill (STM32F103C8) | Two UARTs, interrupt ring buffer, ESP8266 AT commands, station mode, joining Wi-Fi, IP with `AT+CIFSR` | [`TASK10A`](STM32/CAT10/TASK10A/), [`TASK10B`](STM32/CAT10/TASK10B/), [`TASK10C`](STM32/CAT10/TASK10C/) |
| [Cat 11](STM32/CAT11/) | HTTP server on the STM32–Wemos Wi-Fi modem | Blue Pill (STM32F103C8) | TCP server (`AT+CIPMUX`, `AT+CIPSERVER`), `+IPD` requests, static HTTP response, two-step `AT+CIPSEND`, Node-RED HTTP client | [`TASK11A`](STM32/CAT11/TASK11A/), [`TASK11B`](STM32/CAT11/TASK11B/), [`TASK11C`](STM32/CAT11/TASK11C/) |
| [Cat 12](STM32/CAT12/) | Cortex-M4 GPIO and user input | NUCLEO-F401 (STM32F401RB) | F4 clock tree, on-board LED LD2 (PA5) and button B1 (PC13), debounce, short/long press | [`TASK12_A`](STM32/CAT12/TASK12_A/), [`TASK12_B`](STM32/CAT12/TASK12_B/), [`TASK12_C`](STM32/CAT12/TASK12_C/) |
| [Cat 13](STM32/CAT13/) | BME280 over I²C with the Bosch API, served over HTTP | NUCLEO-F401 (STM32F401RB) | I²C1, Bosch Sensor API, ESP8266 HTTP server, auto-refreshing web page, UART debug | [`TASK13`](STM32/CAT13/TASK13/) |
| [Cat 14](STM32/CAT14/) | MQTT publishing of BME280 data with a custom ESP8266 library | Blue Pill (STM32F103C8) | `ESPLib` MQTT connect/publish/subscribe, telemetry topic, remote LED control | [`TASK14`](STM32/CAT14/TASK14/) |
| [Cat 15](STM32/CAT15/) | IoT project: AgroSense – agricultural microclimate monitoring and advisory | NUCLEO-F401 (STM32F401RB) | BME280 sensing, ESP8266 HTTP, Node-RED Dashboard 2, OpenWeather reference data | [`TASK15`](STM32/CAT15/TASK15/) |

## Hardware

| Hardware | Used in | Notes |
|---|---|---|
| AT89S52 development board | Cat 2 – Cat 7 | 11.0592 MHz crystal; on-board LED on P1.0, 4×4 keypad, seven-segment display, 16×2 LCD; programmed over ISP with a USBasp |
| ULN2003 driver + unipolar stepper motor | Cat 5 (`TASK_C`) | Driven from P1.0 – P1.3 |
| STM32 "Blue Pill" (STM32F103C8) | Cat 8 – Cat 11, Cat 14 | 8 MHz crystal, 72 MHz system clock; on-board LED on PC13 (active-low); programmed with an ST-Link over SWD |
| NUCLEO-F401 (STM32F401RB) | Cat 12, Cat 13, Cat 15 | On-board ST-LINK; LD2 on PA5, B1 on PC13 |
| Wemos D1 Mini (ESP8266) | Cat 10, Cat 11, Cat 13 – Cat 15 | Wi-Fi modem running Espressif AT-command firmware |
| Bosch BME280 module | Cat 13 – Cat 15 | Temperature, pressure and humidity sensor on I²C (address 0x76) |
| USB-to-TTL serial converter | Cat 6, Cat 9, Cat 10 | PC terminal (PuTTY) |

## Tools

| Tool | Used for |
|---|---|
| **Keil µVision** with the C51 compiler | 8051 projects (`.uvproj`) |
| **STM32CubeMX** | STM32 pin, clock and peripheral configuration (`.ioc`) and code generation |
| **Keil MDK-ARM (µVision 5)** | STM32 projects (`MDK-ARM/*.uvprojx`) |
| STM32Cube HAL | STM32 peripheral drivers (in each project's `Drivers/` folder) |
| Bosch Sensortec BME280 Sensor API | BME280 driver (Cat 13 – Cat 15) |
| Node-RED (+ Dashboard 2) | HTTP client flow (Cat 11) and the AgroSense dashboard (Cat 15) |
| OpenWeather API | Outside weather reference in the Cat 15 dashboard |
| PuTTY | Serial terminal |

## Repository Layout

```
8051/CATn/
├── README.md (some experiments)     experiment overview and circuit
└── TASK_x.../ or projectFiles/      one folder per task
    ├── README.md                    task details: hardware, circuit, logic, pseudocode, code, inference
    └── *.c, *.uvproj, STARTUP.A51, Objects/*.hex

STM32/CATn/
└── TASKnX/                          one folder per task
    ├── README.md                    task details: hardware, circuit, logic, pseudocode, code, inference
    ├── *.ioc, .mxproject            STM32CubeMX configuration
    ├── Core/, Drivers/              application source and HAL drivers
    └── MDK-ARM/                     *.uvprojx and startup_*.s
```

Keil's temporary files (`.obj`, `.lst`, `.map`, `*.uvopt` and similar) are not committed (see [.gitignore](.gitignore)). The 8051 projects keep their `.hex` file in `Objects/` so they can be flashed without rebuilding.

## Building

**8051 (Keil µVision):** open the task's `.uvproj` file and choose **Project → Build Target (F7)**. The HEX file is written to the project's `Objects/` folder. Flash it to the AT89S52 with an ISP programmer.

**STM32 (Keil MDK-ARM):** open the task's `MDK-ARM/<project>.uvprojx` in Keil µVision 5 and choose **Project → Build Target (F7)**, then **Flash → Download** through the ST-Link.

For the Wi-Fi experiments (Cat 10, Cat 11, Cat 13 – Cat 15), set your own Wi-Fi network name and password in `main.c` first – the repository contains only the placeholders `YOUR_WIFI_SSID` and `YOUR_WIFI_PASSWORD`.