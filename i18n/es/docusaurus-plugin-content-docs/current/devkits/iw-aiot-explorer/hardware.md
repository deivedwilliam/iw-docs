---
id: iw-aiot-explorer-hardware
title: Hardware
sidebar_label: Hardware
sidebar_position: 2
slug: /devkits/iw-aiot-explorer/hardware
---

import BoardViewer from '@site/src/components/BoardViewer';

# Hardware

Esta sección detalla los aspectos físicos y eléctricos de la `iW-AIoT-eXplorer` (Rev. RV01), a partir del datasheet del producto.

---

## Vista de la placa

![Placa iW-AIoT-eXplorer](/img/iw-aiot-explorer/BOARD_VIEW.png)

### Identificación de componentes

![Componentes de la iW-AIoT-eXplorer](/img/iw-aiot-explorer/BOARD_COMPONENTS.png)

La cara frontal lleva la pantalla IPS de 3,5" con touch capacitivo; en la cara trasera están la cámara OV5640, el SoC ESP32-S3R8, la memoria flash, el módulo LoRa y los conectores de expansión.

---

## Modelo 3D

Modelo interactivo de la placa. Arrastra para girar, usa la rueda para hacer zoom y dos dedos (o el botón derecho) para desplazar. Haz clic en los ejes del gizmo (esquina inferior derecha) para alinear la cámara con cada vista.

<BoardViewer src="/models/iw-aiot-explorer-3d.glb" caption="Modelo 3D de la iW-AIoT-eXplorer (preliminar)" />

---

## Características principales

- SoC `ESP32-S3R8` (Xtensa LX7 dual-core, hasta 240 MHz) con instrucciones vectoriales para IA/ML
- 8 MB de PSRAM Octal SPI integrada en el encapsulado (sufijo R8)
- flash externa QSPI de 16 MB (`W25Q128`) o 32 MB (`W25Q256`)
- Wi-Fi 802.11 b/g/n + Bluetooth 5 LE / Mesh (2,4 GHz)
- LoRa / LoRaWAN `RFM95W-915S2` (SX1276), 915 MHz (AU915/US915)
- cámara `OV5640` — DVP 8 bits + SCCB (I2C)
- 2x micrófonos digitales PDM `IMP34DT05` (estéreo L/R)
- audio I2S Clase-D `MAX98357A` + conector de altavoz
- pantalla IPS 3,5" 320x240 `ST7789` (SPI) con touch capacitivo I2C
- acelerómetro de 3 ejes `LIS3DH` (I2C)
- microSD `Hirose DM3AT` en modo SPI
- expansor de I/O I2C `PCA9531` (backlight, resets, mux de UART, GPIO)
- USB-C con bridge USB-Serial `CH343P` y boot/reset automático
- PMIC `IP5306` (carga Li-ion + boost 5 V) y LDO `AMS1117-3,3`

---

## Especificaciones eléctricas

| Parámetro | Mín. | Típ. | Máx. | Unidad / Observación |
| --- | --- | --- | --- | --- |
| Tensión de entrada USB (V_BUS) | 4,5 | 5,0 | 5,5 | V — conector USB-C |
| Tensión de batería (Li-ion) | 3,0 | 3,7 | 4,2 | V — celda única |
| Raíl de boost (IP5306) | — | 5,0 | — | V — VOUT del IP5306 |
| Tensión lógica del sistema | — | 3,3 | — | V — LDO AMS1117-3,3 |
| Frecuencia de reloj de la CPU | — | — | 240 | MHz |
| Frecuencia del cristal | — | 40 | — | MHz (cap. de carga 20 pF) |
| PSRAM integrada | — | 8 | — | MB — Octal SPI (R8) |
| Flash externa QSPI | 16 | — | 32 | MB — W25Q128 / W25Q256 |
| Frecuencia LoRa | — | 915 | — | MHz (AU915 / US915) |
| Consumo típico (Wi-Fi activo) | — | ~250 | — | mA (pico de RF) |
| Consumo en deep-sleep | — | ~10 | — | uA (periféricos apagados) |
| Temperatura de operación | -40 | — | +85 | °C |

> Los valores de consumo son típicos del ESP32-S3 y varían según el firmware, la potencia de RF, los periféricos activos (cámara, display, audio) y el brillo del backlight.

