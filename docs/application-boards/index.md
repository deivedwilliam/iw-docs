---
id: application-boards
title: Application Boards
slug: /application-boards
sidebar_position: 1
---

# Application Boards

As Application Boards (linha `iW-APPLICATION`) são placas de aplicação que se encaixam nos conectores de expansão da [iW-CORE-BOARD](/docs/devkits/iw-core-board) (J4 e J5) e definem a função final do produto. A core board cuida do processamento, da interface gráfica e da conectividade; a application board fornece os sensores, atuadores e o condicionamento de sinal específicos de cada aplicação.

---

## Como funciona

A iW-CORE-BOARD expõe os GPIOs do STM32H7 em dois conectores `iW-APPLICATION` (J4 e J5), incluindo barramentos I2C, SPI, UART e entradas analógicas (ADC). Uma application board encaixa nesses conectores e adiciona a parte específica do produto — por exemplo, um sensor com condicionamento de sinal para um monitor de temperatura, lido via ADC pela core board.

Essa arquitetura modular permite reaproveitar a mesma base de processamento e interface em vários produtos, trocando apenas a placa de aplicação.

---

## Catálogo atual

Esta seção está em construção. Conforme as placas de aplicação forem documentadas, elas aparecerão aqui com visão geral, hardware (pinagem dos conectores `iW-APPLICATION`), exemplos e downloads, no mesmo padrão das demais seções.

---

## Materiais relacionados

- [iW-CORE-BOARD](/docs/devkits/iw-core-board): a placa-base que recebe as application boards.
- [Hardware da iW-CORE-BOARD](/docs/devkits/iw-core-board/hardware): pinagem completa dos conectores de expansão J4 e J5.
