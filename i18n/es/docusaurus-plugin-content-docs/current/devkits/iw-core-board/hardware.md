---
id: iw-core-board-hardware
title: Hardware
sidebar_label: Hardware
sidebar_position: 2
slug: /devkits/iw-core-board/hardware
---

import BoardViewer from '@site/src/components/BoardViewer';

# Hardware

Esta sección detalla los aspectos físicos y eléctricos de la `iW-CORE-BOARD-V1` (Rev. RV01), a partir del datasheet del producto.

---

## Visión general de la placa

La iW-CORE-BOARD-V1 es una core board y plataforma de HMI construida en torno al **STM32H7** (Cortex-M7 hasta 480 MHz). Integra pantalla, conectividad inalámbrica, energía y expansión para servir como núcleo de procesamiento de una familia de placas de aplicación (shields).

![Placa iW-CORE-BOARD-V1](/img/iw-core-board/BOARD_VIEW.png)

### Modelo 3D

Arrastra para girar, usa la rueda para hacer zoom.

<BoardViewer src="/models/iw-coreboard-3d.glb" caption="Modelo 3D de la iW-CORE-BOARD-V1 (preliminar)" />

---

## Características principales

- MCU `STM32H743VIT6` (2 MB Flash) o `STM32H750VB` (128 KB + flash externa)
- 1 MB de SRAM interna (incl. DTCM/ITCM)
- pantalla TFT de 4,3" con 480x272 y touch capacitivo
- Wi-Fi 802.11 b/g/n + BLE por el módulo `Quectel FC41D`
- acelerómetro de 3 ejes por I2C
- PMIC `IP5306` (carga Li-ion, boost y encendido/apagado por botón)
- batería principal `BL-5C` y backup de RTC `CR1220`
- USB Full Speed (OTG) + USB-Serial para consola/log
- lector de tarjeta SD/microSD (SDMMC)
- dos conectores de expansión `iW-APPLICATION` (J4 y J5)
- compatible con `LVGL` y `TouchGFX`

---

## Especificaciones eléctricas

