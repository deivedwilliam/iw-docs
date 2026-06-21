---
id: iw-beacon-overview
title: iW-Beacon
sidebar_label: Visão geral
sidebar_position: 1
slug: /devkits/iw-beacon
description: "iW-Beacon: beacon BLE nRF52832 com sensores LIS3DH e SHTC3 e timer nano-power TPL5110 para autonomia de anos em bateria."
image: /img/iw-beacon/BOARD_VIEW.png
---

# iW-Beacon

A `iW-Beacon` é um beacon Bluetooth Low Energy da Indústrias William construído em torno do SoC **Nordic nRF52832** (ARM Cortex-M4F com BLE 5). É uma placa pensada para operar anos com uma única bateria, transmitindo identificação, telemetria de ambiente e eventos de movimento para gateways, celulares e infraestrutura BLE.

O grande diferencial é a combinação de sensores embarcados com um **timer nano-power TPL5110**, que liga a placa apenas no instante de transmitir e a desliga em seguida. Esse ciclo de trabalho extremamente curto reduz o consumo médio a níveis de microamperes, abrindo espaço para autonomia muito longa em moeda ou pilha.

![Placa iW-Beacon](/img/iw-beacon/BOARD_VIEW.png)

---

## Destaques

- SoC Nordic `nRF52832` (Cortex-M4F, BLE 5)
- Acelerômetro de 3 eixos `LIS3DH`
- Sensor de temperatura e umidade `SHTC3`
- Timer nano-power `TPL5110` para ciclo de trabalho ultrabaixo
- Chave de carga por P-MOSFET `AO3401A`
- Cristais de 32 MHz e 32.768 kHz (RTC)
- Antena com rede de casamento e suporte a NFC do nRF52832
- Depuração por SWD e console UART

---

## Aplicações sugeridas

- beacons de identificação e proximidade (estilo iBeacon)
- rastreamento e localização de ativos
- sensores de ambiente sem fio (temperatura e umidade)
- detecção de movimento, tombamento e adulteração
- nos BLE alimentados por bateria de longa duração
- logística, varejo e automação predial

---

## Por que esta placa chama atenção

A maioria dos beacons resolve só a parte de rádio. A iW-Beacon vai além ao integrar **sensoriamento de ambiente e movimento** no mesmo dispositivo e, principalmente, ao usar o `TPL5110` para cortar o consumo entre transmissões. O resultado é um beacon que não apenas anuncia presença, mas também carrega dados úteis de sensor, mantendo autonomia compatível com instalações do tipo "instale e esqueça".
