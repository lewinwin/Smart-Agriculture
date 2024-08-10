# IoT Garden Project

## Overview

This IoT Garden project utilizes the **Vietduino Wifi BLE ESP32** board to create an Internet of Things (IoT) system for agricultural applications. The system reads values from various sensors, including light sensor, temperature and humidity sensor, and soil moisture sensor. It then monitors and displays these values on the Blynk platform and allows remote control of devices via the Blynk interface.

### Key Features
- **Real-time Environmental Monitoring**: Collects data from sensors and displays it on Blynk.
- **IoT System**: Utilizes the Vietduino Wifi BLE ESP32 board for easy setup and interaction with devices.
- **Automatic Control**: Manages the operation of lights and pumps based on sensor readings and user commands from Blynk.

## Components

- **Vietduino Wifi BLE ESP32**: Main microcontroller board for managing sensors and actuators.
- **DHT11 Temperature and Humidity Sensor**: Measures air temperature and humidity.
- **LDR Light Sensor**: Measures light intensity.
- **Soil Moisture Sensor**: Measures soil moisture level.
- **MakerEdu Shield for Vietduino**: Provides additional connectors and support for various sensors and actuators.
- **LCD1602 I2C Display**: Displays sensor data.
- **Relay Module and Pump**: Controls the water pump for irrigation.

## Blynk Introduction

[Blynk](http://blynk.io/) is an IoT platform that allows you to create and manage connected devices easily. It provides tools and resources to build mobile and web interfaces for monitoring and controlling IoT devices remotely. Blynk is popular among hobbyists and developers for quickly prototyping and deploying IoT applications.

## Libraries Used

- **ESP32**: Core library for ESP32 microcontrollers.
- **DHT11.h**: For interfacing with the DHT11 temperature and humidity sensor.
- **MKL_LiquidCrystal_I2C.h**: For interfacing with the LCD1602 I2C display.
- **BlynkGate.h**: For connecting the ESP32 to the Blynk platform.

## Code Overview

The project code includes the following key components:

1. **Setup and Initialization**:
   - Connects to WiFi and Blynk server.
   - Initializes sensors and LCD display.

2. **Data Collection**:
   - Reads data from light, temperature, humidity, and soil moisture sensors.
   - Updates LCD display with sensor readings.

3. **Blynk Integration**:
   - Sends sensor data to Blynk for remote monitoring.
   - Receives commands from Blynk to control the water pump.

4. **Pump and LED Control**:
   - Manages the state of the water pump based on Blynk commands.
   - Controls an LED based on light sensor readings.

## Getting Started

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/lewinwin/Smart-Agriculture
