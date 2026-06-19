---
id: iw-scott-hardware
title: Hardware
sidebar_label: Hardware
sidebar_position: 2
slug: /devkits/iw-scott/hardware
---

import BoardViewer from '@site/src/components/BoardViewer';

# Hardware

Esta página detalla los bloques de hardware de la `iW-Scott` (Rev. RV01), a partir del datasheet del producto.

![Placa iW-Scott](/img/iw-scott/BOARD_VIEW.png)

### Modelo 3D

Arrastra para girar, usa la rueda para hacer zoom.

<BoardViewer src="/models/iw-scott-3d.glb" caption="Modelo 3D de la iW-Scott (preliminar)" />

---

## Visión general de la placa

La iW-Scott es una Development Board Tools compacta y de bajo consumo construida en torno al SoC `ESP32-C3` (RISC-V, single-core, hasta 160 MHz) con Wi-Fi y Bluetooth 5 LE. Reúne gestión de energía inteligente, acelerómetro, LED RGB direccionable y memoria flash externa en un único módulo.

---

## Características principales

- SoC `ESP32-C3` (RISC-V 32 bits, hasta 160 MHz)
- memoria interna del SoC: 400 KB SRAM, 384 KB ROM, 4 MB flash
- flash externa QSPI Winbond `W25Q` — 8 MB o 16 MB
- Wi-Fi 802.11 b/g/n 2,4 GHz + Bluetooth 5 LE / Mesh
- acelerómetro de 3 ejes `LIS3DH` (I2C/SPI)
- LED RGB direccionable `WS2812B` (NeoPixel)
- PMIC `PT1502` (cargador Li-ion, buck y LDOs)
- antena cerámica `AN9520-245` y conector U.FL
- cristal de 40 MHz (capacitores de carga 22 pF)
- USB-C con protección ESD `USBLC6`

---

## Especificaciones eléctricas

| Parámetro | Mín. | Típ. | Máx. | Unidad / Observación |
| --- | --- | --- | --- | --- |
| Tensión de entrada USB (V_BUS) | 4,5 | 5,0 | 5,5 | V — conector USB-C |
| Tensión de batería (Li-ion) | 3,0 | 3,7 | 4,2 | V — celda única |
| Tensión lógica del sistema | — | 3,3 | — | V — generada por el PT1502 |
| Corriente de carga de la batería | — | — | ~1,0 | A — programable (PT1502) |
| Frecuencia de reloj de la CPU | — | — | 160 | MHz |
| Frecuencia del cristal | — | 40 | — | MHz |
| Consumo modo activo (Wi-Fi TX) | — | ~280 | — | mA (pico de transmisión) |
| Consumo en modem-sleep | — | ~20 | — | mA |
| Consumo en deep-sleep | — | ~5 | — | uA |
| Temperatura de operación | -40 | — | +85 | °C |

> Los valores de consumo son típicos del ESP32-C3 y varían según el firmware, la potencia de RF y los periféricos activos. La corriente de carga depende de la configuración de resistores del PT1502 y de la capacidad de la batería.

---

## Gestión de energía (PMIC PT1502)

El circuito de energía gira en torno al `PT1502`, que integra un cargador Li-ion, un convertidor buck (inductor de 2,2 uH) y LDOs. Selecciona automáticamente entre USB (5 V) y batería, generando el raíl de 3,3 V del sistema.

| Recurso | Descripción |
| --- | --- |
| Entradas de energía | USB-C (5 V vía V_BUS) y batería Li-ion |
| Carga | cargador Li-ion integrado, con estado por LED (CHG_STAT) |
| Salidas | buck regulado + LDOs para 3,3 V y raíles auxiliares |
| Monitoreo | divisor BAT_LEVEL para leer la tensión de la batería por ADC; señales BAT_LOW / CHG_PROG |
| Control | señales PLAY_ON, uP_RESET y PWR_HOLD para encendido/apagado |

---

## Periféricos integrados

### Acelerómetro — LIS3DH

Sensor inercial de 3 ejes de ultrabajo consumo (I2C y SPI), ideal para detección de movimiento, orientación, gestos y TinyML. Conectado a las líneas SDA/SCL/CS del ESP32-C3.

### LED RGB — WS2812B

