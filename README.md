# EmbarcaTechContadorMatrizDeLed

**Video do funcionamento:**

Projeto desenvolvido durante a primeira fase da residência Embarcatech.

Este projeto utiliza um Raspberry Pi Pico para controlar LEDs e exibir informações em um display SSD1306. Ele acende LEDs através de botões, exibe caracteres recebidos via serial no display e mostra números em uma matriz de LEDs WS2812 5x5.

## Recursos e Funcionalidades

- Controle de matriz de LEDs WS2812 (NeoPixel) via monitor serial
- Exibição de números de 0 a 9 em um display 5x5 de LEDs.
- Controle via botões para acender/apagar diferentes LEDS.
- Recebimento de caracteres via monitor serial.
- Exibição em um display SSD1306 do status dos leds e do caractere enviado via monitor serial.
- Implementação de debounce por software para evitar leituras erradas dos botões.

## Componentes Utilizados
- Microcontrolador Raspberry Pi Pico.
- Matriz de LEDs WS2812 (25 LEDs).
- display SSD1306.
- Dois botões de entrada.
- LED RGB.

## Dependências
- Biblioteca pico-sdk para programação do Raspberry Pi Pico.
- Configuração de PIO para comunicação com os LEDs WS2812.
- Uso de hardware/pio.h e hardware/aurt.h para manipulação dos periféricos do Raspberry Pi Pico.

## Estrutura do Código
- display.c: Contém a lógica principal do programa, incluindo inicialização de hardware, controle dos LEDs e manipulação dos botões.
- display.pio.h: Definição do programa PIO para comunicação com os LEDs WS2812.
- matriz.h: Biblioteca criada contendo todas as funções necessárias manipulação da matriz de leds.
- font.h: Biblioteca contendo as matrizes para a criação dos caracteres a  serem exibidos no display.
- ssd1306.c: Responsavel pela manipulação do display SSD1306.
- ssd1306.h: Biblioteca contendo funções utilizadas para a manipulação do display.

## Instalação

Clone este repositório e configure o ambiente de desenvolvimento do Raspberry Pi Pico utilizando o pico-sdk.

```bash
git clone https://github.com/MariaP-Ramalho/EmbarcaTechDisplay.git
cd EmbarcaTechDisplay
mkdir build
cd build
cmake ..
make
```

## Como Executar

- Envie o binário gerado (.uf2) para o Raspberry Pi Pico via USB.
- O programa iniciará automaticamente exibindo as mensagens no display.
