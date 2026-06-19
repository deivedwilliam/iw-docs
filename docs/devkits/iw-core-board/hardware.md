---
id: iw-core-board-hardware
title: Hardware
sidebar_label: Hardware
sidebar_position: 2
slug: /devkits/iw-core-board/hardware
---

import BoardViewer from '@site/src/components/BoardViewer';

# Hardware

Esta seção detalha os aspectos físicos e elétricos da `iW-CORE-BOARD-V1` (Rev. RV01), a partir do datasheet do produto.

---

## Visão geral da placa

A iW-CORE-BOARD-V1 é uma core board e plataforma de HMI construída em torno do **STM32H7** (Cortex-M7 a até 480 MHz). Ela integra display, conectividade sem fio, energia e expansão para servir de núcleo de processamento a uma família de placas de aplicação (shields).

![Placa iW-CORE-BOARD-V1](/img/iw-core-board/BOARD_VIEW.png)

### Modelo 3D

Arraste para girar, role para dar zoom.

<BoardViewer src="/models/iw-coreboard-3d.glb" caption="Modelo 3D da iW-CORE-BOARD-V1 (preliminar)" />

---

## Características principais

- MCU `STM32H743VIT6` (2 MB Flash) ou `STM32H750VB` (128 KB + flash externa)
- 1 MB de SRAM interna (incl. DTCM/ITCM)
- display TFT de 4,3" com 480x272 e touch capacitivo
- Wi-Fi 802.11 b/g/n + BLE pelo módulo `Quectel FC41D`
- acelerômetro de 3 eixos em I2C
- PMIC `IP5306` (carga Li-ion, boost e power on/off por botão)
- bateria principal `BL-5C` e backup de RTC `CR1220`
- USB Full Speed (OTG) + USB-Serial para console/log
- leitor de cartão SD/microSD (SDMMC)
- dois conectores de expansão `iW-APPLICATION` (J4 e J5)
- compatível com `LVGL` e `TouchGFX`

---

## Especificações elétricas

