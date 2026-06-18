---
id: iw-starter-kit-hardware
title: Hardware
sidebar_label: Hardware
sidebar_position: 2
slug: /devkits/iw-starter-kit/hardware
---

import BoardViewer from '@site/src/components/BoardViewer';

# Hardware

Esta secao detalha os aspectos fisicos e eletricos da `iW-ST-StarterKit` (Rev. RV01), a partir do datasheet do produto.

---

## Visao geral da placa

A iW-ST-StarterKit e uma devkit compacta e de baixo custo construida em torno do `STM32F030F4P6` (ARM Cortex-M0 a ate 48 MHz). Ela integra tudo o que e necessario para programar e depurar sem hardware externo: USB-C, ponte USB-Serial, regulador on-board, protecao ESD e botoes dedicados.

![Placa iW-ST Starter Kit](/img/iw-starter-kit/BOARD_VIEW.png)

### Modelo 3D

Arraste para girar, role para dar zoom.

<BoardViewer src="/models/iw-starter-kit-3d.glb" caption="Modelo 3D da iW-ST Starter Kit (preliminar)" />

---

## Caracteristicas principais

- MCU `STM32F030F4P6` (Cortex-M0, ate 48 MHz, TSSOP-20)
- 16 KB de Flash + 4 KB de SRAM
- conector USB-C com protecao ESD `USBLC6-2P6`
- ponte serial `CH343P` (USB para UART / USART1)
- regulador LDO `AP2112K-3.3` (5 V para 3,3 V, ate 600 mA)
- cristal externo de 20 MHz (capacitores de carga 22 pF)
- programacao por SWD (PA13/PA14) e bootloader serial via BOOT0
- ate 15 GPIOs multifuncionais nos conectores de expansao

---

## Especificacoes eletricas

| Parametro | Min. | Tip. | Max. | Unidade / Observacao |
| --- | --- | --- | --- | --- |
| Tensao de entrada USB (V_BUS) | 4,5 | 5,0 | 5,5 | V — conector USB-C |
| Tensao logica do sistema | — | 3,3 | — | V — gerada pelo AP2112K |
| Corrente max. do regulador | — | — | 600 | mA — AP2112K-3.3 |
| Frequencia de clock da CPU | — | — | 48 | MHz |
| Frequencia do cristal externo | — | 20 | — | MHz |
| Tensao de operacao do MCU | 2,4 | 3,3 | 3,6 | V — STM32F030F4 |
| Consumo tipico (Run, 48 MHz) | — | ~12 | — | mA |
| Consumo em Stop | — | ~5 | — | uA |
| Temperatura de operacao | -40 | — | +85 | °C |

> Os valores de consumo sao tipicos do STM32F030F4 e variam conforme firmware, clock e perifericos ativos.

---

## Gestao de energia

A alimentacao e feita pelo USB-C (5 V). O LDO `AP2112K-3.3` converte os 5 V em 3,3 V estaveis para o MCU e perifericos. Os trilhos de 3,3 V e 5 V tambem ficam disponiveis nos conectores de expansao.

| Recurso | Descricao |
| --- | --- |
| Entrada de energia | USB-C (5 V via V_BUS) |
| Regulacao | LDO AP2112K-3.3 — saida 3,3 V ate 600 mA |
| Desacoplamento | capacitores de 100 nF / 10 uF nos trilhos de 5 V e 3,3 V |
| Filtragem analogica | ferrite bead (600R) + 100 nF no pino VDDA do MCU |
| Saidas disponiveis | pinos 3V3 e 5V nos conectores de expansao |

---

## Perifericos e interfaces

### Ponte USB-Serial — CH343P

Conversor USB para UART que liga a `USART1` do STM32 (PA9/TX e PA10/RX) diretamente ao computador via USB-C, permitindo comunicacao serial, log de depuracao e gravacao via bootloader sem adaptador externo.

### Depuracao — SWD

Os pinos `PA13` (SWDIO) e `PA14` (SWCLK) expoem a interface Serial Wire Debug, para programacao e depuracao com ferramentas como ST-Link. Disponiveis no conector de expansao.

### Protecao ESD — USBLC6-2P6

Componente dedicado de protecao contra descargas eletrostaticas nas linhas D+/D- do USB-C.

---

## Mapa de pinos / conectores de expansao

A placa expoe os GPIOs do STM32F030F4P6 em dois conectores laterais. A maioria dos pinos e multifuncional (GPIO, ADC, I2C, SPI, USART, timers e PWM).

| Pino | Sinal | Funcoes alternativas | Observacoes |
| --- | --- | --- | --- |
| 1 | PA0 | ADC_IN0, USART2_CTS, TIM2_CH1 | entrada analogica / wakeup |
| 2 | PA1 | ADC_IN1, USART2_RTS, TIM2_CH2 | entrada analogica |
| 3 | PA2 | ADC_IN2, USART2_TX, TIM2_CH3 | entrada analogica / UART |
| 4 | PA3 | ADC_IN3, USART2_RX, TIM2_CH4 | entrada analogica / UART |
| 5 | PA4 | ADC_IN4, SPI1_NSS | entrada analogica / SPI |
| 6 | PA5 | ADC_IN5, SPI1_SCK | entrada analogica / SPI |
| 7 | PA6 | ADC_IN6, SPI1_MISO, TIM3_CH1 | entrada analogica / SPI |
| 8 | PA7 | ADC_IN7, SPI1_MOSI, TIM3_CH2 | entrada analogica / SPI |
| 9 | PB1 | ADC_IN9, TIM3_CH4 | entrada analogica |
| 10 | PA9 | USART1_TX, I2C1_SCL | UART1 TX -> CH343P |
| 11 | PA10 | USART1_RX, I2C1_SDA | UART1 RX -> CH343P |
| 12 | PA13 | SWDIO | depuracao SWD |
| 13 | PA14 | SWCLK | depuracao SWD |
| 14 | 3V3 | — | saida regulada 3,3 V |
| 15 | 5V | — | entrada/saida 5 V (V_BUS do USB-C) |
| 16 | GND | — | referencia / terra |

> `PA13/PA14` sao, por padrao, a interface SWD — reconfigure-os como GPIO apenas se nao usar depuracao. A numeracao segue a serigrafia dos conectores laterais.

---

## Botoes e controles

| Botao | Sinal | Funcao |
| --- | --- | --- |
| NRST | NRST | reinicia o microcontrolador STM32F030 |
| BOOT0 | BOOT0 | entra no bootloader do sistema (gravacao via serial) ao reiniciar |

---

## Esquematico online

Visualize o projeto de hardware diretamente no navegador:

<iframe
  src="https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-ST-STARTER-KIT%2Ftree%2Fmain%2FHARDWARE"
  width="100%"
  height="700px"
  style={{border: '1px solid #333', borderRadius: '12px'}}
></iframe>

[Abrir no KiCanvas](https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-ST-STARTER-KIT%2Ftree%2Fmain%2FHARDWARE)

---

## Informacoes mecanicas e ambientais

| Item | Especificacao |
| --- | --- |
| Conector principal | USB-C (alimentacao + dados) |
| Conectores de expansao | headers laterais, passo 2,54 mm |
| Microcontrolador | STM32F030F4P6 — TSSOP-20 |
| Temperatura de operacao | -40 °C a +85 °C |
| Tensao logica | 3,3 V |
