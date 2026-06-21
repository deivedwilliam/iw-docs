---
id: iw-beacon-overview
title: iW-Beacon
sidebar_label: Overview
sidebar_position: 1
slug: /devkits/iw-beacon
description: "iW-Beacon: nRF52832 BLE beacon with LIS3DH and SHTC3 sensors and a TPL5110 nano-power timer for years of battery life."
image: /img/iw-beacon/BOARD_VIEW.png
---

# iW-Beacon

The `iW-Beacon` is a Bluetooth Low Energy beacon from Indústrias William built around the **Nordic nRF52832** SoC (ARM Cortex-M4F with BLE 5). It is a board designed to run for years on a single battery, transmitting identification, environmental telemetry and motion events to gateways, phones and BLE infrastructure.

Its main differentiator is the combination of on-board sensors with a **nano-power TPL5110 timer**, which powers the board on only at the moment of transmission and turns it off right after. This extremely short duty cycle reduces the average consumption to microampere levels, opening the door to very long battery life on a coin cell or battery.

![iW-Beacon board](/img/iw-beacon/BOARD_VIEW.png)

---

## Highlights

- Nordic `nRF52832` SoC (Cortex-M4F, BLE 5)
- 3-axis `LIS3DH` accelerometer
- `SHTC3` temperature and humidity sensor
- nano-power `TPL5110` timer for an ultra-low duty cycle
- `AO3401A` P-MOSFET load switch
- 32 MHz and 32.768 kHz crystals (RTC)
- Antenna with a matching network and nRF52832 NFC support
- SWD debugging and a UART console

---

## Suggested applications

- identification and proximity beacons (iBeacon style)
- asset tracking and localization
- wireless environmental sensors (temperature and humidity)
- motion, tilt and tamper detection
- battery-powered BLE nodes with long life
- logistics, retail and building automation

---

## Why this board stands out

Most beacons only solve the radio part. The iW-Beacon goes further by integrating **environmental and motion sensing** into the same device and, above all, by using the `TPL5110` to cut consumption between transmissions. The result is a beacon that not only announces presence but also carries useful sensor data, keeping a battery life compatible with "install and forget" deployments.
