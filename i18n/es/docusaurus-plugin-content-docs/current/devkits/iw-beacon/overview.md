---
id: iw-beacon-overview
title: iW-Beacon
sidebar_label: Visión general
sidebar_position: 1
slug: /devkits/iw-beacon
---

# iW-Beacon

La `iW-Beacon` es un beacon Bluetooth Low Energy de Indústrias William construido en torno al SoC **Nordic nRF52832** (ARM Cortex-M4F con BLE 5). Es una placa pensada para operar años con una única batería, transmitiendo identificación, telemetría de ambiente y eventos de movimiento a gateways, móviles e infraestructura BLE.

Su gran diferencial es la combinación de sensores integrados con un **temporizador nano-power TPL5110**, que enciende la placa solo en el momento de transmitir y la apaga enseguida. Este ciclo de trabajo extremadamente corto reduce el consumo medio a niveles de microamperios, abriendo espacio para una autonomía muy larga con una pila de moneda o batería.

![Placa iW-Beacon](/img/iw-beacon/BOARD_VIEW.png)

---

## Destacados

- SoC Nordic `nRF52832` (Cortex-M4F, BLE 5)
- Acelerómetro de 3 ejes `LIS3DH`
- Sensor de temperatura y humedad `SHTC3`
- Temporizador nano-power `TPL5110` para un ciclo de trabajo ultrabajo
- Interruptor de carga por P-MOSFET `AO3401A`
- Cristales de 32 MHz y 32.768 kHz (RTC)
- Antena con red de adaptación y soporte NFC del nRF52832
- Depuración por SWD y consola UART

---

## Aplicaciones sugeridas

- beacons de identificación y proximidad (estilo iBeacon)
- rastreo y localización de activos
- sensores de ambiente inalámbricos (temperatura y humedad)
- detección de movimiento, vuelco y manipulación
- nodos BLE alimentados por batería de larga duración
- logística, retail y automatización de edificios

---

## Por qué llama la atención esta placa

La mayoría de los beacons solo resuelven la parte de radio. La iW-Beacon va más allá al integrar **sensado de ambiente y movimiento** en el mismo dispositivo y, sobre todo, al usar el `TPL5110` para cortar el consumo entre transmisiones. El resultado es un beacon que no solo anuncia presencia, sino que también lleva datos útiles de sensor, manteniendo una autonomía compatible con instalaciones del tipo "instala y olvídate".
