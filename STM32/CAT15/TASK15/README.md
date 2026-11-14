# CAT15 — TASK15

## EXP
CAT15

## DATE
To be filled with the actual experiment/project date.

## NAME OF EXP
AgroSense – IoT-Based Agricultural Microclimate Monitoring and Advisory System

## HARDWARE SETUP / PLATFORM
- Microcontroller: STM32F401RE
- Environmental sensor: BME280
- Wi-Fi module: Wemos ESP8266
- Sensor interface: I²C1
- STM32-to-ESP8266 communication: USART1 using ESP8266 AT commands
- Debug interface: USART2
- Firmware development/configuration: STM32CubeMX and Keil MDK-ARM
- Data access: HTTP webpage served by the ESP8266
- Dashboard and processing: Node-RED Dashboard 2 (`@flowfuse/node-red-dashboard`)
- External weather reference: OpenWeather API (configured for Chennai, India)

## TASK NO
TASK15

## Circuit Diagram
Add or link the final hardware wiring diagram for the STM32F401RE, BME280 and Wemos ESP8266 here.

The supplied image is the running AgroSense dashboard, not a circuit schematic:
`Evidence/AgroSense_Dashboard.png`

## Core Logic / Pseudocode / Code Base

### 1. Local sensor acquisition and HTTP serving
1. STM32F401RE initializes GPIO, I²C1, USART1 and USART2.
2. The BME280 driver communicates with the sensor over I²C1 and reads temperature, pressure and relative humidity.
3. STM32 communicates with the Wemos ESP8266 over USART1 using AT commands.
4. ESP8266 joins the configured Wi-Fi network and starts an HTTP server on port 80.
5. When a browser/Node-RED HTTP GET request arrives, STM32 reads the BME280 and returns an HTML page containing the current sensor values through the ESP8266.

### 2. Node-RED ingestion and external weather data
1. The Node-RED flow periodically requests the ESP8266-hosted sensor webpage.
2. A function node extracts temperature, pressure and humidity from the returned HTML.
3. Another branch requests current weather data from OpenWeather for Chennai, India.
4. The API response is parsed to obtain temperature, pressure and humidity.
5. The latest local and API readings are retained in Node-RED flow context and checked for validity and freshness.

### 3. Comparison, assessment and recommendations
1. When both sources are available, the flow compares the BME280 readings with the city-level OpenWeather readings.
2. The analysis combines local and API readings using a 65% local-sensor and 35% OpenWeather weighting.
3. It calculates differences and a data-agreement indicator between the two sources.
4. Rule-based indicators and advisory messages are generated from the combined temperature and humidity conditions.
5. The Node-RED dashboard displays local readings, OpenWeather reference readings, an environmental readiness score, indicators, source consistency, warnings and smart field advisories.

### Main project files
- `Firmware/Core/Src/main.c` — STM32 firmware project entry point
- `Firmware/Core/Src/bme280.c` — BME280 sensor driver
- `Firmware/Core/Inc/bme280.h` and `bme280_defs.h` — BME280 driver headers
- `Firmware/EX_15.ioc` — STM32CubeMX configuration
- `Firmware/.mxproject` — STM32CubeMX project metadata
- `Firmware/MDK-ARM/EX_15.uvprojx` — Keil MDK project
- `NodeRED/flows.json` — Node-RED flow export
- `Evidence/AgroSense_Dashboard.png` — supplied dashboard demonstration image

The firmware archive also contains alternative/working source files (`overall.c`, `overall_corrected.c`, `pakka.c`, `pakka_sensor_to_browser_dynamic.c`, and `sensor_alone_pakka.c`). They are preserved as supplied; `main.c` remains the project file's source entry point.

## INFERENCE
The project demonstrates an end-to-end IoT microclimate monitoring and advisory pipeline. The BME280 provides hyperlocal environmental measurements, while OpenWeather provides a city-level reference. Node-RED collects both sources, compares their readings, evaluates environmental conditions using rule-based logic, and presents the result through the AgroSense dashboard with field advisories.

The supplied dashboard screenshot demonstrates a displayed readiness score of 65/100 with the label “Moderate conditions”. The uploaded Node-RED flow's current category thresholds classify scores below 91 as “Unfavorable”; therefore, the screenshot and exported flow appear to represent different dashboard/flow versions. This discrepancy is documented and has not been silently changed.

The exported Node-RED inject nodes are labelled “Local BME280: every 15 s” and “OpenWeather: every 10 min”, but their exported `repeat` values are both `1`. Confirm the intended schedule in Node-RED before relying on those labels.

The Node-RED flow contains an embedded OpenWeather API key, and the firmware contains Wi-Fi configuration. Replace or externalize real credentials before sharing the project publicly. Add the actual circuit schematic and fill in the experiment date when available. The readiness index is a rule-based environmental indicator, not a calibrated crop-yield, irrigation, or disease prediction.