---

## Gestión de energía (PMIC IP5306 + LDO)

El sistema de energía combina el `IP5306` — cargador Li-ion, convertidor boost síncrono (inductor de 1 uH) y gestión por botón con indicación de nivel por LEDs — con el regulador `AMS1117-3,3` que genera el raíl lógico de 3,3 V.

| Recurso | Descripción |
| --- | --- |
| Entradas de energía | USB-C (5 V vía V_BUS) y batería Li-ion (conector J5) |
| Carga | cargador Li-ion integrado en el IP5306, con sensado de corriente |
| Boost / salida 5 V | el convertidor boost síncrono del IP5306 (L3 1 uH) genera el raíl de 5 V |
| Raíl 3,3 V | LDO AMS1117-3,3 a partir del 5 V; LED indicador de energía |
| Indicación de batería | LEDs de nivel de carga accionados por el IP5306 |
| Control por botón | botón KEY (SW3) para encender/apagar y gestión de energía |

---

## Periféricos integrados

### Cámara — OmniVision OV5640

Sensor CMOS de 5 MP (2592x1944, 1/4") con autofoco, AEC/AGC y AWB integrados, conectado por interfaz paralela DVP de 8 bits y configurado por el bus SCCB (compatible con I2C). El reloj principal (MCLK) viene de un oscilador dedicado y los dominios analógico (2,8 V) y de núcleo (1,2 V) se generan con LDOs locales.

| Parámetro | Valor | Observación |
| --- | --- | --- |
| Sensor | OmniVision OV5640 | CMOS, formato óptico 1/4" |
| Resolución máxima | 5 MP — 2592x1944 | QSXGA |
| Tasas de cuadros | QSXGA 15 fps · 1080p 30 fps · 720p 60 fps | VGA 90 fps · QVGA 120 fps |
| Formatos de salida | RGB565/555/444, YUV422/420, RAW, JPEG | — |
| Interfaz de datos | DVP paralela 8 bits | PCLK / HSYNC / VSYNC + CSI_D0-D7 |
| Control | SCCB (compatible I2C) | SIO_C / SIO_D |
| Recursos | autofoco, AEC, AGC, AWB | MCU y motor de AF integrados |

> Los 4 bits menos significativos del bus de la cámara (`CSI_D0-D3`) se multiplexan con las líneas `DIO0-DIO3` del LoRa por el `QS3257QG`, con selección por `GPIO9` (0 = LoRa, 1 = cámara). Así, cámara y recepción de eventos de la radio operan de forma alternada.

### Micrófonos digitales — 2x IMP34DT05

Dos micrófonos MEMS PDM de STMicroelectronics en arreglo estéreo (canales L/R definidos por el pin `LR`), compartiendo las líneas `PDM_CLK` / `PDM_DOUT`. Omnidireccionales y de bajo consumo, ideales para keyword spotting, clasificación de sonido y beamforming.

| Parámetro | Valor |
| --- | --- |
| Configuración | estéreo (L / R) por una única línea de datos |
| SNR | 64 dB |
| Sensibilidad | -26 dBFS +/-3 dB |
| AOP | 122,5 dB SPL |
| Reloj PDM | 1,2 – 3,25 MHz |
| Alimentación | 3,3 V |

### Salida de audio — MAX98357A

Amplificador Clase-D I2S sin filtro que recibe audio digital directo del ESP32-S3 (`BCLK`/`LRCLK`/`DIN`), prescindiendo de DAC y MCLK externos, y acciona un altavoz por el conector dedicado (`J1`) en puente (BTL).

| Parámetro | Valor | Observación |
| --- | --- | --- |
| Potencia máx. de salida | 3,2 W | carga 4 ohms · 5 V · THD+N 10% |
| Eficiencia | hasta 92% | Clase-D |
| THD+N | 0,013% | 1 kHz típico |
| Ganancia | 3 / 6 / 9 / 12 / 15 dB | seleccionable; 9 dB por defecto |
| Carga | 4 – 8 ohms | 4 ohms para potencia máxima |
| Tasas de muestreo | 8 – 96 kHz | I2S, sin MCLK |

### Pantalla IPS 3,5" + touch — ST7789

Pantalla IPS de 3,5" y 320x240 con controlador `ST7789` por SPI; el backlight se conmuta con un MOSFET `AO3400A`, con control (encendido/apagado y PWM) por el expansor `PCA9531`. El touch capacitivo usa su propia interfaz I2C (interrupción en `GPIO6`).

| Parámetro | Valor | Observación |
| --- | --- | --- |
| Tipo | TFT LCD IPS | amplio ángulo de visión |
| Resolución | 320x240 | QVGA (horizontal) |
| Controlador | Sitronix ST7789 | GRAM integrada |
| Interfaz | SPI 4 hilos | SCLK / MOSI / MISO / CS / DC / RST |
| Backlight | LED vía MOSFET AO3400A | control y PWM por el PCA9531 |
| Touch | capacitivo · I2C | INT en GPIO6; SDA/SCL en el bus I2C |

### Acelerómetro — LIS3DH

Sensor MEMS de 3 ejes de ultrabajo consumo por I2C (interrupciones `INT1`/`INT2`), con fondos de escala seleccionables, FIFO interno y funciones integradas de detección de movimiento.

| Parámetro | Valor |
| --- | --- |
| Fondos de escala | +/-2 / +/-4 / +/-8 / +/-16 g |
| Resolución | 16 bits por eje |
| Tasa de datos (ODR) | 1 Hz – 5,3 kHz |
| Consumo | ~2 uA – 11 uA |
| Interrupciones | INT1 / INT2 (movimiento, click, caída libre, 4D/6D) |
| Recursos | FIFO 32 niveles, ADC aux., sensor de temperatura |

### LoRa / LoRaWAN — RFM95W-915S2

Módulo de radio basado en el Semtech `SX1276`, en 915 MHz, con interfaz SPI (compartida con el microSD), líneas de interrupción `DIO0-DIO2` y antena dedicada. El hardware implementa la capa física LoRa; el LoRaWAN (clases A/B/C) se habilita por software.

| Parámetro | Valor | Observación |
| --- | --- | --- |
| Módulo / chip | RFM95W-915S2 · SX1276 | transceptor LoRa sub-GHz |
| Banda | 915 MHz | AU915 / US915 (ISM) |
| Modulación | LoRa (CSS), (G)FSK, OOK | — |
| Potencia máx. de salida | +20 dBm (100 mW) | vía PA_BOOST |
| Sensibilidad de RX | hasta -148 dBm | según SF y ancho de banda |
| Consumo en TX | ~120 mA @ +20 dBm | ~87 mA @ +17 dBm |
| Consumo en RX | ~10,3 mA | recepción continua |
| Consumo en sleep | ~0,2 uA | radio en sleep |

### Almacenamiento — microSD (Hirose DM3AT)

Zócalo microSD push-push en modo SPI, con pull-ups de 10 k, compartiendo el bus SPI con el módulo LoRa (chip-selects independientes). Típicamente con sistema de archivos FAT16/FAT32 para imágenes, modelos de ML, datasets y logs.

### Expansor de I/O — PCA9531

Expansor/dimmer I2C de 8 salidas (drenaje abierto) que centraliza señales auxiliares, liberando GPIO del ESP32-S3.

| Salida | Señal | Función |
| --- | --- | --- |
| LED0 | BACKLIGHT | encendido/apagado y PWM de brillo del display |
| LED1 | LCD_RST | reset del controlador ST7789 |
| LED2 | Control auxiliar | línea reservada |
| LED3 | S1 | selección del mux de UART (HEF4052) |
| LED4 | S2 | selección del mux de UART (HEF4052) |
| LED5 | OUT1 | salida de uso general (conector de expansión) |
| LED6 | OUT2 | salida de uso general (conector de expansión) |
| LED7 | L_RST | reset del módulo LoRa (RFM95W) |

### USB-Serial — CH343P

Conversor USB-Serial (WCH `CH343P`) conectado al USB-C, con reset/boot automático por DTR/RTS (vía transistores BC817) accionando `EN` (reset) y `GPIO0` (boot) — grabación sin pulsar botones. La UART puede enrutarse entre el CH343P y el conector externo por el mux `HEF4052` (selección S1/S2 vía PCA9531).

---

## Mapa de asignación de GPIO (ESP32-S3)

| GPIO(s) | Señal | Interfaz | Observaciones |
| --- | --- | --- | --- |
| 18 / 15 / 14 | SCLK / MOSI / MISO | SPI | bus SPI del display ST7789 |
| 16 / 17 | CS / DC | SPI | chip-select y Data/Command del display |
| 11 / 12 / 10 | SCLK / MOSI / MISO | SPI | bus compartido microSD + LoRa |
| 13 / 21 | CS_SD / NSS_LoRa | SPI | selectores del microSD y del módulo LoRa |
| 5 / 19 | SCL / SDA | I2C | touch, LIS3DH, PCA9531 y conector (pull-up 2,2 k) |
| 6 | TOUCH_INT | I2C | interrupción del panel touch capacitivo |
| 7 / 8 / 4 | BCLK / LRCLK / DIN | I2S | audio para el MAX98357A |
| 20 / 3 | PDM_CLK / PDM_DOUT | PDM | micrófonos digitales (GPIO3 es strapping) |
| 40 / 42 / 45 / 46 | CSI_D4-D7 | DVP | 4 bits más significativos — directos al ESP32-S3 |
| 38 / 39 / 47 / 48 | CSI_D0-D3 / DIO0-D3 | DVP / LoRa | 4 LSB de la cámara multiplexados con DIO0-3 del LoRa (QS3257QG) |
| 9 | MUX_SEL | Control | selección del QS3257QG — 0 = LoRa (DIO), 1 = cámara (CSI_D0-3) |
| 1 / 2 / 41 | PCLK / HSYNC / VSYNC | DVP | sincronismo; SCCB (SIO_C/SIO_D) en su propio bus |
| 0 | BOOT | Strapping | botón BOOT (modo download) |
| CHIP_PU | RESET | — | botón RESET (reinicia el ESP32-S3) |

> `GPIO0`, `GPIO3`, `GPIO45` y `GPIO46` son pines de strapping del ESP32-S3 — observa las restricciones de nivel durante el boot. Cámara y LoRa comparten los GPIO 38/39/47/48 a través del `QS3257QG`, comandado por `GPIO9`, operando de forma alternada.

---

## Botones y controles

| Botón | Señal | Función |
| --- | --- | --- |
| RESET | CHIP_PU | reinicia el ESP32-S3R8 |
| BOOT | GPIO0 | entra en modo download / programación al reiniciar |
| KEY / ON | SW3 (IP5306) | encendido/apagado y gestión de energía por el PMIC |

---

## Esquemático en línea

Visualiza el proyecto de hardware directamente en el navegador:

<iframe
  src="https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-AIOT-EXPLORER%2Ftree%2Fmain%2FHARDWARE"
  width="100%"
  height="700px"
  style={{border: '1px solid #333', borderRadius: '12px'}}
></iframe>

---

## Versiones

| Variante | Flash externa QSPI | Aplicación recomendada |
| --- | --- | --- |
| iW-AIoT-eXplorer / 16 MB | Winbond W25Q128 (16 MB) | firmware AIoT estándar, OTA, modelos de ML compactos |
| iW-AIoT-eXplorer / 32 MB | Winbond W25Q256 (32 MB) | modelos de visión/audio más grandes, datasets en flash, multi-OTA |

---

## Información mecánica y ambiental

| Ítem | Especificación |
| --- | --- |
| Display | IPS 3,5" · 320x240 · ST7789 · touch capacitivo I2C |
| Conector principal | USB-C (alimentación + datos, vía CH343P) |
| Cámara | conector flex para el módulo OV5640 (DVP + SCCB) |
| Almacenamiento | zócalo microSD con detección (Hirose DM3AT) |
| Audio | conector de altavoz (J1, 2 vías) |
| Batería | conector para una celda Li-ion única (J5, 2 vías) |
| Expansión I2C | conector I2C dedicado (SDA / SCL / 3,3 V / GND) |
| Expansión UART/GPIO | conector con UART y GPIO vía PCA9531 |
| Antenas | cerámica AN9520-245 (2,4 GHz) + antena dedicada LoRa 915 MHz |
| Temperatura de operación | -40 °C a +85 °C |
| Tensión lógica | 3,3 V |
