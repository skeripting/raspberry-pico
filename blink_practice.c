// In here, I rewrote the program from scratch to practice using the GPIO registers to control the LED.

#include <stdint.h>
#include "pico/stdlib.h"

#define LED_PIN 25
#define GPIO_OUT_ENABLE_SET (*(volatile uint32_t *)0xD0000024)
#define GPIO_OUT_VALUE_SET (*(volatile uint32_t *)0xD0000014)

int main() {
    gpio_init(LED_PIN);
    GPIO_OUT_ENABLE_SET = (1u << LED_PIN);
    GPIO_OUT_VALUE_SET = (1u << LED_PIN); 
}