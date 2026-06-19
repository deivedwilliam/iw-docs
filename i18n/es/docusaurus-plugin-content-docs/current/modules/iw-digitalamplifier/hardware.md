---
id: iw-digitalamplifier-hardware
title: Hardware
sidebar_label: Hardware
sidebar_position: 2
slug: /modules/iw-digitalamplifier/hardware
---

import BoardViewer from '@site/src/components/BoardViewer';

# Hardware

Esta sección detalla los aspectos eléctricos y las interfaces de la `iW-DigitalAmplifier` (Rev. RV01), a partir del datasheet del producto.

---

## Visión general de la placa

La iW-DigitalAmplifier es una breakout board de audio digital construida en torno al `TAS5805M` (Texas Instruments), un amplificador clase D estéreo de entrada digital con un procesador de audio integrado. Expone la alimentación de potencia (PVDD) en un borne, las señales de audio y control en un conector de 10 vías y las salidas de altavoz en bornes dedicados, con los filtros LC de salida ya integrados.

![Placa iW-DigitalAmplifier](/img/iw-digitalamplifier/BOARD_VIEW.png)

### Modelo 3D

Arrastra para girar, usa la rueda para hacer zoom.

<BoardViewer src="/models/iw-digital-amplifier-3d.glb" caption="Modelo 3D de la iW-DigitalAmplifier (preliminar)" />

---

## Características principales

- amplificador `TAS5805M`, clase D estéreo closed-loop, entrada digital
- potencia de 2 x 23 W (8 ohms, 21 V) estéreo, hasta 45 W en modo mono (4 ohms)
- entrada de audio I2S / TDM (LRCLK, SCLK, SDIN, SDOUT)
- control por I2C (SCL/SDA), dirección seleccionable por ADR/FAULT
- DSP integrado de 96 kHz: SRC, 15 biquads por canal, DRC de 3 bandas y AGL
- alimentación PVDD de 4,5 V a 26,4 V por borne
- filtro de salida LC integrado (10 uH + 0,68 uF por línea)
- salidas estéreo en puente (BTL) por bornes OUT_A / OUT_B
- interruptor de 3,3 V por P-MOSFET `AO3401A` y pin de protección FAULT
- 2 orificios de montaje M3

---

## Especificaciones eléctricas

| Parámetro | Mín. | Típ. | Máx. | Unidad / Observación |
| --- | --- | --- | --- | --- |
| Tensión de alimentación (PVDD) | 4,5 | 12 – 21 | 26,4 | V — borne J4 |
| Tensión lógica del sistema | — | 3,3 | — | V — raíl interno |
| Potencia de salida estéreo (2.0) | — | 2 x 23 | — | W — 8 ohms, 21 V, THD+N = 1% |
| Potencia de salida mono | — | 45 | — | W — 4 ohms, 21 V, THD+N = 1% |
| THD+N | — | ≤ 0,03 | — | % — 1 W, 1 kHz, PVDD = 12 V |
| SNR (ponderado A) | 107 | — | — | dB |
| Corriente quiescente | — | 16,5 | — | mA — PVDD = 13,5 V |
| Tasa de muestreo del DSP | — | 96 | — | kHz |
| Frecuencia de conmutación PWM | — | 384 / 768 | — | kHz — configurable |
| Temperatura de operación | -25 | — | +85 | °C |

> Valores de potencia, THD+N y SNR según el datasheet del TAS5805M (Texas Instruments). La potencia real depende de la tensión PVDD aplicada, la impedancia del altavoz y la disipación térmica del conjunto.

---

## Gestión de energía

La placa tiene dos dominios de alimentación. La potencia (PVDD) entra por el borne `J4`, pasa por un ferrite bead de 600 ohms (FB1) y por un banco de capacitores que alimenta las etapas de salida clase D. La lógica de 3,3 V alimenta las secciones digital y analógica del TAS5805M (DVDD, AVDD, VR_DIG), conmutada por un P-MOSFET `AO3401A`.

