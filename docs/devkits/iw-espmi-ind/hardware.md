---
id: iw-espmi-ind-hardware
title: Hardware
sidebar_label: Hardware
sidebar_position: 2
slug: /devkits/iw-espmi-ind/hardware
---

import BoardViewer from '@site/src/components/BoardViewer';

# Hardware

Esta seção descreve os principais aspectos físicos e elétricos da `iW-ESPMI-IND`.

---

## Visão geral da placa

A iW-ESPMI-IND foi desenhada para aplicações industriais com forte necessidade de comunicação e interface visual.

![Placa iW-ESPMI-IND](/img/iw-espmi-ind/BOARD_VIEW.png)

### Modelo 3D

Arraste para girar, role para dar zoom.

<BoardViewer src="/models/iw-espmi-ind-3d.glb" caption="Modelo 3D da iW-ESPMI-IND (preliminar)" />

---

## Pinout

A imagem abaixo resume as interfaces mais importantes da placa.

![Pinout da iW-ESPMI-IND](/img/IW-ESPMI-IND_PINOUT.png)

### Interfaces principais

- USB para programação e comunicação
- RS485 para rede Modbus RTU
- SD Card para armazenamento local
- HDMI para interface de vídeo

---

## Alimentação

A placa suporta uma faixa de alimentação ampla, o que facilita seu uso em cenários industriais. A entrada passa por proteção de polaridade (MOSFET `AO3401A` + zener) antes do estágio de potência baseado no conversor `LMR61430`, que estabelece o trilho de 5 V (5VP). Deste, o LDO `AMS1117-3.3` gera o trilho de 3,3 V do sistema; um LED indica a presença da tensão de 3,3 V.

| Recurso | Descrição |
| --- | --- |
| Entrada de energia | borne de parafuso (J1) com proteção de polaridade reversa |
| Estágio de potência | conversor LMR61430 + indutor 6,8 uH -> trilho 5 V (5VP) |
| Regulador 3,3 V | LDO AMS1117-3.3 alimentando a lógica e o ESP32-S3 |
| USB | V_BUS (5 V) do USB-C integrado ao trilho 5VP |
| Indicação | LED de status do trilho de 3,3 V |

### Especificações elétricas

| Parâmetro | Mín. | Típ. | Máx. | Unidade / Observação |
| --- | --- | --- | --- | --- |
| Tensão de entrada (borne) | 6,5 | — | 36 | V — entrada de alimentação |
| Trilho intermediario (5VP) | — | 5,0 | — | V — estágio de potência |
| Tensão lógica do sistema | — | 3,3 | — | V — AMS1117-3.3 |
| Tensão USB (V_BUS) | 4,5 | 5,0 | 5,5 | V — conector USB-C |
| Frequência de clock da CPU | — | — | 240 | MHz |
| Saída de vídeo | — | HDMI 1.4 | — | DVI/HDMI via TFP410 |
| Terminação RS485 | — | 120 | — | ohms — selecionável por JP1 |
| Tensão lógica de I/O | — | 3,3 | — | V |
| Temperatura de operação | -40 | — | +85 | °C — limitada pelo ESP32-S3 |

> Utilize fonte estável quando houver display HDMI, cartão SD e cargas externas conectadas ao mesmo tempo. A faixa de resolução/clock de pixel da saída HDMI depende da configuração do firmware e dos limites do TFP410.

---

## Interfaces e recursos

### Vídeo

- saída HDMI
- boa base para interfaces com LVGL e ferramentas visuais

### Comunicação industrial

- transceiver RS485 embarcado
- boa aderência a cenários com Modbus RTU

### Armazenamento

- slot para SD Card para logs, assets e configurações

---

## Esquemático online

Visualize o projeto de hardware diretamente no navegador:

<iframe
  src="https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-ESPMI-IND%2Ftree%2Fmain"
  width="100%"
  height="700px"
  style={{border: '1px solid #333', borderRadius: '12px'}}
></iframe>

---

## Boas práticas

- use alimentação estável em campo
- cuide do aterramento em redes RS485
- separe bem cabos de potência e dados
- valide dissipação e ruído em instalações industriais