| Parâmetro | Mín. | Típ. | Máx. | Unidade / Observação |
| --- | --- | --- | --- | --- |
| Tensão de entrada USB (V_BUS) | 4,5 | 5,0 | 5,5 | V — conector USB |
| Tensão da bateria (BL-5C, Li-ion) | 3,0 | 3,7 | 4,2 | V — célula única |
| Tensão lógica do sistema | — | 3,3 | — | V |
| Bateria de backup RTC (CR1220) | — | 3,0 | — | V — célula de moeda |
| Frequência de clock da CPU | — | — | 480 | MHz (Cortex-M7) |
| Resolução do display | — | 480x272 | — | pixels (4,3") |
| Corrente de carga da bateria | — | — | ~2,0 | A — IP5306 (típico) |
| Temperatura de operação | -40 | — | +85 | °C — limitada pelos componentes |

> O consumo depende de firmware, brilho do display, potência de RF e periféricos ativos. A corrente de carga e determinada pelo IP5306 e pela capacidade da bateria BL-5C.

---

## Gestão de energia (PMIC IP5306)

O circuito de energia gira em torno do `IP5306`, um CI tipo power-bank que integra carregador Li-ion, conversor boost de 5 V e controle de liga/desliga por botão.

| Recurso | Descrição |
| --- | --- |
| Entradas de energia | USB (5 V) e bateria Li-ion BL-5C |
| Carregamento | carregador Li-ion integrado, com indicação de status |
| Saída | boost de 5 V e trilho de 3,3 V do sistema |
| Controle ON/OFF | liga/desliga por botão gerenciado pelo IP5306 |
| Backup do RTC | bateria de moeda CR1220 mantem o relógio em tempo real |

---

## Periféricos integrados

### Display TFT 4,3" com touch capacitivo

Painel colorido de 480x272 acionado pela interface paralela RGB (controlador `LTDC` do STM32H7), com toque capacitivo lido por I2C. Suporta renderização acelerada com LVGL e TouchGFX.

### Conectividade — Quectel FC41D

Wi-Fi 802.11 b/g/n e BLE em 2,4 GHz, habilitando conexão a rede, provisionamento, OTA e comunicação com apps e nuvem.

### Acelerômetro de 3 eixos

Sensor inercial em I2C para detecção de movimento, orientação da tela, gestos e economia de energia.

### Armazenamento — cartão SD

Leitor SD/microSD via SDMMC para logs, recursos gráficos (fontes, imagens), configurações e dados de aplicação.

### USB e USB-Serial

Porta USB Full Speed (OTG) do STM32H7 e um conversor USB-Serial dedicado para console, depuração e gravação de firmware.

---

## Conectores de expansão iW-APPLICATION (J4 e J5)

A placa expõe os GPIOs do STM32H7 em dois conectores 2x9 (18 vias). A maioria dos pinos é multifuncional; as funções alternativas referem-se ao `STM32H743VIT6` (LQFP100). Os pinos analógicos de J5 servem como entradas ADC.

### Conector J4

| Pino | Sinal | Funções alternativas | Observações |
| --- | --- | --- | --- |
| J4-1 | PE0 | TIM4_ETR, UART8_RX, FMC | GPIO de uso geral |
| J4-2 | PE1 | UART8_TX, FMC | GPIO de uso geral |
| J4-3 | PD5 | USART2_TX, FMC_NWE | UART2 TX |
| J4-4 | PD7 | USART2_CK, SPI1_MOSI | GPIO de uso geral |
| J4-5 | PD2 | SDMMC1_CMD, UART5_RX, TIM3_ETR | GPIO de uso geral |
| J4-6 | PD4 | USART2_RTS, FMC_NOE | GPIO de uso geral |
| J4-7 | PD0 | UART4_RX, FDCAN1_RX, FMC_D2 | GPIO de uso geral |
| J4-8 | PD1 | UART4_TX, FDCAN1_TX, FMC_D3 | GPIO de uso geral |
| J4-9 | PC11 | SDMMC1_D3, USART3_RX, SPI3_MISO | GPIO de uso geral |
| J4-10 | PC12 | SDMMC1_CK, UART5_TX, SPI3_MOSI | GPIO de uso geral |
| J4-11 | PA15 | SPI1/SPI3_NSS, TIM2_CH1, JTDI | GPIO de uso geral |
| J4-12 | PC10 | SDMMC1_D2, USART3_TX, SPI3_SCK | GPIO de uso geral |
| J4-13 | I2C3_SDA | dado do barramento I2C3 (PC9) | Barramento I2C3 |
| J4-14 | I2C3_SCL | clock do barramento I2C3 (PA8) | Barramento I2C3 |
| J4-15 | 3V3 | — | saída regulada 3,3 V |
| J4-16 | PC8 | SDMMC1_D0, USART6_CK, TIM3_CH3 | GPIO de uso geral |
| J4-17 | GND | — | referência / terra |
| J4-18 | 3V3 | — | saída regulada 3,3 V |

### Conector J5

| Pino | Sinal | Funções alternativas | Observações |
| --- | --- | --- | --- |
| J5-1 | GND | — | referência / terra |
| J5-2 | 5V | — | trilho de 5 V |
| J5-3 | 5V | — | trilho de 5 V |
| J5-4 | PD15 | TIM4_CH4, FMC_D1 | GPIO / PWM |
| J5-5 | NC | — | não conectado |
| J5-6 | PD14 | TIM4_CH3, FMC_D0 | GPIO / PWM |
| J5-7 | PD8 | USART3_TX, FMC_D13 | GPIO / UART3 TX |
| J5-8 | PD9 | USART3_RX, FMC_D14 | GPIO / UART3 RX |
| J5-9 | PB12 | SPI2_NSS, FDCAN2_RX, TIM1_BKIN | GPIO de uso geral |
| J5-10 | PB13 | SPI2_SCK, FDCAN2_TX, USART3_CTS | GPIO de uso geral |
| J5-11 | PC4 | ADC1/2, GPIO | entrada analógica (ADC) |
| J5-12 | PC5 | ADC1/2, GPIO | entrada analógica (ADC) |
| J5-13 | PA2 | ADC1/2, USART2_TX, TIM2_CH3 | entrada analógica (ADC) |
| J5-14 | PA7 | ADC1/2, SPI1_MOSI, TIM3_CH2 | entrada analógica (ADC) |
| J5-15 | PC2 | ADC1/2/3, SPI2_MISO | entrada analógica (ADC) |
| J5-16 | PC3 | ADC1/2/3, SPI2_MOSI | entrada analógica (ADC) |
| J5-17 | PE6 | SPI4_MOSI, TIM1_CH2N, TIM15_CH2 | GPIO de uso geral |
| J5-18 | PC13 | RTC_TAMP1 / WKUP2 | uso restrito (domínio RTC) |

> As funções alternativas listadas são as principais do STM32H743VIT6. Consulte o datasheet e o reference manual da ST para o mapeamento completo (AF0 a AF15). `PC13` pertence ao domínio do RTC e tem capacidade de corrente limitada. As linhas `I2C3_SDA/SCL` são compartilhadas com periféricos internos da placa.

---

## Botões e controles

| Botão | Função |
| --- | --- |
| ON/OFF | liga e desliga a placa (controle de energia pelo IP5306) |
| RESET | reinicia o microcontrolador STM32H7 |
| BOOT | seleção do modo de boot na inicialização |

---

## Esquemático online

Visualize o projeto de hardware diretamente no navegador:

<iframe
  src="https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-CORE-BOARD-V1%2Ftree%2Fmain%2FHARDWARE"
  width="100%"
  height="700px"
  style={{border: '1px solid #333', borderRadius: '12px'}}
></iframe>

---

## Versões

| Variante | MCU · Flash interna | Aplicação recomendada |
| --- | --- | --- |
| iW-CORE-BOARD-V1 / H743 | STM32H743VIT6 · 2 MB | firmware standalone, HMI completa, recursos na Flash interna |
| iW-CORE-BOARD-V1 / H750 | STM32H750VB · 128 KB | otimizada para custo; execução/armazenamento a partir de memória externa |

---

## Informações mecânicas e ambientais

| Item | Especificação |
| --- | --- |
| Display | TFT 4,3" · 480x272 · touch capacitivo |
| Conector USB | USB Full Speed + USB-Serial |
| Conectores de expansão | J4 e J5 — headers 2x9 (18 pinos) |
| Armazenamento | slot para cartão SD/microSD |
| Bateria principal | conector BL-5C (Li-ion 3,7 V) |
| Bateria de backup | célula de moeda CR1220 (RTC) |
| Depuração | conector SWD |
| Temperatura de operação | -40 °C a +85 °C |
| Tensão lógica | 3,3 V |
