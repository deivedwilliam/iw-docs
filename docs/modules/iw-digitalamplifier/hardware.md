---
id: iw-digitalamplifier-hardware
title: Hardware
sidebar_label: Hardware
sidebar_position: 2
slug: /modules/iw-digitalamplifier/hardware
---

import BoardViewer from '@site/src/components/BoardViewer';

# Hardware

Esta secao detalha os aspectos eletricos e as interfaces da `iW-DigitalAmplifier` (Rev. RV01), a partir do datasheet do produto.

---

## Visao geral da placa

A iW-DigitalAmplifier e uma breakout board de audio digital construida em torno do `TAS5805M` (Texas Instruments), um amplificador classe D estereo de entrada digital com processador de audio integrado. Ela expoe a alimentacao de potencia por borne, os sinais de audio e controle em um conector de 10 vias e as saidas de alto-falante por bornes dedicados, ja com filtros LC integrados.

![Placa iW-DigitalAmplifier](/img/iw-digitalamplifier/BOARD_VIEW.png)

### Modelo 3D

Arraste para girar, role para dar zoom.

<BoardViewer src="/models/iw-digital-amplifier-3d.glb" caption="Modelo 3D da iW-DigitalAmplifier (preliminar)" />

---

## Caracteristicas principais

- amplificador `TAS5805M`, classe D estereo closed-loop, entrada digital
- potencia de 2 x 23 W (8 ohms, 21 V) estereo, ate 45 W em modo mono (4 ohms)
- entrada de audio I2S / TDM (LRCLK, SCLK, SDIN, SDOUT)
- controle por I2C (SCL/SDA), endereco selecionavel por ADR/FAULT
- DSP integrado 96 kHz: SRC, 15 biquads por canal, DRC de 3 bandas e AGL
- alimentacao PVDD de 4,5 V a 26,4 V por borne (screw terminal)
- filtro de saida LC integrado (10 uH + 0,68 uF por linha)
- saidas estereo em ponte (BTL) por bornes OUT_A / OUT_B
- chave de 3,3 V por P-MOSFET `AO3401A` e pino de protecao FAULT
- 2 furos de montagem M3

---

## Especificacoes eletricas

| Parametro | Min. | Tip. | Max. | Unidade / Observacao |
| --- | --- | --- | --- | --- |
| Tensao de alimentacao (PVDD) | 4,5 | 12 – 21 | 26,4 | V — borne J4 |
| Tensao logica do sistema | — | 3,3 | — | V — trilho interno |
| Potencia de saida estereo (2.0) | — | 2 x 23 | — | W — 8 ohms, 21 V, THD+N = 1% |
| Potencia de saida mono | — | 45 | — | W — 4 ohms, 21 V, THD+N = 1% |
| THD+N | — | ≤ 0,03 | — | % — 1 W, 1 kHz, PVDD = 12 V |
| SNR (ponderado A) | 107 | — | — | dB |
| Corrente quiescente | — | 16,5 | — | mA — PVDD = 13,5 V |
| Taxa de amostragem do DSP | — | 96 | — | kHz |
| Frequencia de chaveamento PWM | — | 384 / 768 | — | kHz — configuravel |
| Temperatura de operacao | -25 | — | +85 | °C |

> Valores de potencia, THD+N e SNR conforme o datasheet do TAS5805M (Texas Instruments). A potencia real depende da tensao PVDD aplicada, da impedancia do alto-falante e da dissipacao termica do conjunto.

---

## Gestao de energia

A placa tem dois dominios de alimentacao. A potencia (PVDD) entra pelo borne `J4`, passa por um ferrite bead de 600 ohms (FB1) e por um banco de capacitores que alimenta os estagios de saida classe D. A logica de 3,3 V alimenta as secoes digital e analogica do TAS5805M (DVDD, AVDD, VR_DIG), com chaveamento por P-MOSFET `AO3401A`.

