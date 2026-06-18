---
id: iw-beacon-hardware
title: Hardware
sidebar_label: Hardware
sidebar_position: 2
slug: /devkits/iw-beacon/hardware
---

import BoardViewer from '@site/src/components/BoardViewer';

# Hardware

Esta secao descreve os blocos principais da `iW-Beacon`, extraidos do esquema eletrico do projeto (`iW-iBeacon`).

---

## Visao geral da placa

A iW-Beacon organiza-se em quatro blocos: o SoC de radio `nRF52832`, os sensores em barramento I2C, o circuito de energia com timer nano-power e as interfaces de depuracao.

![Placa iW-Beacon](/img/iw-beacon/BOARD_VIEW.png)

### Modelo 3D

Arraste para girar, role para dar zoom.

<BoardViewer src="/models/iw-beacon-3d.glb" caption="Modelo 3D da iW-Beacon (preliminar)" />

---

## Processamento e radio — nRF52832

- SoC Nordic `nRF52832` (ARM Cortex-M4F de 32 bits)
- Bluetooth 5 Low Energy
- cristal principal de 32 MHz
- cristal de 32.768 kHz para o RTC e temporizacao de baixo consumo
- pinos de NFC disponiveis para pareamento por toque
- conversor DC/DC interno (linhas DCC) para eficiencia de RF

A saida de antena passa por uma rede de casamento (indutores de 15 nH e 10 uH) ate a antena da placa.

---

## Sensores integrados

### Acelerometro — LIS3DH

Sensor inercial de 3 eixos de ultrabaixo consumo, ligado ao SoC com interface I2C/SPI e linhas de interrupcao (`INT1`, `INT2`). Util para acordar a placa por movimento, detectar tombamento, orientacao e adulteracao.

### Temperatura e umidade — SHTC3

Sensor `SHTC3` (Sensirion) em barramento I2C, com pull-ups de 2,2 k. Fornece leitura de temperatura e umidade relativa para telemetria de ambiente diretamente no payload do beacon.

> Os dois sensores compartilham o mesmo barramento I2C do nRF52832, o que simplifica o firmware e o roteamento.

---

## Gestao de energia (nano-power)

O circuito de energia foi desenhado para autonomia maxima.

- bateria `BT1` (celula tipo moeda / pilha) como fonte primaria
- timer nano-power `TPL5110` (`U4`) que define o intervalo de wakeup
- chave de carga por P-MOSFET `AO3401A` (`Q2`), controlada pelo timer
- jumper `JP1` para selecao do modo de alimentacao
- banco de capacitores (47 uF + 4,7 uF + 100 nF) para estabilizar os picos de transmissao

### Como funciona o ciclo

1. O `TPL5110` mantem a placa desligada durante o intervalo programado.
2. No fim do intervalo, ele aciona o MOSFET e energiza o `nRF52832`.
3. O SoC acorda, le os sensores e transmite o pacote BLE.
4. O firmware sinaliza `DONE` ao timer, que corta a alimentacao novamente.

Esse esquema reduz o consumo medio a microamperes, ja que o SoC so consome corrente durante a janela curta de transmissao.

---

## Interfaces e depuracao

- conector SWD (`J1`, 3 vias: `SWDCLK`, `SWDIO`, `GND`) para gravacao e debug
- linhas de console `UART_TX` / `UART_RX`
- barramento I2C compartilhado pelos sensores
- botao/linha de `RESET`

---

## Caracteristicas eletricas (tipicas)

| Parametro | Valor | Observacao |
| --- | --- | --- |
| Tensao logica | 3,3 V | trilho do sistema |
| Bateria | celula unica | tipo moeda / pilha (`BT1`) |
| Chave de carga | AO3401A | P-MOSFET controlado pelo TPL5110 |
| Consumo medio | faixa de microamperes | dominado pelo ciclo do TPL5110 |
| Cristais | 32 MHz + 32.768 kHz | radio + RTC |

> Os valores acima sao derivados dos componentes do esquema. O consumo real depende do intervalo de transmissao, da potencia de RF e dos sensores ativos.

---

## Esquematico online

Visualize o projeto de hardware diretamente no navegador:

<iframe
  src="https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-BEACON%2Ftree%2Fmain%2FHARDWARE"
  width="100%"
  height="700px"
  style={{border: '1px solid #333', borderRadius: '12px'}}
></iframe>

[Abrir no KiCanvas](https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-BEACON%2Ftree%2Fmain%2FHARDWARE)

---

## Boas praticas

- programe o intervalo do `TPL5110` conforme o compromisso entre frescor do dado e autonomia
- use o acelerometro como gatilho para transmissoes orientadas a evento
- mantenha o payload enxuto para encurtar a janela de radio ativa
- valide o casamento de antena antes de fechar o gabinete
