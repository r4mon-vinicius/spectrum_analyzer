#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "mic.h"
#include "kiss_fftr.h"
#include "ssd1306.h"

// Define o endereço I2C do display OLED
#define I2C_SDA 14
#define I2C_SCL 15

#define SAMPLES 512 // Número de amostras do ADC (bloco de amostragem)
uint16_t adc_buffer[SAMPLES];

int main() {
    stdio_init_all(); // Habilita a comunicação serial

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
            fft_in[i] = ((float)adc_buffer[i] - avg) / 2048.0f;  // Normaliza para [-1,1]
        }

        kiss_fftr(fft_cfg, fft_in, fft_out);

        float sampling_rate = 50000.0f; // Taxa de amostragem do ADC
        float max_magnitude = 0.0f;
        uint max_index = 0;

        for (uint i = 0; i < SAMPLES / 2; i++) {
            float magnitude = sqrtf(fft_out[i].r * fft_out[i].r + fft_out[i].i * fft_out[i].i);

            // Verifica se a magnitude atual é maior que a máxima encontrada
            if (magnitude > max_magnitude) {
                max_magnitude = magnitude;
                max_index = i;
            }
        }

        // Resolucao de Frequência
        float resolution = sampling_rate / SAMPLES;
        
        // Calcula a frequência correspondente à maior magnitude
        float max_frequency = max_index * resolution;
        printf("Frequência de maior magnitude: %.2f Hz | Magnitude: %f\n", max_frequency, max_magnitude);
    }

    kiss_fftr_free(fft_cfg);
}

// Função para inicializar o display OLED
// void init_display(uint8_t *ssd, struct render_area *frame_area) {
//     // Inicializa o I2C
//     i2c_init(i2c1, ssd1306_i2c_clock * 1000);
//     gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
//     gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
//     gpio_pull_up(I2C_SDA);
//     gpio_pull_up(I2C_SCL);

//     // Inicializa o display OLED
//     ssd1306_init();

//     // Configura a área de renderização para o display (largura de ssd1306_width pixels por ssd1306_n_pages páginas)
//     *frame_area = (struct render_area) {
//         .start_column = 0,
//         .end_column = ssd1306_width - 1,
//         .start_page = 0,
//         .end_page = ssd1306_n_pages - 1
//     };

//     calculate_render_area_buffer_length(frame_area);

//     // Limpa o buffer do display
//     memset(ssd, 0, ssd1306_buffer_length);
//     render_on_display(ssd, frame_area);
// }