| Recurso | Descripción |
| --- | --- |
| Entrada de potencia | PVDD 4,5-26,4 V en el borne J4 (borne de tornillo de 2 vías) |
| Filtrado PVDD | C2/C3 390 uF + C1/C4 22 uF + C5/C6 100 nF; ferrite FB1 600 ohms |
| Raíl de 3,3 V | interruptor P-MOSFET AO3401A (Q1) con C19 10 uF / C20 100 nF |
| Dominios del CI | DVDD, AVDD y VR_DIG con desacoplo C9/C10 1 uF |
| Control / protección | pin PDN (power-down) y ADR/FAULT con pull-ups |

---

## Interfaces y etapa de salida

### Entrada de audio digital — I2S

Interfaz serial I2S / TDM para el flujo de audio: `LRCLK` (word clock), `SCLK` (bit clock), `SDIN` (datos de entrada) y `SDOUT` (loopback/TDM). Compatible con microcontroladores, DAC y SoC con salida I2S.

### Control — I2C

El TAS5805M se configura por I2C (`SCL`/`SDA`, con pull-ups de 10 k). La dirección del dispositivo la fija el pin `ADR/FAULT`, que también señaliza fallas (sobrecorriente, sobretemperatura, clipping). El pin `PDN` controla el modo power-down.

### Etapa de salida y filtro LC

Cada canal acciona una salida en puente (BTL): `OUT_A` y `OUT_B`. Antes de los bornes, cada línea pasa por un filtro LC (inductor de 10 uH + capacitor de 0,68 uF), reduciendo EMI y entregando audio limpio a los bornes de altavoz J1 y J2.

---

## Conector de audio y control (J3, 1x10)

El conector `J3` reúne las señales de audio digital (I2S), el bus de control (I2C) y las señales de gestión del TAS5805M, además de la tierra de referencia.

| Pin | Señal | Tipo | Función | Observaciones |
| --- | --- | --- | --- | --- |
| 1 | ADR/FAULT | E/S | selección de dirección I2C / falla | pull-up R2 15 k |
| 2 | LRCLK | entrada | word clock I2S | sincronismo L/R |
| 3 | SCLK | entrada | bit clock I2S | — |
| 4 | SDIN | entrada | datos de audio digital (I2S) | flujo de entrada |
| 5 | SDOUT | salida | datos de audio (loopback / TDM) | — |
| 6 | PDN | entrada | power-down (activo en bajo) | pull-up R1 10 k |
| 7 | SCL | entrada | clock del bus I2C | pull-up R3 10 k |
| 8 | SDA | E/S | datos del bus I2C | pull-up R4 10 k |
| 9 | GND | alimentación | referencia / tierra | — |
| 10 | GND | alimentación | referencia / tierra | — |

> Las señales I2S e I2C operan a un nivel lógico de 3,3 V. El pin ADR/FAULT fija la dirección I2C del TAS5805M y reporta las condiciones de falla del amplificador.

---

## Esquemático en línea

Visualiza el proyecto de hardware directamente en el navegador:

<iframe
  src="https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-DIGITAL-AMPLIFIER%2Ftree%2Fmain%2FHARDWARE"
  width="100%"
  height="700px"
  style={{border: '1px solid #333', borderRadius: '12px'}}
></iframe>

---

## Información mecánica y ambiental

| Ítem | Especificación |
| --- | --- |
| Borne de alimentación | J4 — borne de tornillo de 2 vías (PVDD / GND) |
| Conector de audio/control | J3 — header 1x10 (I2S + I2C + GND) |
| Bornes de salida | J1 (Canal A) y J2 (Canal B) — bornes de tornillo de 2 vías |
| Orificios de fijación | 2 x orificios de montaje M3 |
| Disipación térmica | PowerPAD del TAS5805M al plano de cobre |
| Temperatura de operación | -25 °C a +85 °C |
| Tensión lógica | 3,3 V |
