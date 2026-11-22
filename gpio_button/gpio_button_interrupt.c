
#include "pico/stdlib.h"

#define LED_PIN 25
#define BUTTON_PIN 14

volatile bool button_pressed = false; // volatile tells the compiler the button_pressed value might change

void button_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_PIN && (events & GPIO_IRQ_EDGE_FALL)) {
        button_pressed = true;
    }
}

int main() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, false);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    bool led_on = false;

    while (true) {
        if (button_pressed) {
            button_pressed = false;
            led_on = !led_on;
            gpio_put(LED_PIN, led_on);
        }

        sleep_ms(10);
    }
}
