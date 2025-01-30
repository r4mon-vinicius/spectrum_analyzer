#ifndef MIC_H
#define MIC_H

#include <stdint.h>

// Estrutura para configurar a amostragem do microfone
typedef struct {
    uint8_t mic_channel;
    uint8_t mic_pin;
    float adc_clock_div;
    uint16_t samples;
} mic_config_t;

void mic_init(mic_config_t *config);
void sample_mic(mic_config_t *config, uint16_t *adc_buffer);
float mic_power(uint16_t *adc_buffer, uint16_t samples);
uint8_t get_intensity(float v);

#endif // MIC_H
