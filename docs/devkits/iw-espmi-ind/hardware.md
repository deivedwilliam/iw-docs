---
id: iw-espmi-ind-hardware
title: Hardware
sidebar_label: Hardware
sidebar_position: 2
slug: /devkits/iw-espmi-ind/hardware
---

import BoardViewer from '@site/src/components/BoardViewer';

# Hardware

Esta secao descreve os principais aspectos fisicos e eletricos da `iW-ESPMI-IND`.

---

## Visao geral da placa

A iW-ESPMI-IND foi desenhada para aplicacoes industriais com forte necessidade de comunicacao e interface visual.

![Placa iW-ESPMI-IND](/img/iw-espmi-ind/BOARD_VIEW.png)

### Modelo 3D

Arraste para girar, role para dar zoom.

<BoardViewer src="/models/iw-espmi-ind-3d.glb" caption="Modelo 3D da iW-ESPMI-IND (preliminar)" />

---

## Pinout

A imagem abaixo resume as interfaces mais importantes da placa.

![Pinout da iW-ESPMI-IND](/img/IW-ESPMI-IND_PINOUT.png)

### Interfaces principais

- USB para programacao e comunicacao
- RS485 para rede Modbus RTU
- SD Card para armazenamento local
- HDMI para interface de video

---

## Alimentacao

A placa suporta uma faixa de alimentacao ampla, o que facilita seu uso em cenarios industriais. A entrada passa por protecao de polaridade (MOSFET `AO3401A` + zener) antes do estagio de potencia baseado no conversor `LMR61430`, que estabelece o trilho de 5 V (5VP). Deste, o LDO `AMS1117-3.3` gera o trilho de 3,3 V do sistema; um LED indica a presenca da tensao de 3,3 V.

| Recurso | Descricao |
| --- | --- |
| Entrada de energia | borne de parafuso (J1) com protecao de polaridade reversa |
| Estagio de potencia | conversor LMR61430 + indutor 6,8 uH -> trilho 5 V (5VP) |
| Regulador 3,3 V | LDO AMS1117-3.3 alimentando a logica e o ESP32-S3 |
| USB | V_BUS (5 V) do USB-C integrado ao trilho 5VP |
| Indicacao | LED de status do trilho de 3,3 V |

### Especificacoes eletricas

| Parametro | Min. | Tip. | Max. | Unidade / Observacao |
| --- | --- | --- | --- | --- |
| Tensao de entrada (borne) | 6,5 | — | 36 | V — entrada de alimentacao |
| Trilho intermediario (5VP) | — | 5,0 | — | V — estagio de potencia |
| Tensao logica do sistema | — | 3,3 | — | V — AMS1117-3.3 |
| Tensao USB (V_BUS) | 4,5 | 5,0 | 5,5 | V — conector USB-C |
| Frequencia de clock da CPU | — | — | 240 | MHz |
| Saida de video | — | HDMI 1.4 | — | DVI/HDMI via TFP410 |
| Terminacao RS485 | — | 120 | — | ohms — selecionavel por JP1 |
| Tensao logica de I/O | — | 3,3 | — | V |
| Temperatura de operacao | -40 | — | +85 | °C — limitada pelo ESP32-S3 |

> Utilize fonte estavel quando houver display HDMI, cartao SD e cargas externas conectadas ao mesmo tempo. A faixa de resolucao/clock de pixel da saida HDMI depende da configuracao do firmware e dos limites do TFP410.

---

## Interfaces e recursos

### Video

- saida HDMI
- boa base para interfaces com LVGL e ferramentas visuais

### Comunicacao industrial

- transceiver RS485 embarcado
- boa aderencia a cenarios com Modbus RTU

### Armazenamento

- slot para SD Card para logs, assets e configuracoes

---

## Esquematico online

Visualize o projeto de hardware diretamente no navegador:

<iframe
  src="https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-ESPMI-IND%2Ftree%2Fmain"
  width="100%"
  height="700px"
  style={{border: '1px solid #333', borderRadius: '12px'}}
></iframe>

[Abrir no KiCanvas](https://kicanvas.org/?repo=https%3A%2F%2Fgithub.com%2FIndustriasWilliam%2FIW-ESPMI-IND%2Ftree%2Fmain)

---

## Boas praticas

- use alimentacao estavel em campo
- cuide do aterramento em redes RS485
- separe bem cabos de potencia e dados
- valide dissipacao e ruido em instalacoes industriais
