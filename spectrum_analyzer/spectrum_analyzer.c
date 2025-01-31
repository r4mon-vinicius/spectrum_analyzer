#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "mic.h"
#include "kiss_fftr.h"
#include "ssd1306.h"

// Define o endereço i2c do display oled
#define I2C_SDA 14
#define I2C_SCL 15

#define SAMPLES 512 // Número de amostras do ADC (bloco de amostragem)
uint16_t adc_buffer[SAMPLES];

int main() {
    stdio_init_all(); // Habilita a comunicação serial

    // Inicializa o i2c do oled
    i2c_init(i2c1, 400 * 1000); // 400 kHz
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializa o display OLED
    ssd1306_init();

    // Configuração da FFT
    kiss_fft_scalar fft_in[SAMPLES];
    kiss_fft_cpx fft_out[SAMPLES];
    kiss_fftr_cfg fft_cfg = kiss_fftr_alloc(SAMPLES, false, 0, 0);


    mic_config_t mic_config = {
        .mic_channel = 2, // Canal 2 do ADC
        .mic_pin = 28, // Pino 28 do RP2040
        .adc_clock_div = 960.0f, // 2835.5f = Gera uma taxa amostragem de 44.1 kHz // 960 = 50 khz
        .samples = SAMPLES // Numero de amostras
    };

    mic_init(&mic_config); // Inicializa o microfone

    while (true) {
        sample_mic(&mic_config, adc_buffer); // Captura as amostras do ADC

        uint64_t sum = 0; 

        for (uint i = 0; i < SAMPLES; i++) {
            sum += adc_buffer[i];
        }

        float avg = sum / (float)SAMPLES;

        for (uint i = 0; i < SAMPLES; i++) {
            fft_in[i] = ((float)adc_buffer[i] - avg) / 2048.0f;  // Normaliza para entre -1 e 1
        }

        // Calcula a FFT
        kiss_fftr(fft_cfg, fft_in, fft_out);

        float magnitudes[SAMPLES / 2];
        for (int i = 0; i < SAMPLES / 2; i++) {
            // Calcula a magnitude 
            magnitudes[i] = sqrtf(fft_out[i].r * fft_out[i].r + fft_out[i].i * fft_out[i].i);
        }

        float max_value = 0;
        // Encontra o maior valor
        for (int i = 0; i < SAMPLES / 2; i++) {
            if (magnitudes[i] > max_value) {
                max_value = magnitudes[i];
            }
        }

        // Normaliza os valores para caberem na altura do display (64px)
        uint8_t bar_heights[128];
        for (int i = 0; i < 128; i++) {
            int index = i * (SAMPLES / 2) / 128;  // Ajusta para 128 colunas 
            bar_heights[i] = (uint8_t)(magnitudes[index] / max_value * 64);
        }

        uint8_t frame_buffer[ssd1306_width * (ssd1306_height / 8)] = {0}; // Limpa buffer

        for (int x = 0; x < 128; x++) {
            for (int y = 0; y < bar_heights[x]; y++) {
                ssd1306_set_pixel(frame_buffer, x, 63 - y, true); // Inverte y ( o (0,0) é no canto superior esquerdo)
            }
        }

        // Atualiza o display
        struct render_area area = {0, 127, 0, 7, sizeof(frame_buffer)};
        render_on_display(frame_buffer, &area);
        // Delay de 10ms para melhorar a visualização
        sleep_ms(10);
        
        }

    kiss_fftr_free(fft_cfg);
}