---
id: application-boards
title: Application Boards
slug: /application-boards
sidebar_position: 1
---

# Application Boards

Las Application Boards (la línea `iW-APPLICATION`) son placas de aplicación que se encajan en los conectores de expansión de la [iW-CORE-BOARD](/docs/devkits/iw-core-board) (J4 y J5) y definen la función final del producto. La core board se encarga del procesamiento, la interfaz gráfica y la conectividad; la application board aporta los sensores, actuadores y el acondicionamiento de señal específicos de cada aplicación.

---

## Cómo funciona

La iW-CORE-BOARD expone los GPIO del STM32H7 en dos conectores `iW-APPLICATION` (J4 y J5), incluyendo buses I2C, SPI y UART y entradas analógicas (ADC). Una application board se encaja en estos conectores y añade la parte específica del producto — por ejemplo, un sensor con acondicionamiento de señal para un monitor de temperatura, leído por ADC desde la core board.

Esta arquitectura modular permite reutilizar la misma base de procesamiento e interfaz en varios productos, cambiando solo la placa de aplicación.

---

## Catálogo actual

Esta sección está en construcción. A medida que se documenten las placas de aplicación, aparecerán aquí con visión general, hardware (pinout de los conectores `iW-APPLICATION`), ejemplos y descargas, siguiendo el mismo patrón que las demás secciones.

---

## Materiales relacionados

- [iW-CORE-BOARD](/docs/devkits/iw-core-board): la placa base que recibe las application boards.
- [Hardware de la iW-CORE-BOARD](/docs/devkits/iw-core-board/hardware): pinout completo de los conectores de expansión J4 y J5.
