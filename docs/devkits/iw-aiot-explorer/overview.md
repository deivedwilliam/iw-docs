---
id: iw-aiot-explorer-overview
title: iW-AIoT-eXplorer
sidebar_label: Visão geral
sidebar_position: 1
slug: /devkits/iw-aiot-explorer
description: "iW-AIoT-eXplorer: plataforma AIoT ESP32-S3R8 com câmera OV5640, microfones PDM, display touch e LoRa/LoRaWAN para IA na borda."
image: /img/iw-aiot-explorer/BOARD_VIEW.png
---

# iW-AIoT-eXplorer

A `iW-AIoT-eXplorer` é uma plataforma de desenvolvimento da Indústrias William voltada a exploração de **AIoT** — a convergência entre Inteligência Artificial e Internet das Coisas. É construída em torno do SoC **ESP32-S3R8** (Xtensa LX7 dual-core, até 240 MHz) com 8 MB de PSRAM integrada e acelerador vetorial para inferência de redes neurais, além de Wi-Fi e Bluetooth 5 LE.

![Placa iW-AIoT-eXplorer](/img/iw-aiot-explorer/BOARD_VIEW.png)

Ao redor do SoC, a placa reúne tudo o que um projeto de IA embarcada com áudio e visão precisa: camera `OV5640` de 5 MP, dois microfones digitais PDM em arranjo estéreo, saída de áudio Classe-D, display IPS de 3,5" com touch capacitivo, acelerômetro de 3 eixos, cartão microSD e conectividade de longo alcance **LoRa / LoRaWAN** em 915 MHz. A alimentação por PMIC com carregador de bateria permite operação autônoma em campo.

---

## Destaques

- SoC `ESP32-S3R8` (Xtensa LX7 dual-core, 8 MB PSRAM) com aceleração para TinyML / Edge AI
- camera `OV5640` de 5 MP (DVP + SCCB) para visão computacional
- 2x microfones digitais PDM `IMP34DT05` (estéreo) para keyword spotting e classificação de som
- saída de áudio Classe-D `MAX98357A` (I2S)
- display IPS 3,5" 320x240 `ST7789` com touch capacitivo
- acelerômetro de 3 eixos `LIS3DH`
- rádio `LoRa / LoRaWAN RFM95W` (SX1276) em 915 MHz
- 16 MB ou 32 MB de flash QSPI + cartão microSD
- Wi-Fi 802.11 b/g/n + Bluetooth 5 LE / Mesh
- PMIC `IP5306` com bateria Li-ion e USB-C com gravação automática

---

## Aplicações sugeridas

- visão computacional na borda e reconhecimento de imagem
- keyword spotting e comando por voz
- classificação de áudio e som
- nos sensores LoRa / LoRaWAN e monitoramento remoto
- interfaces HMI com display touch
- detecção de movimento e gestos
- protótipos de produto AIoT conectado

---

## Compatibilidade

- ESP-IDF
- Arduino IDE
- stacks LoRaWAN como RadioLib, LMIC ou a API LoRaWAN do ESP-IDF

---

## Por que esta placa chama atenção

A iW-AIoT-eXplorer junta, em uma só placa, **sensores de visão e áudio**, **display interativo** e **dois alcances de rádio** (Wi-Fi/BLE para curto alcance e LoRa para longo alcance), tudo sobre um ESP32-S3 com PSRAM farta e aceleração de IA. Isso permite rodar inferência local de imagem e som sem depender da nuvem e, ao mesmo tempo, conectar o dispositivo a redes próximas ou a quilômetros de distância.

---

## Documentação relacionada

- [Hardware](/docs/devkits/iw-aiot-explorer/hardware): características, specs elétricas, periféricos, mapa de GPIOs e esquemático interativo.
- [Downloads](/docs/devkits/iw-aiot-explorer/downloads): datasheet, imagens e projeto de hardware.
