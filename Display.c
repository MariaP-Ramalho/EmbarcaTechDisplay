#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "inc/matriz.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "tusb.h" 

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define BAUD_RATE 115200

volatile char received_char = '\0';

void on_uart_rx();
ssd1306_t init_hardware();

int main()
{
  PIO pio = init_pio();
  ssd1306_t ssd = init_hardware();
  char c;
  char str[10] = "Input: ";
  char estado_led_verde[15] = "Led Verde: ";
  char estado_led_azul[15] = "Led Azul: ";

  bool cor = true;
  while (true)
  {
    cor = !cor;

    ssd1306_fill(&ssd, !cor);                     // Limpa o display
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_draw_string(&ssd, str, 10, 10);
    printf(str);                                      // Desenha uma string
    ssd1306_draw_string(&ssd, "Led Verde: ", 10, 30); // Desenha uma string
    ssd1306_draw_string(&ssd, "Led Azul: ", 10, 50); // Desenha uma string
    ssd1306_send_data(&ssd);                          // Atualiza o display

    scanf(" %c", &c);

    if(c >= '0' && c <= '9'){
      imprime_numeros(c, pio, 0);
    }

    str[7] = c;
    str[8] = '\0';

    sleep_ms(1000);
  }
}

ssd1306_t init_hardware()
{
  stdio_init_all();
  // I2C Initialisation. Using it at 400Khz.
  i2c_init(I2C_PORT, 400 * 1000);

  gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
  gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
  gpio_pull_up(I2C_SDA);                                        // Pull up the data line
  gpio_pull_up(I2C_SCL);                                        // Pull up the clock line
  ssd1306_t ssd;                                                // Inicializa a estrutura do display
  ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
  ssd1306_config(&ssd);                                         // Configura o display
  ssd1306_send_data(&ssd);
  // Envia os dados para o display
  // Limpa o display. O display inicia com todos os pixels apagados.
  ssd1306_fill(&ssd, false);
  ssd1306_send_data(&ssd);
  return ssd;
}