LED direccionable (NeoPixel) controlado por un único pin de datos (línea DIN), accionado por la línea `U0TXD/GPIO21`. Permite señalización de estado a color por un protocolo serial de 1 hilo.

### Memoria flash QSPI — W25Q

Flash serial externa Winbond, en 8 MB (`W25Q64`) o 16 MB (`W25Q128`), por bus QSPI (CS/CLK/IO0-IO3) para firmware, modelos de ML, sistemas de archivos y datos.

---

## Mapa de pines / conector de expansión (2x9)

La placa expone los GPIO del ESP32-C3 en un conector doble de 18 vías. La mayoría de los pines son multifunción (GPIO, ADC1, I2C, SPI, UART y PWM por software).

| Pin | Señal | Funciones alternativas | Observaciones |
| --- | --- | --- | --- |
| 1 | GPIO0 | ADC1_CH0, XTAL_32K_P | entrada analógica |
| 2 | GPIO1 | ADC1_CH1, XTAL_32K_N | entrada analógica |
| 3 | GPIO2 | ADC1_CH2 | strapping / pull-up de boot (4,7 k) |
| 4 | GPIO3 | ADC1_CH3 | entrada analógica |
| 5 | GPIO4 | ADC1_CH4, MTMS | JTAG / ADC |
| 6 | GPIO5 | ADC2_CH0, MTDI | JTAG / ADC |
| 7 | GPIO6 | MTCK | GPIO / JTAG |
| 8 | GPIO7 | MTDO | GPIO / JTAG |
| 9 | GPIO8 | — | strapping; pull-up de boot (4,7 k) |
| 10 | GPIO9 | BOOT | strapping; botón BOOT (download) |
| 11 | GPIO10 | PLAY_ON | control de encendido |
| 12 | GPIO20 | U0RXD | UART0 (consola) — RX |
| 13 | GPIO21 | U0TXD | UART0 (consola) — TX; -> LED WS2812B |
| 14 | 3V3 | — | salida regulada 3,3 V |
| 15 | GND | — | referencia / tierra |
| 16 | GND | — | referencia / tierra |
| 17 | BATT | — | tensión de la batería Li-ion (3,0-4,2 V) |
| 18 | 5V | — | entrada/salida 5 V (V_BUS del USB-C) |

> `GPIO2`, `GPIO8` y `GPIO9` son pines de strapping — observa las restricciones de nivel durante el boot. La numeración sigue el conector de expansión 2x9 (J4).

---

## Botones y controles

| Botón | Señal | Función |
| --- | --- | --- |
| RESET (EN) | ESP_EN | reinicia el ESP32-C3 (vía transistor BC807) |
| BOOT | GPIO9 | entra en modo download / programación al reiniciar |
| PLAY / ON | GPIO10 / PLAY_ON | encendido y control de energía (PT1502) |

---

## Conectividad

- Wi-Fi para integración en red local e IoT
- Bluetooth 5 LE / Mesh para apps, beacons y periféricos BLE
- antena cerámica integrada con opción de U.FL para antena externa
- GPIO disponibles para expansión y periféricos adicionales

---

## Esquemático en línea

Visualiza el proyecto de hardware directamente en el navegador:

<iframe
  src="https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-SCOTT-DEVKIT%2Ftree%2Fmain%2FHARDWARE"
  width="100%"
  height="700px"
  style={{border: '1px solid #333', borderRadius: '12px'}}
></iframe>

---

## Versiones

| Variante | Flash externa QSPI | Uso recomendado |
| --- | --- | --- |
| iW-Scott / 8 MB | Winbond W25Q64 (8 MB) | firmware IoT estándar, OTA, sistemas de archivos |
| iW-Scott / 16 MB | Winbond W25Q128 (16 MB) | modelos de ML más grandes, logging extenso, multi-OTA |

---

## Información mecánica y ambiental

| Ítem | Especificación |
| --- | --- |
| Conector principal | USB-C (alimentación + datos) |
| Conector de expansión | header 2x9 (18 pines), paso 2,54 mm |
| Batería | conectores dedicados para una celda Li-ion única |
| Temperatura de operación | -40 °C a +85 °C |
| Tensión lógica | 3,3 V |
