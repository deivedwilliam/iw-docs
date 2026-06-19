---
id: application-boards
title: Application Boards
slug: /application-boards
sidebar_position: 1
---

# Application Boards

Application Boards (the `iW-APPLICATION` line) are application-specific boards that plug into the expansion connectors of the [iW-CORE-BOARD](/docs/devkits/iw-core-board) (J4 and J5) and define the final function of the product. The core board handles processing, the graphical interface and connectivity; the application board provides the sensors, actuators and signal conditioning specific to each application.

---

## How it works

The iW-CORE-BOARD exposes the STM32H7 GPIOs on two `iW-APPLICATION` connectors (J4 and J5), including I2C, SPI and UART buses and analog inputs (ADC). An application board plugs into these connectors and adds the product-specific part — for example, a sensor with signal conditioning for a temperature monitor, read via ADC by the core board.

This modular architecture lets you reuse the same processing and interface base across several products, swapping only the application board.

---

## Current catalog

This section is a work in progress. As the application boards are documented, they will appear here with an overview, hardware (pinout of the `iW-APPLICATION` connectors), examples and downloads, following the same pattern as the other sections.

---

## Related materials

- [iW-CORE-BOARD](/docs/devkits/iw-core-board): the base board that hosts the application boards.
- [iW-CORE-BOARD hardware](/docs/devkits/iw-core-board/hardware): full pinout of the J4 and J5 expansion connectors.