| Parámetro | Mín. | Típ. | Máx. | Unidad / Observación |
| --- | --- | --- | --- | --- |
| Tensión de entrada USB (V_BUS) | 4,5 | 5,0 | 5,5 | V — conector USB |
| Tensión de la batería (BL-5C, Li-ion) | 3,0 | 3,7 | 4,2 | V — celda única |
| Tensión lógica del sistema | — | 3,3 | — | V |
| Batería de backup RTC (CR1220) | — | 3,0 | — | V — celda de moneda |
| Frecuencia de reloj de la CPU | — | — | 480 | MHz (Cortex-M7) |
| Resolución del display | — | 480x272 | — | píxeles (4,3") |
| Corriente de carga de la batería | — | — | ~2,0 | A — IP5306 (típico) |
| Temperatura de operación | -40 | — | +85 | °C — limitada por los componentes |

> El consumo depende del firmware, el brillo del display, la potencia de RF y los periféricos activos. La corriente de carga la determina el IP5306 y la capacidad de la batería BL-5C.

---

## Gestión de energía (PMIC IP5306)

El circuito de energía gira en torno al `IP5306`, un CI tipo power-bank que integra un cargador Li-ion, un convertidor boost de 5 V y control de encendido/apagado por botón.

| Recurso | Descripción |
| --- | --- |
| Entradas de energía | USB (5 V) y batería Li-ion BL-5C |
| Carga | cargador Li-ion integrado, con indicación de estado |
| Salida | boost de 5 V y raíl de 3,3 V del sistema |
| Control ON/OFF | encendido/apagado por botón gestionado por el IP5306 |
| Backup del RTC | la celda de moneda CR1220 mantiene el reloj en tiempo real |

---

## Periféricos integrados

### Display TFT 4,3" con touch capacitivo

Panel a color de 480x272 accionado por la interfaz paralela RGB (controlador `LTDC` del STM32H7), con touch capacitivo leído por I2C. Soporta renderizado gráfico acelerado con LVGL y TouchGFX.

### Conectividad — Quectel FC41D

Wi-Fi 802.11 b/g/n y BLE en 2,4 GHz, habilitando conexión a red, aprovisionamiento, OTA y comunicación con apps y nube.

### Acelerómetro de 3 ejes

Sensor inercial por I2C para detección de movimiento, orientación de la pantalla, gestos y ahorro de energía.

### Almacenamiento — tarjeta SD

Lector SD/microSD por SDMMC para logs, recursos gráficos (fuentes, imágenes), configuraciones y datos de aplicación.

### USB y USB-Serial

El puerto USB Full Speed (OTG) del STM32H7 y un conversor USB-Serial dedicado para consola, depuración y grabación de firmware.

---

## Conectores de expansión iW-APPLICATION (J4 y J5)

La placa expone los GPIO del STM32H7 en dos conectores 2x9 (18 vías). La mayoría de los pines son multifunción; las funciones alternativas se refieren al `STM32H743VIT6` (LQFP100). Los pines analógicos de J5 sirven como entradas ADC.

### Conector J4

| Pin | Señal | Funciones alternativas | Observaciones |
| --- | --- | --- | --- |
| J4-1 | PE0 | TIM4_ETR, UART8_RX, FMC | GPIO de uso general |
| J4-2 | PE1 | UART8_TX, FMC | GPIO de uso general |
| J4-3 | PD5 | USART2_TX, FMC_NWE | UART2 TX |
| J4-4 | PD7 | USART2_CK, SPI1_MOSI | GPIO de uso general |
| J4-5 | PD2 | SDMMC1_CMD, UART5_RX, TIM3_ETR | GPIO de uso general |
| J4-6 | PD4 | USART2_RTS, FMC_NOE | GPIO de uso general |
| J4-7 | PD0 | UART4_RX, FDCAN1_RX, FMC_D2 | GPIO de uso general |
| J4-8 | PD1 | UART4_TX, FDCAN1_TX, FMC_D3 | GPIO de uso general |
| J4-9 | PC11 | SDMMC1_D3, USART3_RX, SPI3_MISO | GPIO de uso general |
| J4-10 | PC12 | SDMMC1_CK, UART5_TX, SPI3_MOSI | GPIO de uso general |
| J4-11 | PA15 | SPI1/SPI3_NSS, TIM2_CH1, JTDI | GPIO de uso general |
| J4-12 | PC10 | SDMMC1_D2, USART3_TX, SPI3_SCK | GPIO de uso general |
| J4-13 | I2C3_SDA | dato del bus I2C3 (PC9) | bus I2C3 |
| J4-14 | I2C3_SCL | clock del bus I2C3 (PA8) | bus I2C3 |
| J4-15 | 3V3 | — | salida regulada 3,3 V |
| J4-16 | PC8 | SDMMC1_D0, USART6_CK, TIM3_CH3 | GPIO de uso general |
| J4-17 | GND | — | referencia / tierra |
| J4-18 | 3V3 | — | salida regulada 3,3 V |

### Conector J5

| Pin | Señal | Funciones alternativas | Observaciones |
| --- | --- | --- | --- |
| J5-1 | GND | — | referencia / tierra |
| J5-2 | 5V | — | raíl de 5 V |
| J5-3 | 5V | — | raíl de 5 V |
| J5-4 | PD15 | TIM4_CH4, FMC_D1 | GPIO / PWM |
| J5-5 | NC | — | no conectado |
| J5-6 | PD14 | TIM4_CH3, FMC_D0 | GPIO / PWM |
| J5-7 | PD8 | USART3_TX, FMC_D13 | GPIO / UART3 TX |
| J5-8 | PD9 | USART3_RX, FMC_D14 | GPIO / UART3 RX |
| J5-9 | PB12 | SPI2_NSS, FDCAN2_RX, TIM1_BKIN | GPIO de uso general |
| J5-10 | PB13 | SPI2_SCK, FDCAN2_TX, USART3_CTS | GPIO de uso general |
| J5-11 | PC4 | ADC1/2, GPIO | entrada analógica (ADC) |
| J5-12 | PC5 | ADC1/2, GPIO | entrada analógica (ADC) |
| J5-13 | PA2 | ADC1/2, USART2_TX, TIM2_CH3 | entrada analógica (ADC) |
| J5-14 | PA7 | ADC1/2, SPI1_MOSI, TIM3_CH2 | entrada analógica (ADC) |
| J5-15 | PC2 | ADC1/2/3, SPI2_MISO | entrada analógica (ADC) |
| J5-16 | PC3 | ADC1/2/3, SPI2_MOSI | entrada analógica (ADC) |
| J5-17 | PE6 | SPI4_MOSI, TIM1_CH2N, TIM15_CH2 | GPIO de uso general |
| J5-18 | PC13 | RTC_TAMP1 / WKUP2 | uso restringido (dominio RTC) |

> Las funciones alternativas listadas son las principales del STM32H743VIT6. Consulta el datasheet y el reference manual de ST para el mapeo completo (AF0 a AF15). `PC13` pertenece al dominio del RTC y tiene capacidad de corriente limitada. Las líneas `I2C3_SDA/SCL` se comparten con periféricos internos de la placa.

---

## Botones y controles

| Botón | Función |
| --- | --- |
| ON/OFF | enciende y apaga la placa (control de energía por el IP5306) |
| RESET | reinicia el microcontrolador STM32H7 |
| BOOT | selección del modo de boot en el arranque |

---

## Esquemático en línea

Visualiza el proyecto de hardware directamente en el navegador:

<iframe
  src="https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-CORE-BOARD-V1%2Ftree%2Fmain%2FHARDWARE"
  width="100%"
  height="700px"
  style={{border: '1px solid #333', borderRadius: '12px'}}
></iframe>

---

## Versiones

| Variante | MCU · Flash interna | Uso recomendado |
| --- | --- | --- |
| iW-CORE-BOARD-V1 / H743 | STM32H743VIT6 · 2 MB | firmware standalone, HMI completa, recursos en la Flash interna |
| iW-CORE-BOARD-V1 / H750 | STM32H750VB · 128 KB | optimizada para costo; ejecución/almacenamiento desde memoria externa |

---

## Información mecánica y ambiental

| Ítem | Especificación |
| --- | --- |
| Display | TFT 4,3" · 480x272 · touch capacitivo |
| Conector USB | USB Full Speed + USB-Serial |
| Conectores de expansión | J4 y J5 — headers 2x9 (18 pines) |
| Almacenamiento | ranura para tarjeta SD/microSD |
| Batería principal | conector BL-5C (Li-ion 3,7 V) |
| Batería de backup | celda de moneda CR1220 (RTC) |
| Depuración | conector SWD |
| Temperatura de operación | -40 °C a +85 °C |
| Tensión lógica | 3,3 V |