| Recurso | Descricao |
| --- | --- |
| Entrada de potencia | PVDD 4,5-26,4 V no borne J4 (screw terminal 2 vias) |
| Filtragem PVDD | C2/C3 390 uF + C1/C4 22 uF + C5/C6 100 nF; ferrite FB1 600 ohms |
| Trilho de 3,3 V | chave P-MOSFET AO3401A (Q1) com C19 10 uF / C20 100 nF |
| Dominios do CI | DVDD, AVDD e VR_DIG com decoupling C9/C10 1 uF |
| Controle / protecao | pino PDN (power-down) e ADR/FAULT com pull-ups |

---

## Interfaces e estagio de saida

### Entrada de audio digital — I2S

Interface serial I2S / TDM para o fluxo de audio: `LRCLK` (clock de palavra), `SCLK` (clock de bit), `SDIN` (dados de entrada) e `SDOUT` (loopback/TDM). Compativel com microcontroladores, DACs e SoCs com saida I2S.

### Controle — I2C

O TAS5805M e configurado por I2C (`SCL`/`SDA`, com pull-ups de 10 k). O endereco do dispositivo e definido pelo pino `ADR/FAULT`, que tambem sinaliza falhas (sobrecorrente, sobretemperatura, clipping). O pino `PDN` controla o modo power-down.

### Estagio de saida e filtro LC

Cada canal aciona uma saida em ponte (BTL): `OUT_A` e `OUT_B`. Antes dos bornes, cada linha passa por um filtro LC (indutor de 10 uH + capacitor de 0,68 uF), reduzindo EMI e entregando audio limpo aos bornes de alto-falante J1 e J2.

---

## Conector de audio e controle (J3, 1x10)

O conector `J3` reune os sinais de audio digital (I2S), o barramento de controle (I2C) e os sinais de gerenciamento do TAS5805M, alem do terra de referencia.

| Pino | Sinal | Tipo | Funcao | Observacoes |
| --- | --- | --- | --- | --- |
| 1 | ADR/FAULT | E/S | selecao de endereco I2C / falha | pull-up R2 15 k |
| 2 | LRCLK | entrada | clock de palavra (word clock) I2S | sincronismo L/R |
| 3 | SCLK | entrada | clock de bit (bit clock) I2S | — |
| 4 | SDIN | entrada | dados de audio digital (I2S) | fluxo de entrada |
| 5 | SDOUT | saida | dados de audio (loopback / TDM) | — |
| 6 | PDN | entrada | power-down (ativo em nivel baixo) | pull-up R1 10 k |
| 7 | SCL | entrada | clock do barramento I2C | pull-up R3 10 k |
| 8 | SDA | E/S | dados do barramento I2C | pull-up R4 10 k |
| 9 | GND | alimentacao | referencia / terra | — |
| 10 | GND | alimentacao | referencia / terra | — |

> Os sinais I2S e I2C operam em nivel logico de 3,3 V. O pino ADR/FAULT define o endereco I2C do TAS5805M e reporta condicoes de falha do amplificador.

---

## Esquematico online

Visualize o projeto de hardware diretamente no navegador:

<iframe
  src="https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-DIGITAL-AMPLIFIER%2Ftree%2Fmain%2FHARDWARE"
  width="100%"
  height="700px"
  style={{border: '1px solid #333', borderRadius: '12px'}}
></iframe>

[Abrir no KiCanvas](https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-DIGITAL-AMPLIFIER%2Ftree%2Fmain%2FHARDWARE)

---

## Informacoes mecanicas e ambientais

| Item | Especificacao |
| --- | --- |
| Borne de alimentacao | J4 — screw terminal 2 vias (PVDD / GND) |
| Conector de audio/controle | J3 — header 1x10 (I2S + I2C + GND) |
| Bornes de saida | J1 (Canal A) e J2 (Canal B) — screw terminal 2 vias |
| Furos de fixacao | 2 x furos de montagem M3 |
| Dissipacao termica | PowerPAD do TAS5805M para o plano de cobre |
| Temperatura de operacao | -25 °C a +85 °C |
| Tensao logica | 3,3 V |
