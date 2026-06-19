---
id: iw-scott-examples
title: Examples
sidebar_label: Examples
sidebar_position: 3
slug: /devkits/iw-scott/examples
---

# Examples

Esta placa já possui exemplos interessantes dentro do próprio projeto. Abaixo esta um resumo dos principais casos que encontramos na pasta da `iW-Scott`.

---

## Acc-CTRL-LED

Exemplo em estilo Arduino/PlatformIO que usa o **LIS3DH** para ler aceleração e traduzir orientação espacial em cor e brilho do LED RGB.

### O que demonstra

- leitura do acelerômetro por I2C
- filtro simples para suavizar medidas
- mapeamento de orientação para HSV no NeoPixel
- lógica de power hold para desligamento da placa

---

## JurassicGame

Exemplo mais completo que usa display, joystick BLE e LED RGB para montar um minijogo local.

### O que demonstra

- uso do display com `TFT_eSPI`
- joystick BLE como dispositivo de entrada
- controle de backlight
- animação gráfica local
- gerência de energia e UX embarcada

---

## BLE-Gateware

Projeto em ESP-IDF focado em gateway BLE. O firmware faz varredura passiva, interpreta pacotes de advertisement e encaminha leituras de sensores.

### O que demonstra

- varredura BLE com ESP-IDF
- parse de advertisement manufacturer specific
- fila de leituras e cache de sensores
- integração com runtime de publicação

---

## Como usar esta seção

Se a ideia for aprender a placa rápido:

1. comece pelo `Acc-CTRL-LED` para validar sensor e LED
2. avance para `JurassicGame` para explorar interface local
3. use `BLE-Gateware` quando precisar de um caso mais próximo de produto conectado
