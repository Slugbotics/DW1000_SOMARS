#include "pico/stdlib.h"
#include <stdio.h>

#include "pins.h"
#include "globals.h"

// pre-define functions
void prepare_loop(int delay_in_seconds);
void run_zero();
void run_one();

int main() {
    //This function must be initialized here or else PicoLogger functions won't work for some reason
    stdio_init_all();

    //Initialize the pins
    gpio_init(NRST_0_GPIO);
    gpio_set_dir(NRST_0_GPIO, GPIO_OUT);
    gpio_put(NRST_0_GPIO, 1);

    gpio_init(NRST_1_GPIO);
    gpio_set_dir(NRST_1_GPIO, GPIO_OUT);
    gpio_put(NRST_1_GPIO, 1);

    gpio_init(BOOT_0_GPIO);
    gpio_set_dir(BOOT_0_GPIO, GPIO_OUT);
    gpio_put(BOOT_0_GPIO, 0);

    gpio_init(BOOT_1_GPIO);
    gpio_set_dir(BOOT_1_GPIO, GPIO_OUT);
    gpio_put(BOOT_1_GPIO, 0);

    // wait for x amount of time so you can connect to the serial port
    prepare_loop(7);

    printf("pico parasite ready to recive commands\n");

    // init LED
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    uint32_t prev_time = to_ms_since_boot(get_absolute_time());
    bool led_state = false;

    char ch;
    
    // blink LED once a second
    while (true) {
        uint32_t current_time = to_ms_since_boot(get_absolute_time());
        uint32_t elapsed_time = current_time - prev_time;
        if (elapsed_time >= 1000) {
            gpio_put(LED_PIN, led_state);  // Set LED state
            led_state = !led_state;        // Toggle LED state
            prev_time = current_time;
        }

        // recive serial input
        int c = getchar_timeout_us(1);
        if (c != PICO_ERROR_TIMEOUT)
        {
            if(c == '0') {
                run_zero();
            }
            else if(c == '1') {
                run_one();
            }
        }
    }
}

void run_zero()
{
    printf("triggering zero-th device\n");

    gpio_put(BOOT_0_GPIO, 1);
    sleep_ms(BOOT_TO_NRST_DELAY_MS);
    gpio_put(NRST_0_GPIO, 0);
    sleep_ms(NRST_TO_NRST_DELAY_MS);
    gpio_put(NRST_0_GPIO, 1);
    sleep_ms(NRST_TO_BOOT_DELAY_MS);
    gpio_put(BOOT_0_GPIO, 0);

    printf("finished triggering zero-th device\n");
}

void run_one()
{
    printf("triggering one-th device\n");

    gpio_put(BOOT_1_GPIO, 1);
    sleep_ms(BOOT_TO_NRST_DELAY_MS);
    gpio_put(NRST_1_GPIO, 0);
    sleep_ms(NRST_TO_NRST_DELAY_MS);
    gpio_put(NRST_1_GPIO, 1);
    sleep_ms(NRST_TO_BOOT_DELAY_MS);
    gpio_put(BOOT_1_GPIO, 0);

    printf("finished triggering one-th device\n");
}

// wait for x amount of seconds
void prepare_loop(int delay_in_seconds)
{
    for(int i = 0; i < delay_in_seconds; i++)
    {
        printf("Prepairing %d", i);
        sleep_ms(250);
        printf(".");
        sleep_ms(250);
        printf(".");
        sleep_ms(250);
        printf(".");
        sleep_ms(250);
        printf("\n");
    }
}