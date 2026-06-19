---
id: iw-espmi-ind-firmware
title: Firmware
sidebar_label: Firmware
sidebar_position: 3
slug: /devkits/iw-espmi-ind/firmware
---

# Firmware

La `iW-ESPMI-IND` fue pensada para acelerar proyectos industriales con interfaz local. El camino de firmware más natural gira en torno a **Arduino IDE** y **ESP-IDF**.

---

## Stacks compatibles

- **Arduino IDE**
  Bueno para pruebas de concepto, pruebas de periféricos e integración rápida.
- **ESP-IDF**
  La mejor opción cuando el proyecto necesita más control, multitarea, red y organización de producto.

---

## Bloques de aplicación comunes

### Interfaz gráfica

- HDMI como salida principal
- integración con bibliotecas gráficas como LVGL
- pantallas para HMI, dashboards y configuración local

### Comunicación industrial

- RS485 para Modbus RTU
- lectura y escritura de registros
- integración con PLC, variadores e instrumentos

### Datos locales

- tarjeta SD para logs, recetas, assets e históricos

---

## Flujo sugerido para nuevos proyectos

1. validar HDMI y el display elegido
2. levantar la comunicación RS485 con una prueba simple de Modbus
3. estructurar configuraciones y logs en la tarjeta SD
4. organizar tareas y servicios en el firmware final

---

## Recomendación práctica

Si el objetivo es un producto con interfaz robusta y conectividad industrial, conviene empezar directamente con ESP-IDF. Si la meta es validar un concepto con rapidez, Arduino IDE puede ser un buen primer paso.
