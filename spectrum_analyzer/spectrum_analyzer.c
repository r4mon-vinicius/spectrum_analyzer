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
        .samples = SAMPLES
    };

    mic_init(&mic_config);

    while (true) {
        sample_mic(&mic_config, adc_buffer);
        float avg = mic_power(adc_buffer, mic_config.samples);
        avg = 2.f * fabsf(ADC_ADJUST(avg));

        uint8_t intensity = get_intensity(avg);
        printf("%2d %8.4f\n", intensity, avg);
        sleep_ms(1000);
    }
}
