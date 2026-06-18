---
id: iw-beacon-overview
title: iW-Beacon
sidebar_label: Visao geral
sidebar_position: 1
slug: /devkits/iw-beacon
---

# iW-Beacon

A `iW-Beacon` e um beacon Bluetooth Low Energy da Industrias William construido em torno do SoC **Nordic nRF52832** (ARM Cortex-M4F com BLE 5). E uma placa pensada para operar anos com uma unica bateria, transmitindo identificacao, telemetria de ambiente e eventos de movimento para gateways, celulares e infraestrutura BLE.

O grande diferencial e a combinacao de sensores embarcados com um **timer nano-power TPL5110**, que liga a placa apenas no instante de transmitir e a desliga em seguida. Esse ciclo de trabalho extremamente curto reduz o consumo medio a niveis de microamperes, abrindo espaco para autonomia muito longa em moeda ou pilha.

![Placa iW-Beacon](/img/iw-beacon/BOARD_VIEW.png)

---

## Destaques

- SoC Nordic `nRF52832` (Cortex-M4F, BLE 5)
- Acelerometro de 3 eixos `LIS3DH`
- Sensor de temperatura e umidade `SHTC3`
- Timer nano-power `TPL5110` para ciclo de trabalho ultrabaixo
- Chave de carga por P-MOSFET `AO3401A`
- Cristais de 32 MHz e 32.768 kHz (RTC)
- Antena com rede de casamento e suporte a NFC do nRF52832
- Depuracao por SWD e console UART

---

## Aplicacoes sugeridas

- beacons de identificacao e proximidade (estilo iBeacon)
- rastreamento e localizacao de ativos
- sensores de ambiente sem fio (temperatura e umidade)
- deteccao de movimento, tombamento e adulteracao
- nos BLE alimentados por bateria de longa duracao
- logistica, varejo e automacao predial

---

## Por que esta placa chama atencao

A maioria dos beacons resolve so a parte de radio. A iW-Beacon vai alem ao integrar **sensoriamento de ambiente e movimento** no mesmo dispositivo e, principalmente, ao usar o `TPL5110` para cortar o consumo entre transmissoes. O resultado e um beacon que nao apenas anuncia presenca, mas tambem carrega dados uteis de sensor, mantendo autonomia compativel com instalacoes do tipo "instale e esqueca".
