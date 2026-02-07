# EnviroSense: IoT Air Quality Monitoring System

## Project Overview
EnviroSense is a real-time air quality monitoring system built using NodeMCU (ESP8266) and Arduino. It tracks temperature, humidity, and gas levels (CO2/Smoke) to ensure a safe environment.

##  Tech Stack
- **Hardware:** NodeMCU ESP8266, DHT22 Sensor, MQ135 Gas Sensor
- **Software:** Arduino IDE, C++
- **Connectivity:** WiFi / IoT Cloud (if you used Blynk/Thingspeak)

##  Circuit Connection
- **DHT22:** VCC -> 3V3, GND -> G, Data -> D2
- **MQ135:** VCC -> VIN, GND -> G, A0 -> A0
