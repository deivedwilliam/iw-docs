---
id: iw-aiot-explorer-hardware
title: Hardware
sidebar_label: Hardware
sidebar_position: 2
slug: /devkits/iw-aiot-explorer/hardware
---

import BoardViewer from '@site/src/components/BoardViewer';

# Hardware

Esta secao detalha os aspectos fisicos e eletricos da `iW-AIoT-eXplorer` (Rev. RV01), a partir do datasheet do produto.

---

## Vista da placa

![Placa iW-AIoT-eXplorer](/img/iw-aiot-explorer/BOARD_VIEW.png)

### Identificacao de componentes

![Componentes da iW-AIoT-eXplorer](/img/iw-aiot-explorer/BOARD_COMPONENTS.png)

A face frontal traz o display IPS de 3,5" com touch capacitivo; na face traseira ficam a camera OV5640, o SoC ESP32-S3R8, a memoria flash, o modulo LoRa e os conectores de expansao.

---

## Modelo 3D

Modelo interativo da placa. Arraste para girar, role para dar zoom e use dois dedos (ou o botao direito) para deslocar.

<BoardViewer src="/models/iw-aiot-explorer-3d.glb" caption="Modelo 3D da iW-AIoT-eXplorer (preliminar)" />

---

## Caracteristicas principais

- SoC `ESP32-S3R8` (Xtensa LX7 dual-core, ate 240 MHz) com instrucoes vetoriais para IA/ML
- 8 MB de PSRAM Octal SPI integrada ao encapsulamento (sufixo R8)
- flash externa QSPI de 16 MB (`W25Q128`) ou 32 MB (`W25Q256`)
- Wi-Fi 802.11 b/g/n + Bluetooth 5 LE / Mesh (2,4 GHz)
- LoRa / LoRaWAN `RFM95W-915S2` (SX1276), 915 MHz (AU915/US915)
- camera `OV5640` — DVP 8 bits + SCCB (I2C)
- 2x microfones digitais PDM `IMP34DT05` (estereo L/R)
- audio I2S Classe-D `MAX98357A` + conector de alto-falante
- display IPS 3,5" 320x240 `ST7789` (SPI) com touch capacitivo I2C
- acelerometro de 3 eixos `LIS3DH` (I2C)
- microSD `Hirose DM3AT` em modo SPI
- expansor de I/O I2C `PCA9531` (backlight, resets, mux de UART, GPIOs)
- USB-C com bridge USB-Serial `CH343P` e boot/reset automatico
- PMIC `IP5306` (carga Li-ion + boost 5 V) e LDO `AMS1117-3,3`

---

## Especificacoes eletricas

| Parametro | Min. | Tip. | Max. | Unidade / Observacao |
| --- | --- | --- | --- | --- |
| Tensao de entrada USB (V_BUS) | 4,5 | 5,0 | 5,5 | V — conector USB-C |
| Tensao de bateria (Li-ion) | 3,0 | 3,7 | 4,2 | V — celula unica |
| Trilho de boost (IP5306) | — | 5,0 | — | V — VOUT do IP5306 |
| Tensao logica do sistema | — | 3,3 | — | V — LDO AMS1117-3,3 |
| Frequencia de clock da CPU | — | — | 240 | MHz |
| Frequencia do cristal | — | 40 | — | MHz (cap. de carga 20 pF) |
| PSRAM integrada | — | 8 | — | MB — Octal SPI (R8) |
| Flash externa QSPI | 16 | — | 32 | MB — W25Q128 / W25Q256 |
| Frequencia LoRa | — | 915 | — | MHz (AU915 / US915) |
| Consumo tipico (Wi-Fi ativo) | — | ~250 | — | mA (pico de RF) |
| Consumo em deep-sleep | — | ~10 | — | uA (perifericos desligados) |
| Temperatura de operacao | -40 | — | +85 | °C |

> Os valores de consumo sao tipicos do ESP32-S3 e variam conforme firmware, potencia de RF, perifericos ativos (camera, display, audio) e brilho do backlight.

---

## Gestao de energia (PMIC IP5306 + LDO)

