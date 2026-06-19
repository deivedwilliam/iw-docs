---
id: iw-scott-hardware
title: Hardware
sidebar_label: Hardware
sidebar_position: 2
slug: /devkits/iw-scott/hardware
---

import BoardViewer from '@site/src/components/BoardViewer';

# Hardware

Esta página detalha os blocos de hardware da `iW-Scott` (Rev. RV01), a partir do datasheet do produto.

![Placa iW-Scott](/img/iw-scott/BOARD_VIEW.png)

### Modelo 3D

Arraste para girar, role para dar zoom.

<BoardViewer src="/models/iw-scott-3d.glb" caption="Modelo 3D da iW-Scott (preliminar)" />

---

## Visão geral da placa

A iW-Scott é uma Development Board Tools compacta e de baixo consumo, construída em torno do SoC `ESP32-C3` (RISC-V, single-core, até 160 MHz) com Wi-Fi e Bluetooth 5 LE. Ela reúne gestão de energia inteligente, acelerômetro, LED RGB endereçável e memória flash externa em um único módulo.

---

## Características principais

- SoC `ESP32-C3` (RISC-V 32 bits, até 160 MHz)
- memória interna do SoC: 400 KB SRAM, 384 KB ROM, 4 MB flash
- flash externa QSPI Winbond `W25Q` — 8 MB ou 16 MB
- Wi-Fi 802.11 b/g/n 2,4 GHz + Bluetooth 5 LE / Mesh
- acelerômetro de 3 eixos `LIS3DH` (I2C/SPI)
- LED RGB endereçável `WS2812B` (NeoPixel)
- PMIC `PT1502` (carregador Li-ion, buck e LDOs)
- antena cerâmica `AN9520-245` e conector U.FL
- cristal de 40 MHz (capacitores de carga 22 pF)
- USB-C com proteção ESD `USBLC6`

---

## Especificações elétricas

| Parâmetro | Mín. | Típ. | Máx. | Unidade / Observação |
| --- | --- | --- | --- | --- |
| Tensão de entrada USB (V_BUS) | 4,5 | 5,0 | 5,5 | V — conector USB-C |
| Tensão de bateria (Li-ion) | 3,0 | 3,7 | 4,2 | V — célula única |
| Tensão lógica do sistema | — | 3,3 | — | V — gerada pelo PT1502 |
| Corrente de carga da bateria | — | — | ~1,0 | A — programável (PT1502) |
| Frequência de clock da CPU | — | — | 160 | MHz |
| Frequência do cristal | — | 40 | — | MHz |
| Consumo modo ativo (Wi-Fi TX) | — | ~280 | — | mA (pico de transmissão) |
| Consumo em modem-sleep | — | ~20 | — | mA |
| Consumo em deep-sleep | — | ~5 | — | uA |
| Temperatura de operação | -40 | — | +85 | °C |

> Os valores de consumo são típicos do ESP32-C3 e variam conforme firmware, potência de RF e periféricos ativos. A corrente de carga depende da configuração de resistores do PT1502 e da capacidade da bateria.

---

## Gestão de energia (PMIC PT1502)

O circuito de energia gira em torno do `PT1502`, que integra carregador Li-ion, conversor buck (indutor de 2,2 uH) e LDOs. Ele seleciona automaticamente entre USB (5 V) e bateria, gerando o trilho de 3,3 V do sistema.

| Recurso | Descrição |
| --- | --- |
| Entradas de energia | USB-C (5 V via V_BUS) e bateria Li-ion |
| Carregamento | carregador Li-ion integrado, com status por LED (CHG_STAT) |
| Saídas | buck regulado + LDOs para 3,3 V e trilhos auxiliares |
| Monitoramento | divisor BAT_LEVEL para ler a tensão da bateria via ADC; sinais BAT_LOW / CHG_PROG |
| Controle | sinais PLAY_ON, uP_RESET e PWR_HOLD para liga/desliga |

---

## Periféricos integrados

### Acelerômetro — LIS3DH

Sensor inercial de 3 eixos de ultrabaixo consumo (I2C e SPI), ideal para detecção de movimento, orientação, gestos e TinyML. Conectado as linhas SDA/SCL/CS do ESP32-C3.

### LED RGB — WS2812B

