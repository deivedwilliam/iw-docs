---
id: iw-starter-kit-hardware
title: Hardware
sidebar_label: Hardware
sidebar_position: 2
slug: /devkits/iw-starter-kit/hardware
---

import BoardViewer from '@site/src/components/BoardViewer';

# Hardware

Esta sección detalla los aspectos físicos y eléctricos de la `iW-ST-StarterKit` (Rev. RV01), a partir del datasheet del producto.

---

## Visión general de la placa

La iW-ST-StarterKit es una devkit compacta y de bajo costo construida en torno al `STM32F030F4P6` (ARM Cortex-M0, hasta 48 MHz). Integra todo lo necesario para programar y depurar sin hardware externo: USB-C, puente USB-Serial, regulador on-board, protección ESD y botones dedicados.

![Placa iW-ST Starter Kit](/img/iw-starter-kit/BOARD_VIEW.png)

### Modelo 3D

Arrastra para girar, usa la rueda para hacer zoom.

<BoardViewer src="/models/iw-starter-kit-3d.glb" caption="Modelo 3D de la iW-ST Starter Kit (preliminar)" />

---

## Características principales

- MCU `STM32F030F4P6` (Cortex-M0, hasta 48 MHz, TSSOP-20)
- 16 KB de Flash + 4 KB de SRAM
- Conector USB-C con protección ESD `USBLC6-2P6`
- Puente serial `CH343P` (USB a UART / USART1)
- Regulador LDO `AP2112K-3.3` (5 V a 3,3 V, hasta 600 mA)
- Cristal externo de 20 MHz (capacitores de carga 22 pF)
- Programación por SWD (PA13/PA14) y bootloader serial vía BOOT0
- Hasta 15 GPIO multifunción en los conectores de expansión

---

## Especificaciones eléctricas

| Parámetro | Mín. | Típ. | Máx. | Unidad / Observación |
| --- | --- | --- | --- | --- |
| Tensión de entrada USB (V_BUS) | 4,5 | 5,0 | 5,5 | V — conector USB-C |
| Tensión lógica del sistema | — | 3,3 | — | V — generada por el AP2112K |
| Corriente máx. del regulador | — | — | 600 | mA — AP2112K-3.3 |
| Frecuencia de reloj de la CPU | — | — | 48 | MHz |
| Frecuencia del cristal externo | — | 20 | — | MHz |
| Tensión de operación del MCU | 2,4 | 3,3 | 3,6 | V — STM32F030F4 |
| Consumo típico (Run, 48 MHz) | — | ~12 | — | mA |
| Consumo en Stop | — | ~5 | — | uA |
| Temperatura de operación | -40 | — | +85 | °C |

> Los valores de consumo son típicos del STM32F030F4 y varían según el firmware, el reloj y los periféricos activos.

---

## Gestión de energía

La alimentación llega por el conector USB-C (5 V). El LDO `AP2112K-3.3` convierte los 5 V en 3,3 V estables para el MCU y los periféricos. Los raíles de 3,3 V y 5 V también están disponibles en los conectores de expansión.

| Recurso | Descripción |
| --- | --- |
| Entrada de energía | USB-C (5 V vía V_BUS) |
| Regulación | LDO AP2112K-3.3 — salida 3,3 V hasta 600 mA |
| Desacoplo | capacitores de 100 nF / 10 uF en los raíles de 5 V y 3,3 V |
| Filtrado analógico | ferrite bead (600R) + 100 nF en el pin VDDA del MCU |
| Salidas disponibles | pines 3V3 y 5V en los conectores de expansión |

---

## Periféricos e interfaces

### Puente USB-Serial — CH343P

Conversor USB a UART que conecta la `USART1` del STM32 (PA9/TX y PA10/RX) directamente al ordenador por USB-C, permitiendo comunicación serial, registro de depuración y programación por bootloader sin adaptador externo.

### Depuración — SWD

Los pines `PA13` (SWDIO) y `PA14` (SWCLK) exponen la interfaz Serial Wire Debug, para programación y depuración con herramientas como ST-Link. Disponibles en el conector de expansión.

### Protección ESD — USBLC6-2P6

Componente dedicado de protección contra descargas electrostáticas en las líneas de datos D+/D- del USB-C.

---

## Mapa de pines / conectores de expansión

La placa expone los GPIO del STM32F030F4P6 en dos conectores laterales. La mayoría de los pines son multifunción (GPIO, ADC, I2C, SPI, USART, temporizadores y PWM).

| Pin | Señal | Funciones alternativas | Observaciones |
| --- | --- | --- | --- |
| 1 | PA0 | ADC_IN0, USART2_CTS, TIM2_CH1 | entrada analógica / wakeup |
| 2 | PA1 | ADC_IN1, USART2_RTS, TIM2_CH2 | entrada analógica |
| 3 | PA2 | ADC_IN2, USART2_TX, TIM2_CH3 | entrada analógica / UART |
| 4 | PA3 | ADC_IN3, USART2_RX, TIM2_CH4 | entrada analógica / UART |
| 5 | PA4 | ADC_IN4, SPI1_NSS | entrada analógica / SPI |
| 6 | PA5 | ADC_IN5, SPI1_SCK | entrada analógica / SPI |
| 7 | PA6 | ADC_IN6, SPI1_MISO, TIM3_CH1 | entrada analógica / SPI |
| 8 | PA7 | ADC_IN7, SPI1_MOSI, TIM3_CH2 | entrada analógica / SPI |
| 9 | PB1 | ADC_IN9, TIM3_CH4 | entrada analógica |
| 10 | PA9 | USART1_TX, I2C1_SCL | UART1 TX -> CH343P |
| 11 | PA10 | USART1_RX, I2C1_SDA | UART1 RX -> CH343P |
| 12 | PA13 | SWDIO | depuración SWD |
| 13 | PA14 | SWCLK | depuración SWD |
| 14 | 3V3 | — | salida regulada 3,3 V |
| 15 | 5V | — | entrada/salida 5 V (V_BUS del USB-C) |
| 16 | GND | — | referencia / tierra |

> `PA13/PA14` son la interfaz SWD por defecto — reconfigúralos como GPIO solo si no usas depuración. La numeración sigue la serigrafía de los conectores laterales.

---

## Botones y controles

| Botón | Señal | Función |
| --- | --- | --- |
| NRST | NRST | reinicia el microcontrolador STM32F030 |
| BOOT0 | BOOT0 | entra en el bootloader del sistema (programación serial) al reiniciar |

---

## Esquemático en línea

Visualiza el proyecto de hardware directamente en el navegador:

<iframe
  src="https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-ST-STARTER-KIT%2Ftree%2Fmain%2FHARDWARE"
  width="100%"
  height="700px"
  style={{border: '1px solid #333', borderRadius: '12px'}}
></iframe>

---

## Información mecánica y ambiental

| Ítem | Especificación |
| --- | --- |
| Conector principal | USB-C (alimentación + datos) |
| Conectores de expansión | headers laterales, paso 2,54 mm |
| Microcontrolador | STM32F030F4P6 — TSSOP-20 |
| Temperatura de operación | -40 °C a +85 °C |
| Tensión lógica | 3,3 V |
