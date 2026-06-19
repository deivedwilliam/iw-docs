---
id: iw-espmi-ind-firmware
title: Firmware
sidebar_label: Firmware
sidebar_position: 3
slug: /devkits/iw-espmi-ind/firmware
---

# Firmware

The `iW-ESPMI-IND` was designed to speed up industrial projects with a local interface. The most natural firmware path revolves around **Arduino IDE** and **ESP-IDF**.

---

## Supported stacks

- **Arduino IDE**
  Good for proofs of concept, peripheral testing and quick integration.
- **ESP-IDF**
  The better choice when the project needs more control, multitasking, networking and product-level organization.

---

## Common application blocks

### Graphical interface

- HDMI as the main output
- integration with graphical libraries such as LVGL
- screens for HMI, dashboards and local configuration

### Industrial communication

- RS485 for Modbus RTU
- reading and writing registers
- integration with PLCs, drives and instruments

### Local data

- SD Card for logs, recipes, assets and history

---

## Suggested flow for new projects

1. validate HDMI and the chosen display
2. bring up RS485 communication with a simple Modbus test
3. structure configurations and logs on the SD Card
4. organize tasks and services in the final firmware

---

## Practical recommendation

If the goal is a product with a robust interface and industrial connectivity, it's worth starting directly with ESP-IDF. If the goal is to validate a concept quickly, Arduino IDE can be a good first step.