O sistema de energia combina o `IP5306` — carregador Li-ion, conversor boost sincrono (indutor de 1 uH) e gestao por botao com indicacao de nivel por LEDs — com o regulador `AMS1117-3,3` que gera o trilho logico de 3,3 V.

| Recurso | Descricao |
| --- | --- |
| Entradas de energia | USB-C (5 V via V_BUS) e bateria Li-ion (conector J5) |
| Carregamento | carregador Li-ion integrado ao IP5306, com sense de corrente |
| Boost / saida 5 V | conversor boost sincrono do IP5306 (L3 1 uH) gera o trilho de 5 V |
| Trilho 3,3 V | LDO AMS1117-3,3 a partir do 5 V; LED indicador de energia |
| Indicacao de bateria | LEDs de nivel de carga acionados pelo IP5306 |
| Controle por botao | botao KEY (SW3) para ligar/desligar e gestao de energia |

---

## Perifericos integrados

### Camera — OmniVision OV5640

Sensor CMOS de 5 MP (2592x1944, 1/4") com autofoco, AEC/AGC e AWB embarcados, conectado por interface paralela DVP de 8 bits e configurado pelo barramento SCCB (compativel com I2C). O clock principal (MCLK) vem de oscilador dedicado e os dominios analogico (2,8 V) e de nucleo (1,2 V) sao gerados por LDOs locais.

| Parametro | Valor | Observacao |
| --- | --- | --- |
| Sensor | OmniVision OV5640 | CMOS, formato optico 1/4" |
| Resolucao maxima | 5 MP — 2592x1944 | QSXGA |
| Taxas de quadros | QSXGA 15 fps · 1080p 30 fps · 720p 60 fps | VGA 90 fps · QVGA 120 fps |
| Formatos de saida | RGB565/555/444, YUV422/420, RAW, JPEG | — |
| Interface de dados | DVP paralela 8 bits | PCLK / HSYNC / VSYNC + CSI_D0-D7 |
| Controle | SCCB (compativel I2C) | SIO_C / SIO_D |
| Recursos | autofoco, AEC, AGC, AWB | MCU e motor de AF embarcados |

> Os 4 bits menos significativos do barramento da camera (`CSI_D0-D3`) sao multiplexados com as linhas `DIO0-DIO3` do LoRa pelo `QS3257QG`, com selecao por `GPIO9` (0 = LoRa, 1 = camera). Assim, camera e recepcao de eventos do radio operam de forma alternada.

### Microfones digitais — 2x IMP34DT05

Dois microfones MEMS PDM da STMicroelectronics em arranjo estereo (canais L/R definidos pelo pino `LR`), compartilhando as linhas `PDM_CLK` / `PDM_DOUT`. Omnidirecionais e de baixo consumo, ideais para keyword spotting, classificacao de som e beamforming.

| Parametro | Valor |
| --- | --- |
| Configuracao | estereo (L / R) por uma unica linha de dados |
| SNR | 64 dB |
| Sensibilidade | -26 dBFS +/-3 dB |
| AOP | 122,5 dB SPL |
| Clock PDM | 1,2 – 3,25 MHz |
| Alimentacao | 3,3 V |

### Saida de audio — MAX98357A

Amplificador Classe-D I2S sem filtro que recebe audio digital direto do ESP32-S3 (`BCLK`/`LRCLK`/`DIN`), dispensando DAC e MCLK externos, e aciona um alto-falante pelo conector dedicado (`J1`) em ponte (BTL).

| Parametro | Valor | Observacao |
| --- | --- | --- |
| Potencia max. de saida | 3,2 W | carga 4 ohms · 5 V · THD+N 10% |
| Eficiencia | ate 92% | Classe-D |
| THD+N | 0,013% | 1 kHz tipico |
| Ganho | 3 / 6 / 9 / 12 / 15 dB | selecionavel; 9 dB padrao |
| Carga | 4 – 8 ohms | 4 ohms para potencia maxima |
| Taxas de amostragem | 8 – 96 kHz | I2S, sem MCLK |

### Display IPS 3,5" + touch — ST7789

Display IPS de 3,5" e 320x240 com controlador `ST7789` em SPI; o backlight e comutado por MOSFET `AO3400A` com controle (liga/desliga e PWM) pelo expansor `PCA9531`. O touch capacitivo usa interface I2C propria (interrupcao em `GPIO6`).

| Parametro | Valor | Observacao |
| --- | --- | --- |
| Tipo | TFT LCD IPS | amplo angulo de visao |
| Resolucao | 320x240 | QVGA (paisagem) |
| Controlador | Sitronix ST7789 | GRAM integrada |
| Interface | SPI 4 fios | SCLK / MOSI / MISO / CS / DC / RST |
| Backlight | LED via MOSFET AO3400A | controle e PWM pelo PCA9531 |
| Touch | capacitivo · I2C | INT em GPIO6; SDA/SCL no barramento I2C |

### Acelerometro — LIS3DH

Sensor MEMS de 3 eixos de ultrabaixo consumo em I2C (interrupcoes `INT1`/`INT2`), com fundos de escala selecionaveis, FIFO interno e funcoes embarcadas de deteccao de movimento.

| Parametro | Valor |
| --- | --- |
| Fundos de escala | +/-2 / +/-4 / +/-8 / +/-16 g |
| Resolucao | 16 bits por eixo |
| Taxa de dados (ODR) | 1 Hz – 5,3 kHz |
| Consumo | ~2 uA – 11 uA |
| Interrupcoes | INT1 / INT2 (movimento, click, queda livre, 4D/6D) |
| Recursos | FIFO 32 niveis, ADC aux., sensor de temperatura |

### LoRa / LoRaWAN — RFM95W-915S2

Modulo de radio baseado no Semtech `SX1276`, em 915 MHz, com interface SPI (compartilhada com o microSD), linhas de interrupcao `DIO0-DIO2` e antena dedicada. O hardware implementa a camada fisica LoRa; o LoRaWAN (classes A/B/C) e habilitado por software.

| Parametro | Valor | Observacao |
| --- | --- | --- |
| Modulo / chip | RFM95W-915S2 · SX1276 | transceptor LoRa sub-GHz |
| Banda | 915 MHz | AU915 / US915 (ISM) |
| Modulacao | LoRa (CSS), (G)FSK, OOK | — |
| Potencia max. de saida | +20 dBm (100 mW) | via PA_BOOST |
| Sensibilidade de RX | ate -148 dBm | conforme SF e largura de banda |
| Consumo em TX | ~120 mA @ +20 dBm | ~87 mA @ +17 dBm |
| Consumo em RX | ~10,3 mA | recepcao continua |
| Consumo em sleep | ~0,2 uA | radio em sleep |

### Armazenamento — microSD (Hirose DM3AT)

Soquete microSD push-push em modo SPI, com pull-ups de 10 k, compartilhando o barramento SPI com o modulo LoRa (chip-selects independentes). Tipicamente em sistema de arquivos FAT16/FAT32 para imagens, modelos de ML, datasets e logs.

### Expansor de I/O — PCA9531

Expansor/dimmer I2C de 8 saidas (dreno aberto) que centraliza sinais auxiliares, liberando GPIOs do ESP32-S3.

| Saida | Sinal | Funcao |
| --- | --- | --- |
| LED0 | BACKLIGHT | liga/desliga e PWM de brilho do display |
| LED1 | LCD_RST | reset do controlador ST7789 |
| LED2 | Controle auxiliar | linha reservada |
| LED3 | S1 | selecao do mux de UART (HEF4052) |
| LED4 | S2 | selecao do mux de UART (HEF4052) |
| LED5 | OUT1 | saida de uso geral (conector de expansao) |
| LED6 | OUT2 | saida de uso geral (conector de expansao) |
| LED7 | L_RST | reset do modulo LoRa (RFM95W) |

### USB-Serial — CH343P

Conversor USB-Serial (WCH `CH343P`) ligado ao USB-C, com reset/boot automatico por DTR/RTS (via transistores BC817) acionando `EN` (reset) e `GPIO0` (boot) — gravacao sem pressionar botoes. A UART pode ser roteada entre o CH343P e o conector externo pelo mux `HEF4052` (selecao S1/S2 via PCA9531).

---

## Mapa de alocacao de GPIOs (ESP32-S3)

| GPIO(s) | Sinal | Interface | Observacoes |
| --- | --- | --- | --- |
| 18 / 15 / 14 | SCLK / MOSI / MISO | SPI | barramento SPI do display ST7789 |
| 16 / 17 | CS / DC | SPI | chip-select e Data/Command do display |
| 11 / 12 / 10 | SCLK / MOSI / MISO | SPI | barramento compartilhado microSD + LoRa |
| 13 / 21 | CS_SD / NSS_LoRa | SPI | seletores do microSD e do modulo LoRa |
| 5 / 19 | SCL / SDA | I2C | touch, LIS3DH, PCA9531 e conector (pull-up 2,2 k) |
| 6 | TOUCH_INT | I2C | interrupcao do painel touch capacitivo |
| 7 / 8 / 4 | BCLK / LRCLK / DIN | I2S | audio para o MAX98357A |
| 20 / 3 | PDM_CLK / PDM_DOUT | PDM | microfones digitais (GPIO3 e strapping) |
| 40 / 42 / 45 / 46 | CSI_D4-D7 | DVP | 4 bits mais significativos — diretos ao ESP32-S3 |
| 38 / 39 / 47 / 48 | CSI_D0-D3 / DIO0-D3 | DVP / LoRa | 4 LSB da camera multiplexados com DIO0-3 do LoRa (QS3257QG) |
| 9 | MUX_SEL | Controle | selecao do QS3257QG — 0 = LoRa (DIO), 1 = camera (CSI_D0-3) |
| 1 / 2 / 41 | PCLK / HSYNC / VSYNC | DVP | sincronismo; SCCB (SIO_C/SIO_D) em barramento proprio |
| 0 | BOOT | Strapping | botao BOOT (modo download) |
| CHIP_PU | RESET | — | botao RESET (reinicia o ESP32-S3) |

> `GPIO0`, `GPIO3`, `GPIO45` e `GPIO46` sao pinos de strapping do ESP32-S3 — observe as restricoes de nivel durante o boot. Camera e LoRa compartilham os GPIOs 38/39/47/48 atraves do `QS3257QG`, comandado por `GPIO9`, operando de forma alternada.

---

## Botoes e controles

| Botao | Sinal | Funcao |
| --- | --- | --- |
| RESET | CHIP_PU | reinicia o ESP32-S3R8 |
| BOOT | GPIO0 | entra em modo de download / programacao ao reiniciar |
| KEY / ON | SW3 (IP5306) | liga/desliga e gerenciamento de energia pelo PMIC |

---

## Esquematico online

Visualize o projeto de hardware diretamente no navegador:

<iframe
  src="https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-AIOT-EXPLORER%2Ftree%2Fmain%2FHARDWARE"
  width="100%"
  height="700px"
  style={{border: '1px solid #333', borderRadius: '12px'}}
></iframe>

[Abrir no KiCanvas](https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-AIOT-EXPLORER%2Ftree%2Fmain%2FHARDWARE)

---

## Versoes

| Variante | Flash externa QSPI | Aplicacao recomendada |
| --- | --- | --- |
| iW-AIoT-eXplorer / 16 MB | Winbond W25Q128 (16 MB) | firmware AIoT padrao, OTA, modelos de ML compactos |
| iW-AIoT-eXplorer / 32 MB | Winbond W25Q256 (32 MB) | modelos de visao/audio maiores, datasets em flash, multi-OTA |

---

## Informacoes mecanicas e ambientais

| Item | Especificacao |
| --- | --- |
| Display | IPS 3,5" · 320x240 · ST7789 · touch capacitivo I2C |
| Conector principal | USB-C (alimentacao + dados, via CH343P) |
| Camera | conector flex para modulo OV5640 (DVP + SCCB) |
| Armazenamento | soquete microSD com deteccao (Hirose DM3AT) |
| Audio | conector de alto-falante (J1, 2 vias) |
| Bateria | conector para celula Li-ion unica (J5, 2 vias) |
| Expansao I2C | conector I2C dedicado (SDA / SCL / 3,3 V / GND) |
| Expansao UART/GPIO | conector com UART e GPIO via PCA9531 |
| Antenas | ceramica AN9520-245 (2,4 GHz) + antena dedicada LoRa 915 MHz |
| Temperatura de operacao | -40 °C a +85 °C |
| Tensao logica | 3,3 V |
