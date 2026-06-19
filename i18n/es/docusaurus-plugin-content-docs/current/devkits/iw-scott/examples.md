---
id: iw-scott-examples
title: Examples
sidebar_label: Examples
sidebar_position: 3
slug: /devkits/iw-scott/examples
---

# Examples

Esta placa ya incluye ejemplos interesantes dentro del propio proyecto. A continuación, un resumen de los principales casos que encontramos en la carpeta de la `iW-Scott`.

---

## Acc-CTRL-LED

Ejemplo en estilo Arduino/PlatformIO que usa el **LIS3DH** para leer la aceleración y traducir la orientación espacial en color y brillo del LED RGB.

### Qué demuestra

- lectura del acelerómetro por I2C
- filtro simple para suavizar las medidas
- mapeo de orientación a HSV en el NeoPixel
- lógica de power hold para apagar la placa

---

## JurassicGame

Ejemplo más completo que usa pantalla, joystick BLE y LED RGB para montar un minijuego local.

### Qué demuestra

- uso de la pantalla con `TFT_eSPI`
- joystick BLE como dispositivo de entrada
- control del backlight
- animación gráfica local
- gestión de energía y UX embebida

---

## BLE-Gateware

Proyecto en ESP-IDF enfocado en un gateway BLE. El firmware hace un escaneo pasivo, interpreta paquetes de advertisement y reenvía lecturas de sensores.

### Qué demuestra

- escaneo BLE con ESP-IDF
- parseo de advertisement manufacturer specific
- cola de lecturas y caché de sensores
- integración con un runtime de publicación

---

## Cómo usar esta sección

Si la idea es aprender la placa rápido:

1. empieza por `Acc-CTRL-LED` para validar el sensor y el LED
2. avanza a `JurassicGame` para explorar la interfaz local
3. usa `BLE-Gateware` cuando necesites un caso más cercano a un producto conectado
