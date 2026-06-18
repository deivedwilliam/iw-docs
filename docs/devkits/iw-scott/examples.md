---
id: iw-scott-examples
title: Examples
sidebar_label: Examples
sidebar_position: 3
slug: /devkits/iw-scott/examples
---

# Examples

Esta placa ja possui exemplos interessantes dentro do proprio projeto. Abaixo esta um resumo dos principais casos que encontramos na pasta da `iW-Scott`.

---

## Acc-CTRL-LED

Exemplo em estilo Arduino/PlatformIO que usa o **LIS3DH** para ler aceleracao e traduzir orientacao espacial em cor e brilho do LED RGB.

### O que demonstra

- leitura do acelerometro por I2C
- filtro simples para suavizar medidas
- mapeamento de orientacao para HSV no NeoPixel
- logica de power hold para desligamento da placa

---

## JurassicGame

Exemplo mais completo que usa display, joystick BLE e LED RGB para montar um minijogo local.

### O que demonstra

- uso do display com `TFT_eSPI`
- joystick BLE como dispositivo de entrada
- controle de backlight
- animacao grafica local
- gerencia de energia e UX embarcada

---

## BLE-Gateware

Projeto em ESP-IDF focado em gateway BLE. O firmware faz varredura passiva, interpreta pacotes de advertisement e encaminha leituras de sensores.

### O que demonstra

- varredura BLE com ESP-IDF
- parse de advertisement manufacturer specific
- fila de leituras e cache de sensores
- integracao com runtime de publicacao

---

## Como usar esta secao

Se a ideia for aprender a placa rapido:

1. comece pelo `Acc-CTRL-LED` para validar sensor e LED
2. avance para `JurassicGame` para explorar interface local
3. use `BLE-Gateware` quando precisar de um caso mais proximo de produto conectado
