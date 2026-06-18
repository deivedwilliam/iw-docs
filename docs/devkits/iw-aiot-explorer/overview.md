---
id: iw-aiot-explorer-overview
title: iW-AIoT-eXplorer
sidebar_label: Visao geral
sidebar_position: 1
slug: /devkits/iw-aiot-explorer
---

# iW-AIoT-eXplorer

A `iW-AIoT-eXplorer` e uma plataforma de desenvolvimento da Industrias William voltada a exploracao de **AIoT** — a convergencia entre Inteligencia Artificial e Internet das Coisas. E construida em torno do SoC **ESP32-S3R8** (Xtensa LX7 dual-core, ate 240 MHz) com 8 MB de PSRAM integrada e acelerador vetorial para inferencia de redes neurais, alem de Wi-Fi e Bluetooth 5 LE.

![Placa iW-AIoT-eXplorer](/img/iw-aiot-explorer/BOARD_VIEW.png)

Ao redor do SoC, a placa reune tudo o que um projeto de IA embarcada com audio e visao precisa: camera `OV5640` de 5 MP, dois microfones digitais PDM em arranjo estereo, saida de audio Classe-D, display IPS de 3,5" com touch capacitivo, acelerometro de 3 eixos, cartao microSD e conectividade de longo alcance **LoRa / LoRaWAN** em 915 MHz. A alimentacao por PMIC com carregador de bateria permite operacao autonoma em campo.

---

## Destaques

- SoC `ESP32-S3R8` (Xtensa LX7 dual-core, 8 MB PSRAM) com aceleracao para TinyML / Edge AI
- camera `OV5640` de 5 MP (DVP + SCCB) para visao computacional
- 2x microfones digitais PDM `IMP34DT05` (estereo) para keyword spotting e classificacao de som
- saida de audio Classe-D `MAX98357A` (I2S)
- display IPS 3,5" 320x240 `ST7789` com touch capacitivo
- acelerometro de 3 eixos `LIS3DH`
- radio `LoRa / LoRaWAN RFM95W` (SX1276) em 915 MHz
- 16 MB ou 32 MB de flash QSPI + cartao microSD
- Wi-Fi 802.11 b/g/n + Bluetooth 5 LE / Mesh
- PMIC `IP5306` com bateria Li-ion e USB-C com gravacao automatica

---

## Aplicacoes sugeridas

- visao computacional na borda e reconhecimento de imagem
- keyword spotting e comando por voz
- classificacao de audio e som
- nos sensores LoRa / LoRaWAN e monitoramento remoto
- interfaces HMI com display touch
- deteccao de movimento e gestos
- prototipos de produto AIoT conectado

---

## Compatibilidade

- ESP-IDF
- Arduino IDE
- stacks LoRaWAN como RadioLib, LMIC ou a API LoRaWAN do ESP-IDF

---

## Por que esta placa chama atencao

A iW-AIoT-eXplorer junta, em uma so placa, **sensores de visao e audio**, **display interativo** e **dois alcances de radio** (Wi-Fi/BLE para curto alcance e LoRa para longo alcance), tudo sobre um ESP32-S3 com PSRAM farta e aceleracao de IA. Isso permite rodar inferencia local de imagem e som sem depender da nuvem e, ao mesmo tempo, conectar o dispositivo a redes proximas ou a quilometros de distancia.

---

## Documentacao relacionada

- [Hardware](/docs/devkits/iw-aiot-explorer/hardware): caracteristicas, specs eletricas, perifericos, mapa de GPIOs e esquematico interativo.
- [Downloads](/docs/devkits/iw-aiot-explorer/downloads): datasheet, imagens e projeto de hardware.
