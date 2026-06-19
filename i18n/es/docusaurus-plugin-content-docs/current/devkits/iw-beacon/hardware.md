---
id: iw-beacon-hardware
title: Hardware
sidebar_label: Hardware
sidebar_position: 2
slug: /devkits/iw-beacon/hardware
---

import BoardViewer from '@site/src/components/BoardViewer';

# Hardware

Esta sección describe los bloques principales de la `iW-Beacon`, extraídos del esquema eléctrico del proyecto (`iW-iBeacon`).

---

## Visión general de la placa

La iW-Beacon se organiza en cuatro bloques: el SoC de radio `nRF52832`, los sensores en el bus I2C, el circuito de energía con el temporizador nano-power y las interfaces de depuración.

![Placa iW-Beacon](/img/iw-beacon/BOARD_VIEW.png)

### Modelo 3D

Arrastra para girar, usa la rueda para hacer zoom.

<BoardViewer src="/models/iw-beacon-3d.glb" caption="Modelo 3D de la iW-Beacon (preliminar)" />

---

## Procesamiento y radio — nRF52832

- SoC Nordic `nRF52832` (ARM Cortex-M4F de 32 bits)
- Bluetooth 5 Low Energy
- cristal principal de 32 MHz
- cristal de 32.768 kHz para el RTC y la temporización de bajo consumo
- pines de NFC disponibles para emparejamiento por toque
- convertidor DC/DC interno (líneas DCC) para eficiencia de RF

La salida de antena pasa por una red de adaptación (inductores de 15 nH y 10 uH) hasta la antena de la placa.

---

## Sensores integrados

### Acelerómetro — LIS3DH

Sensor inercial de 3 ejes de ultrabajo consumo, conectado al SoC por interfaz I2C/SPI y líneas de interrupción (`INT1`, `INT2`). Útil para despertar la placa por movimiento, y para detectar vuelco, orientación y manipulación.

### Temperatura y humedad — SHTC3

Sensor `SHTC3` (Sensirion) en el bus I2C, con pull-ups de 2,2 k. Proporciona lectura de temperatura y humedad relativa para telemetría de ambiente directamente en el payload del beacon.

> Ambos sensores comparten el mismo bus I2C del nRF52832, lo que simplifica el firmware y el ruteo.

---

## Gestión de energía (nano-power)

El circuito de energía fue diseñado para máxima autonomía.

- batería `BT1` (tipo moneda / pila) como fuente primaria
- temporizador nano-power `TPL5110` (`U4`) que define el intervalo de wakeup
- interruptor de carga por P-MOSFET `AO3401A` (`Q2`), controlado por el temporizador
- jumper `JP1` para seleccionar el modo de alimentación
- banco de capacitores (47 uF + 4,7 uF + 100 nF) para estabilizar los picos de transmisión

### Cómo funciona el ciclo

1. El `TPL5110` mantiene la placa apagada durante el intervalo programado.
2. Al final del intervalo, acciona el MOSFET y alimenta el `nRF52832`.
3. El SoC despierta, lee los sensores y transmite el paquete BLE.
4. El firmware señaliza `DONE` al temporizador, que corta la alimentación de nuevo.

Este esquema reduce el consumo medio a microamperios, ya que el SoC solo consume corriente durante la breve ventana de transmisión.

---

## Interfaces y depuración

- conector SWD (`J1`, 3 vías: `SWDCLK`, `SWDIO`, `GND`) para grabación y debug
- líneas de consola `UART_TX` / `UART_RX`
- bus I2C compartido por los sensores
- botón/línea de `RESET`

---

## Características eléctricas (típicas)

| Parámetro | Valor | Observación |
| --- | --- | --- |
| Tensión lógica | 3,3 V | raíl del sistema |
| Batería | celda única | tipo moneda / pila (`BT1`) |
| Interruptor de carga | AO3401A | P-MOSFET controlado por el TPL5110 |
| Consumo medio | rango de microamperios | dominado por el ciclo del TPL5110 |
| Cristales | 32 MHz + 32.768 kHz | radio + RTC |

> Los valores anteriores se derivan de los componentes del esquema. El consumo real depende del intervalo de transmisión, de la potencia de RF y de los sensores activos.

---

## Buenas prácticas

- programa el intervalo del `TPL5110` según el compromiso entre frescura del dato y autonomía
- usa el acelerómetro como disparador de transmisiones orientadas a eventos
- mantén el payload reducido para acortar la ventana de radio activa
- valida la adaptación de antena antes de cerrar la carcasa
