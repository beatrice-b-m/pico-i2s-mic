#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "mic_i2s.h"

// used pins:
#define MIC_BCLK_PIN  10
#define MIC_LRCL_PIN  11
#define MIC_DOUT_PIN  12
#define LED_PIN       13

// recording config:
#define MIC_PIO_NUM     0       // use pio0
#define MIC_NUM_SAMPLES 2048    // num samples in each buffer (recording is double-buffered)
#define MIC_SAMPLE_FREQ 24000   // our mic sensitivity is good up to ~10kHz, so the sampling rate should be about twice that

static void print_samples(uint32_t *samples, size_t num_samples)
{
  // left channel only
  for (size_t i = 0; i < num_samples; i += 2) {
    printf("%d\n", (int16_t) ((samples[i] >> 15) & 0xffff));
  }
}

int main(void)
{
  stdio_init_all();
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  mic_i2s_init(MIC_PIO_NUM, MIC_DOUT_PIN, MIC_BCLK_PIN, MIC_SAMPLE_FREQ, MIC_NUM_SAMPLES);
  mic_i2s_start();
  sleep_ms(3000);

  while (1) {
    // record with LED on
    gpio_put(LED_PIN, 1);
    uint32_t *samples = mic_i2s_get_sample_buffer(true);

    // print with LED off
    gpio_put(LED_PIN, 0);
    print_samples(samples, MIC_NUM_SAMPLES);

    // // blink LED a few times
    // for (int i = 0; i < 4; i++) {
    //   gpio_put(LED_PIN, 1);
    //   sleep_ms(250);
    //   gpio_put(LED_PIN, 0);
    //   sleep_ms(250);
    // }
    // sleep_ms(1000);
  }
}
