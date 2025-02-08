#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "inc/matriz.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

//Definição dos pinos
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define button_a 5 // Pino do botão A
#define button_b 6 // Pino do botão B
#define led_g 11
#define led_b 12

//Variaveis de controle
volatile bool led_state_g = false;
volatile bool led_state_b = false;

//Variaveis globais
ssd1306_t ssd;
char estado_led_verde[20] = "Led Verde: ";
char estado_led_azul[20] = "Led Azul: ";
char str[10] = "Input: ";
char temp[20];
bool ligado = false;

//Declaração de funções
void on_uart_rx();
void init_hardware();
void alterar_display();

int main()
{
  PIO pio = init_pio();
  init_hardware();
  alterar_display();

  char c;

  while (true)
  {

    scanf(" %c", &c);

    if (c >= '0' && c <= '9')
    {
      imprime_numeros(c, pio, 0);
    }

    str[7] = c;
    str[8] = '\0';

    alterar_display();

    sleep_ms(1000);
  }
}

void gpio_irq_handler(uint gpio, uint32_t events)
{
  static absolute_time_t last_time = {0}; // Garante que a variável mantenha seu valor entre chamadas
  absolute_time_t current_time = get_absolute_time();

  // Calcula a diferença de tempo em microsegundos
  int64_t diff = absolute_time_diff_us(last_time, current_time);

  if (diff > 250000) // 250ms
  {
    if (!gpio_get(button_a) || !gpio_get(button_b)) // Verifica se ainda está pressionado
    {

      if (gpio == button_a)
      {
        led_state_g = !led_state_g;   // Inverte o estado
        gpio_put(led_g, led_state_g); // Atualiza o LED

        strncpy(&estado_led_verde[11], led_state_g ? "on " : "off", 4); //Alterar variavel do estado
        estado_led_verde[14] = '\0'; // Garante o terminador null
        printf("O ESTADO DO LED VERDE FOI ALTERADO \n");
        alterar_display();
      }
      else if (gpio == button_b)
      {
        led_state_b = !led_state_b;   // Inverte o estado
        gpio_put(led_b, led_state_b); // Atualiza o LED

        strncpy(&estado_led_azul[10], led_state_b ? "on " : "off", 4);
        estado_led_azul[13] = '\0'; // Garante o terminador null
        alterar_display();
        printf("O ESTADO DO LED AZUL FOI ALTERADO\n");
      }
    }
    last_time = current_time; // Atualiza o tempo da última interrupção válida
  }
}

void alterar_display()
{
  bool cor = true;
  cor = !cor;

  ssd1306_fill(&ssd, !cor);                            // Limpa o display
  ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor);        // Desenha um retângulo
  ssd1306_draw_string(&ssd, str, 10, 10);              // Desenha uma string
  ssd1306_draw_string(&ssd, estado_led_verde, 10, 30); // Desenha uma string
  ssd1306_draw_string(&ssd, estado_led_azul, 10, 50); // Desenha uma string
  ssd1306_send_data(&ssd);
}

void init_hardware()
{
  stdio_init_all();

  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA);                                        // Pull up the data line
  gpio_pull_up(I2C_SCL);                                        // Pull up the clock line
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd);                                         // Configura o display
  ssd1306_send_data(&ssd);

  // Envia os dados para o display
  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);

  gpio_init(led_g);              // Inicializa o pino do LED
  gpio_set_dir(led_g, GPIO_OUT); // Configura o pino como saida

  gpio_init(led_b);              // Inicializa o pino do LED
  gpio_set_dir(led_b, GPIO_OUT); // Configura o pino como saida

  gpio_init(button_a);             // Inicializa o pino do botao
  gpio_set_dir(button_a, GPIO_IN); // Configura o pino como entrada
  gpio_pull_up(button_a);          // Habilita Pull up

  gpio_init(button_b);             // Inicializa o pino do botao
  gpio_set_dir(button_b, GPIO_IN); // Configura o pino como entrada
  gpio_pull_up(button_b);          // Habilita Pull up

  gpio_set_irq_enabled_with_callback(button_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); // seta interrupções
  gpio_set_irq_enabled_with_callback(button_b, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
}