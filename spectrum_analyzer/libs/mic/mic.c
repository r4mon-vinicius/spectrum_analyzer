#include "mic.h"
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "hardware/adc.h"

static unsigned char dma_channel;
static dma_channel_config dma_cfg;

// Inicializa o microfone com as configurações passadas.
void mic_init(mic_config_t *config) {
    adc_gpio_init(config->mic_pin);
    adc_init();
    adc_select_input(config->mic_channel);

    adc_fifo_setup(
        true, true, 1, false, false
    );

    adc_set_clkdiv(config->adc_clock_div);

    dma_channel = dma_claim_unused_channel(true);
    dma_cfg = dma_channel_get_default_config(dma_channel);

    channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_16);
    channel_config_set_read_increment(&dma_cfg, false);
    channel_config_set_write_increment(&dma_cfg, true);
    channel_config_set_dreq(&dma_cfg, DREQ_ADC);
}

// Captura as amostras do ADC e armazena no buffer.
void sample_mic(mic_config_t *config, uint16_t *adc_buffer) {
    adc_fifo_drain();
    adc_run(false);

    dma_channel_configure(dma_channel, &dma_cfg,
        adc_buffer,
        &(adc_hw->fifo),
        config->samples,
        true
    );

    adc_run(true);
    dma_channel_wait_for_finish_blocking(dma_channel);
    adc_run(false);
}

// Calcula a potência média das leituras do ADC (Valor RMS).
float mic_power(uint16_t *adc_buffer, uint16_t samples) {
    float avg = 0.f;

    for (uint16_t i = 0; i < samples; ++i)
        avg += adc_buffer[i] * adc_buffer[i];

    avg /= samples;
    return sqrtf(avg);
}

// Calcula a intensidade do volume baseado na tensão.
uint8_t get_intensity(float v) {
    uint8_t count = 0;

    while ((v -= ADC_STEP / 20) > 0.f)
        ++count;

    return count;
}
