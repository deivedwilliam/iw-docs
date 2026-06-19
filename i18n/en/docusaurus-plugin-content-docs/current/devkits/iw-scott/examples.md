---
id: iw-scott-examples
title: Examples
sidebar_label: Examples
sidebar_position: 3
slug: /devkits/iw-scott/examples
---

# Examples

This board already ships with interesting examples inside the project itself. Below is a summary of the main cases found in the `iW-Scott` folder.

---

## Acc-CTRL-LED

An Arduino/PlatformIO-style example that uses the **LIS3DH** to read acceleration and translate spatial orientation into the color and brightness of the RGB LED.

### What it demonstrates

- reading the accelerometer over I2C
- a simple filter to smooth the readings
- mapping orientation to HSV on the NeoPixel
- power-hold logic to turn the board off

---

## JurassicGame

A more complete example that uses a display, a BLE joystick and the RGB LED to build a local mini-game.

### What it demonstrates

- using the display with `TFT_eSPI`
- a BLE joystick as the input device
- backlight control
- local graphical animation
- power management and embedded UX

---

## BLE-Gateware

An ESP-IDF project focused on a BLE gateway. The firmware performs passive scanning, parses advertisement packets and forwards sensor readings.

### What it demonstrates

- BLE scanning with ESP-IDF
- parsing manufacturer-specific advertisements
- a reading queue and sensor cache
- integration with a publishing runtime

---

## How to use this section

If the goal is to learn the board quickly:

1. start with `Acc-CTRL-LED` to validate the sensor and the LED
2. move on to `JurassicGame` to explore the local interface
3. use `BLE-Gateware` when you need a case closer to a connected product
