---
id: iw-espmi-ind-firmware
title: Firmware
sidebar_label: Firmware
sidebar_position: 3
slug: /devkits/iw-espmi-ind/firmware
---

# Firmware

A `iW-ESPMI-IND` foi pensada para acelerar projetos industriais com interface local. O caminho mais natural de firmware gira em torno de **Arduino IDE** e **ESP-IDF**.

---

## Stacks suportadas

- **Arduino IDE**
  Bom para provas de conceito, testes de perifericos e integracao rapida.
- **ESP-IDF**
  Melhor escolha quando o projeto precisa de mais controle, multitarefa, rede e organizacao de produto.

---

## Blocos de aplicacao comuns

### Interface grafica

- HDMI como saida principal
- integracao com bibliotecas graficas como LVGL
- telas para HMI, dashboards e configuracao local

### Comunicacao industrial

- RS485 para Modbus RTU
- leitura e escrita de registradores
- integracao com CLPs, inversores e instrumentos

### Dados locais

- SD Card para logs, receitas, assets e historicos

---

## Fluxo sugerido para novos projetos

1. validar HDMI e display escolhido
2. subir comunicacao RS485 com um teste simples de Modbus
3. estruturar configuracoes e logs em SD Card
4. organizar tarefas e servicos no firmware final

---

## Recomendacao pratica

Se o objetivo for um produto com interface robusta e conectividade industrial, vale iniciar diretamente em ESP-IDF. Se a meta for validar conceito com velocidade, Arduino IDE pode ser um bom primeiro passo.