LED endereçável (NeoPixel) controlado por um único pino de dados (linha DIN), acionado pela linha `U0TXD/GPIO21`. Permite sinalização de status colorida via protocolo serial de 1 fio.

### Memória flash QSPI — W25Q

Flash serial externa Winbond, em 8 MB (`W25Q64`) ou 16 MB (`W25Q128`), por barramento QSPI (CS/CLK/IO0-IO3) para firmware, modelos de ML, sistemas de arquivos e dados.

---

## Mapa de pinos / conector de expansão (2x9)

A placa expõe os GPIOs do ESP32-C3 em um conector duplo de 18 vias. A maioria dos pinos é multifuncional (GPIO, ADC1, I2C, SPI, UART e PWM por software).

| Pino | Sinal | Funções alternativas | Observações |
| --- | --- | --- | --- |
| 1 | GPIO0 | ADC1_CH0, XTAL_32K_P | entrada analógica |
| 2 | GPIO1 | ADC1_CH1, XTAL_32K_N | entrada analógica |
| 3 | GPIO2 | ADC1_CH2 | strapping / pull-up de boot (4,7 k) |
| 4 | GPIO3 | ADC1_CH3 | entrada analógica |
| 5 | GPIO4 | ADC1_CH4, MTMS | JTAG / ADC |
| 6 | GPIO5 | ADC2_CH0, MTDI | JTAG / ADC |
| 7 | GPIO6 | MTCK | GPIO / JTAG |
| 8 | GPIO7 | MTDO | GPIO / JTAG |
| 9 | GPIO8 | — | strapping; pull-up de boot (4,7 k) |
| 10 | GPIO9 | BOOT | strapping; botão BOOT (download) |
| 11 | GPIO10 | PLAY_ON | controle de ligamento |
| 12 | GPIO20 | U0RXD | UART0 (console) — RX |
| 13 | GPIO21 | U0TXD | UART0 (console) — TX; -> LED WS2812B |
| 14 | 3V3 | — | saída regulada 3,3 V |
| 15 | GND | — | referência / terra |
| 16 | GND | — | referência / terra |
| 17 | BATT | — | tensão da bateria Li-ion (3,0-4,2 V) |
| 18 | 5V | — | entrada/saída 5 V (V_BUS do USB-C) |

> `GPIO2`, `GPIO8` e `GPIO9` são pinos de strapping — observe as restrições de nível durante o boot. A numeração segue o conector de expansão 2x9 (J4).

---

## Botões e controles

| Botão | Sinal | Função |
| --- | --- | --- |
| RESET (EN) | ESP_EN | reinicia o ESP32-C3 (via transistor BC807) |
| BOOT | GPIO9 | entra em modo de download / programação ao reiniciar |
| PLAY / ON | GPIO10 / PLAY_ON | ligamento e controle de energia (PT1502) |

---

## Conectividade

- Wi-Fi para integração em rede local e IoT
- Bluetooth 5 LE / Mesh para apps, beacons e periféricos BLE
- antena cerâmica embarcada com opção de U.FL para antena externa
- GPIOs disponíveis para expansão e periféricos adicionais

---

## Esquemático online

Visualize o projeto de hardware diretamente no navegador:

<iframe
  src="https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-SCOTT-DEVKIT%2Ftree%2Fmain%2FHARDWARE"
  width="100%"
  height="700px"
  style={{border: '1px solid #333', borderRadius: '12px'}}
></iframe>

---

## Versões

| Variante | Flash externa QSPI | Aplicação recomendada |
| --- | --- | --- |
| iW-Scott / 8 MB | Winbond W25Q64 (8 MB) | firmware IoT padrão, OTA, sistemas de arquivos |
| iW-Scott / 16 MB | Winbond W25Q128 (16 MB) | modelos de ML maiores, logging extenso, multi-OTA |

---

## Informações mecânicas e ambientais

| Item | Especificação |
| --- | --- |
| Conector principal | USB-C (alimentação + dados) |
| Conector de expansão | header 2x9 (18 pinos), passo 2,54 mm |
| Bateria | conectores dedicados para célula Li-ion única |
| Temperatura de operação | -40 °C a +85 °C |
| Tensão lógica | 3,3 V |
