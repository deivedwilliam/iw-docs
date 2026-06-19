---
id: iw-aiot-explorer-overview
title: iW-AIoT-eXplorer
sidebar_label: Visión general
sidebar_position: 1
slug: /devkits/iw-aiot-explorer
---

# iW-AIoT-eXplorer

La `iW-AIoT-eXplorer` es una plataforma de desarrollo de Indústrias William orientada a la exploración de **AIoT** — la convergencia entre Inteligencia Artificial e Internet de las Cosas. Está construida en torno al SoC **ESP32-S3R8** (Xtensa LX7 dual-core, hasta 240 MHz) con 8 MB de PSRAM integrada y un acelerador vectorial para inferencia de redes neuronales, además de Wi-Fi y Bluetooth 5 LE.

![Placa iW-AIoT-eXplorer](/img/iw-aiot-explorer/BOARD_VIEW.png)

Alrededor del SoC, la placa reúne todo lo que necesita un proyecto de IA embebida con audio y visión: cámara `OV5640` de 5 MP, dos micrófonos digitales PDM en arreglo estéreo, salida de audio Clase-D, pantalla IPS de 3,5" con touch capacitivo, acelerómetro de 3 ejes, tarjeta microSD y conectividad de largo alcance **LoRa / LoRaWAN** en 915 MHz. La alimentación por PMIC con cargador de batería permite operación autónoma en campo.

---

## Destacados

- SoC `ESP32-S3R8` (Xtensa LX7 dual-core, 8 MB PSRAM) con aceleración para TinyML / Edge AI
- cámara `OV5640` de 5 MP (DVP + SCCB) para visión por computadora
- 2x micrófonos digitales PDM `IMP34DT05` (estéreo) para keyword spotting y clasificación de sonido
- salida de audio Clase-D `MAX98357A` (I2S)
- pantalla IPS 3,5" 320x240 `ST7789` con touch capacitivo
- acelerómetro de 3 ejes `LIS3DH`
- radio `LoRa / LoRaWAN RFM95W` (SX1276) en 915 MHz
- 16 MB o 32 MB de flash QSPI + tarjeta microSD
- Wi-Fi 802.11 b/g/n + Bluetooth 5 LE / Mesh
- PMIC `IP5306` con batería Li-ion y USB-C con grabación automática

---

## Aplicaciones sugeridas

- visión por computadora en el borde y reconocimiento de imagen
- keyword spotting y comando por voz
- clasificación de audio y sonido
- nodos sensores LoRa / LoRaWAN y monitoreo remoto
- interfaces HMI con pantalla táctil
- detección de movimiento y gestos
- prototipos de producto AIoT conectado

---

## Compatibilidad

- ESP-IDF
- Arduino IDE
- stacks LoRaWAN como RadioLib, LMIC o la API LoRaWAN de ESP-IDF

---

## Por qué llama la atención esta placa

La iW-AIoT-eXplorer reúne, en una sola placa, **sensores de visión y audio**, una **pantalla interactiva** y **dos alcances de radio** (Wi-Fi/BLE para corto alcance y LoRa para largo alcance), todo sobre un ESP32-S3 con PSRAM abundante y aceleración de IA. Esto permite ejecutar inferencia local de imagen y sonido sin depender de la nube y, al mismo tiempo, conectar el dispositivo a redes cercanas o a kilómetros de distancia.

---

## Documentación relacionada

- [Hardware](/docs/devkits/iw-aiot-explorer/hardware): características, especificaciones eléctricas, periféricos, mapa de GPIO y esquemático interactivo.
- [Descargas](/docs/devkits/iw-aiot-explorer/downloads): datasheet, imágenes y proyecto de hardware.
