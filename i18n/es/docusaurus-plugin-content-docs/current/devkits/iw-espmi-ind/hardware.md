---
id: iw-espmi-ind-hardware
title: Hardware
sidebar_label: Hardware
sidebar_position: 2
slug: /devkits/iw-espmi-ind/hardware
---

import BoardViewer from '@site/src/components/BoardViewer';

# Hardware

Esta sección describe los principales aspectos físicos y eléctricos de la `iW-ESPMI-IND`.

---

## Visión general de la placa

La iW-ESPMI-IND fue diseñada para aplicaciones industriales con una fuerte necesidad de comunicación e interfaz visual.

![Placa iW-ESPMI-IND](/img/iw-espmi-ind/BOARD_VIEW.png)

### Modelo 3D

Arrastra para girar, usa la rueda para hacer zoom.

<BoardViewer src="/models/iw-espmi-ind-3d.glb" caption="Modelo 3D de la iW-ESPMI-IND (preliminar)" />

---

## Pinout

La imagen siguiente resume las interfaces más importantes de la placa.

![Pinout de la iW-ESPMI-IND](/img/IW-ESPMI-IND_PINOUT.png)

### Interfaces principales

- USB para programación y comunicación
- RS485 para red Modbus RTU
- tarjeta SD para almacenamiento local
- HDMI para salida de vídeo

---

## Alimentación

La placa admite un rango de alimentación amplio, lo que facilita su uso en escenarios industriales. La entrada pasa por protección de polaridad (MOSFET `AO3401A` + zener) antes de la etapa de potencia basada en el convertidor `LMR61430`, que establece el raíl de 5 V (5VP). De ahí, el LDO `AMS1117-3.3` genera el raíl de 3,3 V del sistema; un LED indica la presencia de la tensión de 3,3 V.

| Recurso | Descripción |
| --- | --- |
| Entrada de energía | borne de tornillo (J1) con protección de polaridad inversa |
| Etapa de potencia | convertidor LMR61430 + inductor 6,8 uH -> raíl 5 V (5VP) |
| Regulador 3,3 V | LDO AMS1117-3.3 alimentando la lógica y el ESP32-S3 |
| USB | V_BUS (5 V) del USB-C integrado al raíl 5VP |
| Indicación | LED de estado del raíl de 3,3 V |

### Especificaciones eléctricas

| Parámetro | Mín. | Típ. | Máx. | Unidad / Observación |
| --- | --- | --- | --- | --- |
| Tensión de entrada (borne) | 6,5 | — | 36 | V — entrada de alimentación |
| Raíl intermedio (5VP) | — | 5,0 | — | V — etapa de potencia |
| Tensión lógica del sistema | — | 3,3 | — | V — AMS1117-3.3 |
| Tensión USB (V_BUS) | 4,5 | 5,0 | 5,5 | V — conector USB-C |
| Frecuencia de reloj de la CPU | — | — | 240 | MHz |
| Salida de vídeo | — | HDMI 1.4 | — | DVI/HDMI vía TFP410 |
| Terminación RS485 | — | 120 | — | ohms — seleccionable por JP1 |
| Tensión lógica de I/O | — | 3,3 | — | V |
| Temperatura de operación | -40 | — | +85 | °C — limitada por el ESP32-S3 |

> Usa una fuente estable cuando haya display HDMI, tarjeta SD y cargas externas conectadas al mismo tiempo. El rango de resolución / reloj de píxel de la salida HDMI depende de la configuración del firmware y de los límites del TFP410.

---

## Interfaces y recursos

### Vídeo

- salida HDMI
- buena base para interfaces con LVGL y herramientas visuales

### Comunicación industrial

- transceptor RS485 integrado
- buena adherencia a escenarios con Modbus RTU

### Almacenamiento

- ranura para tarjeta SD para logs, assets y configuraciones

---

## Esquemático en línea

Visualiza el proyecto de hardware directamente en el navegador:

<iframe
  src="https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-ESPMI-IND%2Ftree%2Fmain"
  width="100%"
  height="700px"
  style={{border: '1px solid #333', borderRadius: '12px'}}
></iframe>

---

## Buenas prácticas

- usa alimentación estable en campo
- cuida la puesta a tierra en redes RS485
- separa bien los cables de potencia y de datos
- valida la disipación y el ruido en instalaciones industriales
