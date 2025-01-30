#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "mic.h"

#define SAMPLES 256 // Número de amostras do ADC
uint16_t adc_buffer[SAMPLES];

int main() {
    stdio_init_all();
    sleep_ms(5000); // Aguarda 5 segundo para abertura do serial

    mic_config_t mic_config = {
        .mic_channel = 2, // Canal 2 do ADC
        .mic_pin = 28, // Pino 28 do RP2040
        .adc_clock_div = 2835.5f, // Gera um clock de 44.1 kHz
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
        float fft_in[SAMPLES];

        for (uint i = 0; i < SAMPLES; i++) {
            fft_in[i] = (float)adc_buffer[i] - avg;
        }

    }
}
