---
id: iw-digitalamplifier-hardware
title: Hardware
sidebar_label: Hardware
sidebar_position: 2
slug: /modules/iw-digitalamplifier/hardware
---

import BoardViewer from '@site/src/components/BoardViewer';

# Hardware

Esta seção detalha os aspectos elétricos e as interfaces da `iW-DigitalAmplifier` (Rev. RV01), a partir do datasheet do produto.

---

## Visão geral da placa

A iW-DigitalAmplifier é uma breakout board de áudio digital construída em torno do `TAS5805M` (Texas Instruments), um amplificador classe D estéreo de entrada digital com processador de áudio integrado. Ela expõe a alimentação de potência por borne, os sinais de áudio e controle em um conector de 10 vias e as saídas de alto-falante por bornes dedicados, já com filtros LC integrados.

![Placa iW-DigitalAmplifier](/img/iw-digitalamplifier/BOARD_VIEW.png)

### Modelo 3D

Arraste para girar, role para dar zoom.

<BoardViewer src="/models/iw-digital-amplifier-3d.glb" caption="Modelo 3D da iW-DigitalAmplifier (preliminar)" />

---

## Características principais

- amplificador `TAS5805M`, classe D estéreo closed-loop, entrada digital
- potência de 2 x 23 W (8 ohms, 21 V) estéreo, até 45 W em modo mono (4 ohms)
- entrada de áudio I2S / TDM (LRCLK, SCLK, SDIN, SDOUT)
- controle por I2C (SCL/SDA), endereço selecionável por ADR/FAULT
- DSP integrado 96 kHz: SRC, 15 biquads por canal, DRC de 3 bandas e AGL
- alimentação PVDD de 4,5 V a 26,4 V por borne (screw terminal)
- filtro de saída LC integrado (10 uH + 0,68 uF por linha)
- saídas estéreo em ponte (BTL) por bornes OUT_A / OUT_B
- chave de 3,3 V por P-MOSFET `AO3401A` e pino de proteção FAULT
- 2 furos de montagem M3

---

## Especificações elétricas

| Parâmetro | Mín. | Típ. | Máx. | Unidade / Observação |
| --- | --- | --- | --- | --- |
| Tensão de alimentação (PVDD) | 4,5 | 12 – 21 | 26,4 | V — borne J4 |
| Tensão lógica do sistema | — | 3,3 | — | V — trilho interno |
| Potência de saída estéreo (2.0) | — | 2 x 23 | — | W — 8 ohms, 21 V, THD+N = 1% |
| Potência de saída mono | — | 45 | — | W — 4 ohms, 21 V, THD+N = 1% |
| THD+N | — | ≤ 0,03 | — | % — 1 W, 1 kHz, PVDD = 12 V |
| SNR (ponderado A) | 107 | — | — | dB |
| Corrente quiescente | — | 16,5 | — | mA — PVDD = 13,5 V |
| Taxa de amostragem do DSP | — | 96 | — | kHz |
| Frequência de chaveamento PWM | — | 384 / 768 | — | kHz — configurável |
| Temperatura de operação | -25 | — | +85 | °C |

> Valores de potência, THD+N e SNR conforme o datasheet do TAS5805M (Texas Instruments). A potência real depende da tensão PVDD aplicada, da impedância do alto-falante e da dissipação térmica do conjunto.

---

## Gestão de energia

A placa tem dois domínios de alimentação. A potência (PVDD) entra pelo borne `J4`, passa por um ferrite bead de 600 ohms (FB1) e por um banco de capacitores que alimenta os estágios de saída classe D. A lógica de 3,3 V alimenta as seções digital e analógica do TAS5805M (DVDD, AVDD, VR_DIG), com chaveamento por P-MOSFET `AO3401A`.

| Recurso | Descrição |
| --- | --- |
| Entrada de potência | PVDD 4,5-26,4 V no borne J4 (screw terminal 2 vias) |
| Filtragem PVDD | C2/C3 390 uF + C1/C4 22 uF + C5/C6 100 nF; ferrite FB1 600 ohms |
| Trilho de 3,3 V | chave P-MOSFET AO3401A (Q1) com C19 10 uF / C20 100 nF |
| Domínios do CI | DVDD, AVDD e VR_DIG com decoupling C9/C10 1 uF |
| Controle / proteção | pino PDN (power-down) e ADR/FAULT com pull-ups |

---

## Interfaces e estágio de saída

### Entrada de áudio digital — I2S

Interface serial I2S / TDM para o fluxo de áudio: `LRCLK` (clock de palavra), `SCLK` (clock de bit), `SDIN` (dados de entrada) e `SDOUT` (loopback/TDM). Compatível com microcontroladores, DACs e SoCs com saída I2S.

### Controle — I2C

O TAS5805M e configurado por I2C (`SCL`/`SDA`, com pull-ups de 10 k). O endereço do dispositivo é definido pelo pino `ADR/FAULT`, que também sinaliza falhas (sobrecorrente, sobretemperatura, clipping). O pino `PDN` controla o modo power-down.

### Estágio de saída e filtro LC

Cada canal aciona uma saída em ponte (BTL): `OUT_A` e `OUT_B`. Antes dos bornes, cada linha passa por um filtro LC (indutor de 10 uH + capacitor de 0,68 uF), reduzindo EMI e entregando áudio limpo aos bornes de alto-falante J1 e J2.

---

## Conector de áudio e controle (J3, 1x10)

O conector `J3` reúne os sinais de áudio digital (I2S), o barramento de controle (I2C) e os sinais de gerenciamento do TAS5805M, além do terra de referência.

| Pino | Sinal | Tipo | Função | Observações |
| --- | --- | --- | --- | --- |
| 1 | ADR/FAULT | E/S | seleção de endereço I2C / falha | pull-up R2 15 k |
| 2 | LRCLK | entrada | clock de palavra (word clock) I2S | sincronismo L/R |
| 3 | SCLK | entrada | clock de bit (bit clock) I2S | — |
| 4 | SDIN | entrada | dados de áudio digital (I2S) | fluxo de entrada |
| 5 | SDOUT | saída | dados de áudio (loopback / TDM) | — |
| 6 | PDN | entrada | power-down (ativo em nível baixo) | pull-up R1 10 k |
| 7 | SCL | entrada | clock do barramento I2C | pull-up R3 10 k |
| 8 | SDA | E/S | dados do barramento I2C | pull-up R4 10 k |
| 9 | GND | alimentação | referência / terra | — |
| 10 | GND | alimentação | referência / terra | — |

> Os sinais I2S e I2C operam em nível lógico de 3,3 V. O pino ADR/FAULT define o endereço I2C do TAS5805M e reporta condições de falha do amplificador.

---

## Esquemático online

Visualize o projeto de hardware diretamente no navegador:

<iframe
  src="https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-DIGITAL-AMPLIFIER%2Ftree%2Fmain%2FHARDWARE"
  width="100%"
  height="700px"
  style={{border: '1px solid #333', borderRadius: '12px'}}
></iframe>

---

## Informações mecânicas e ambientais

| Item | Especificação |
| --- | --- |
| Borne de alimentação | J4 — screw terminal 2 vias (PVDD / GND) |
| Conector de áudio/controle | J3 — header 1x10 (I2S + I2C + GND) |
| Bornes de saída | J1 (Canal A) e J2 (Canal B) — screw terminal 2 vias |
| Furos de fixação | 2 x furos de montagem M3 |
| Dissipação térmica | PowerPAD do TAS5805M para o plano de cobre |
| Temperatura de operação | -25 °C a +85 °C |
| Tensão lógica | 3,3 V |
