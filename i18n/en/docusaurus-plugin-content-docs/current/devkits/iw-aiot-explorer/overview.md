---
id: iw-aiot-explorer-overview
title: iW-AIoT-eXplorer
sidebar_label: Overview
sidebar_position: 1
slug: /devkits/iw-aiot-explorer
description: "iW-AIoT-eXplorer: ESP32-S3R8 AIoT platform with an OV5640 camera, PDM microphones, a touch display and LoRa/LoRaWAN for edge AI."
image: /img/iw-aiot-explorer/BOARD_VIEW.png
---

# iW-AIoT-eXplorer

The `iW-AIoT-eXplorer` is a development platform from Indústrias William aimed at exploring **AIoT** — the convergence of Artificial Intelligence and the Internet of Things. It is built around the **ESP32-S3R8** SoC (Xtensa LX7 dual-core, up to 240 MHz) with 8 MB of integrated PSRAM and a vector accelerator for neural-network inference, plus Wi-Fi and Bluetooth 5 LE.

![iW-AIoT-eXplorer board](/img/iw-aiot-explorer/BOARD_VIEW.png)

Around the SoC, the board brings together everything an embedded AI project with audio and vision needs: a 5 MP `OV5640` camera, two PDM digital microphones in a stereo arrangement, a Class-D audio output, a 3.5" IPS display with capacitive touch, a 3-axis accelerometer, a microSD card and long-range **LoRa / LoRaWAN** connectivity at 915 MHz. Power via a PMIC with a battery charger enables autonomous field operation.

---

## Highlights

- `ESP32-S3R8` SoC (Xtensa LX7 dual-core, 8 MB PSRAM) with acceleration for TinyML / Edge AI
- 5 MP `OV5640` camera (DVP + SCCB) for computer vision
- 2x PDM digital microphones `IMP34DT05` (stereo) for keyword spotting and sound classification
- Class-D `MAX98357A` audio output (I2S)
- 3.5" 320x240 IPS display `ST7789` with capacitive touch
- 3-axis `LIS3DH` accelerometer
- `LoRa / LoRaWAN RFM95W` radio (SX1276) at 915 MHz
- 16 MB or 32 MB of QSPI flash + microSD card
- Wi-Fi 802.11 b/g/n + Bluetooth 5 LE / Mesh
- `IP5306` PMIC with Li-ion battery and USB-C with automatic flashing

---

## Suggested applications

- computer vision at the edge and image recognition
- keyword spotting and voice command
- audio and sound classification
- LoRa / LoRaWAN sensor nodes and remote monitoring
- HMI interfaces with a touch display
- motion and gesture detection
- connected AIoT product prototypes

---

## Compatibility

- ESP-IDF
- Arduino IDE
- LoRaWAN stacks such as RadioLib, LMIC or the ESP-IDF LoRaWAN API

---

## Why this board stands out

The iW-AIoT-eXplorer brings together, on a single board, **vision and audio sensors**, an **interactive display** and **two radio ranges** (Wi-Fi/BLE for short range and LoRa for long range), all on top of an ESP32-S3 with abundant PSRAM and AI acceleration. This makes it possible to run local image and sound inference without depending on the cloud and, at the same time, connect the device to nearby networks or kilometers away.

---

## Related documentation

- [Hardware](/docs/devkits/iw-aiot-explorer/hardware): features, electrical specs, peripherals, GPIO map and interactive schematic.
- [Downloads](/docs/devkits/iw-aiot-explorer/downloads): datasheet, images and hardware project.
