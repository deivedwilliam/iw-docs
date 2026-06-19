---
id: iw-starter-kit-hardware
title: Hardware
sidebar_label: Hardware
sidebar_position: 2
slug: /devkits/iw-starter-kit/hardware
---

import BoardViewer from '@site/src/components/BoardViewer';

# Hardware

Esta seção detalha os aspectos físicos e elétricos da `iW-ST-StarterKit` (Rev. RV01), a partir do datasheet do produto.

---

## Visão geral da placa

A iW-ST-StarterKit é uma devkit compacta e de baixo custo construída em torno do `STM32F030F4P6` (ARM Cortex-M0 a até 48 MHz). Ela integra tudo o que é necessário para programar e depurar sem hardware externo: USB-C, ponte USB-Serial, regulador on-board, proteção ESD e botões dedicados.

![Placa iW-ST Starter Kit](/img/iw-starter-kit/BOARD_VIEW.png)

### Modelo 3D

Arraste para girar, role para dar zoom.

<BoardViewer src="/models/iw-starter-kit-3d.glb" caption="Modelo 3D da iW-ST Starter Kit (preliminar)" />

---

## Características principais

- MCU `STM32F030F4P6` (Cortex-M0, até 48 MHz, TSSOP-20)
- 16 KB de Flash + 4 KB de SRAM
- conector USB-C com proteção ESD `USBLC6-2P6`
- ponte serial `CH343P` (USB para UART / USART1)
- regulador LDO `AP2112K-3.3` (5 V para 3,3 V, até 600 mA)
- cristal externo de 20 MHz (capacitores de carga 22 pF)
- programação por SWD (PA13/PA14) e bootloader serial via BOOT0
- até 15 GPIOs multifuncionais nos conectores de expansão

---

## Especificações elétricas

| Parâmetro | Mín. | Típ. | Máx. | Unidade / Observação |
| --- | --- | --- | --- | --- |
| Tensão de entrada USB (V_BUS) | 4,5 | 5,0 | 5,5 | V — conector USB-C |
| Tensão lógica do sistema | — | 3,3 | — | V — gerada pelo AP2112K |
| Corrente máx. do regulador | — | — | 600 | mA — AP2112K-3.3 |
| Frequência de clock da CPU | — | — | 48 | MHz |
| Frequência do cristal externo | — | 20 | — | MHz |
| Tensão de operação do MCU | 2,4 | 3,3 | 3,6 | V — STM32F030F4 |
| Consumo típico (Run, 48 MHz) | — | ~12 | — | mA |
| Consumo em Stop | — | ~5 | — | uA |
| Temperatura de operação | -40 | — | +85 | °C |

> Os valores de consumo são típicos do STM32F030F4 e variam conforme firmware, clock e periféricos ativos.

---

## Gestão de energia

A alimentação é feita pelo USB-C (5 V). O LDO `AP2112K-3.3` converte os 5 V em 3,3 V estáveis para o MCU e periféricos. Os trilhos de 3,3 V e 5 V também ficam disponíveis nos conectores de expansão.

| Recurso | Descrição |
| --- | --- |
| Entrada de energia | USB-C (5 V via V_BUS) |
| Regulação | LDO AP2112K-3.3 — saída 3,3 V até 600 mA |
| Desacoplamento | capacitores de 100 nF / 10 uF nos trilhos de 5 V e 3,3 V |
| Filtragem analógica | ferrite bead (600R) + 100 nF no pino VDDA do MCU |
| Saídas disponíveis | pinos 3V3 e 5V nos conectores de expansão |

---

## Periféricos e interfaces

### Ponte USB-Serial — CH343P

Conversor USB para UART que liga a `USART1` do STM32 (PA9/TX e PA10/RX) diretamente ao computador via USB-C, permitindo comunicação serial, log de depuração e gravação via bootloader sem adaptador externo.

### Depuração — SWD

Os pinos `PA13` (SWDIO) e `PA14` (SWCLK) expõem a interface Serial Wire Debug, para programação e depuração com ferramentas como ST-Link. Disponíveis no conector de expansão.

### Proteção ESD — USBLC6-2P6

Componente dedicado de proteção contra descargas eletrostáticas nas linhas D+/D- do USB-C.

---

## Mapa de pinos / conectores de expansão

A placa expõe os GPIOs do STM32F030F4P6 em dois conectores laterais. A maioria dos pinos é multifuncional (GPIO, ADC, I2C, SPI, USART, timers e PWM).

| Pino | Sinal | Funções alternativas | Observações |
| --- | --- | --- | --- |
| 1 | PA0 | ADC_IN0, USART2_CTS, TIM2_CH1 | entrada analógica / wakeup |
| 2 | PA1 | ADC_IN1, USART2_RTS, TIM2_CH2 | entrada analógica |
| 3 | PA2 | ADC_IN2, USART2_TX, TIM2_CH3 | entrada analógica / UART |
| 4 | PA3 | ADC_IN3, USART2_RX, TIM2_CH4 | entrada analógica / UART |
| 5 | PA4 | ADC_IN4, SPI1_NSS | entrada analógica / SPI |
| 6 | PA5 | ADC_IN5, SPI1_SCK | entrada analógica / SPI |
| 7 | PA6 | ADC_IN6, SPI1_MISO, TIM3_CH1 | entrada analógica / SPI |
| 8 | PA7 | ADC_IN7, SPI1_MOSI, TIM3_CH2 | entrada analógica / SPI |
| 9 | PB1 | ADC_IN9, TIM3_CH4 | entrada analógica |
| 10 | PA9 | USART1_TX, I2C1_SCL | UART1 TX -> CH343P |
| 11 | PA10 | USART1_RX, I2C1_SDA | UART1 RX -> CH343P |
| 12 | PA13 | SWDIO | depuração SWD |
| 13 | PA14 | SWCLK | depuração SWD |
| 14 | 3V3 | — | saída regulada 3,3 V |
| 15 | 5V | — | entrada/saída 5 V (V_BUS do USB-C) |
| 16 | GND | — | referência / terra |

> `PA13/PA14` são, por padrão, a interface SWD — reconfigure-os como GPIO apenas se não usar depuração. A numeração segue a serigrafia dos conectores laterais.

---

## Botões e controles

| Botão | Sinal | Função |
| --- | --- | --- |
| NRST | NRST | reinicia o microcontrolador STM32F030 |
| BOOT0 | BOOT0 | entra no bootloader do sistema (gravação via serial) ao reiniciar |

---

## Esquemático online

Visualize o projeto de hardware diretamente no navegador:

<iframe
  src="https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-ST-STARTER-KIT%2Ftree%2Fmain%2FHARDWARE"
  width="100%"
  height="700px"
  style={{border: '1px solid #333', borderRadius: '12px'}}
></iframe>

---

## Informações mecânicas e ambientais

| Item | Especificação |
| --- | --- |
| Conector principal | USB-C (alimentação + dados) |
| Conectores de expansão | headers laterais, passo 2,54 mm |
| Microcontrolador | STM32F030F4P6 — TSSOP-20 |
| Temperatura de operação | -40 °C a +85 °C |
| Tensão lógica | 3,3 V |
