/*
Author: Kushal Timsina
Date: 11/16/2026
Description: This is a learning exercise that I completed to learn how to blink an LED using the Raspberry Pi Pico.

Notes:
volatile - Tells the compiler that the value of the variable can change outside of this program, so it should not optimize away the code that accesses this variable

These memory addresses are predetermined by the RP2040 manufacturer:

0xD0000014 - This is the memory address of the GPIO output set register. 
0xD0000018 - This is the memory address of the GPIO output clear register. 
0xD0000024 - This is the memory address of the GPIO output enable set register. 

RP2040 datasheet: https://pip-assets.raspberrypi.com/categories/814-rp2040/documents/RP-008371-DS-1-rp2040-datasheet.pdf

I found the memory address offsets on table 16 of the datasheet.
*/

#include <stdint.h>
#include "pico/stdlib.h"

#define LED_PIN 25
#define GPIO_OUT_SET (*(volatile uint32_t *)0xD0000014) // uint32_t means unsigned int 32 bits type. Guaranteed to be 32 bits on the pico.
#define GPIO_OUT_CLR (*(volatile uint32_t *)0xD0000018)
#define GPIO_OUTPUT_ENABLE_SET  (*(volatile uint32_t *)0xD0000024) // The base address of the SIO registers is 0xD0000000, add the offset of 0x24 to get the address of the GPIO output enable set register.

// These are some helper functions that I made to help me understand how to use the GPIO registers to control the LED.
// I am not using them in the main function, but I am keeping them here for reference.
void enable_gpio_output(uint pin) {
    GPIO_OUTPUT_ENABLE_SET = (1u << pin); 
}

void set_gpio_output(uint pin) {
    GPIO_OUT_SET = (1u << pin);
}

void clear_gpio_output(uint pin) {
    GPIO_OUT_CLR = (1u << pin);
}

int main() {
    gpio_init(LED_PIN);
    GPIO_OUTPUT_ENABLE_SET = (1u << LED_PIN); // Write a 1 to bit 15 of GPIO_OUTPUT_ENABLE_SET to set GPIO 15 HIGH without affecting other GPIO outputs.

    while (true) {
        GPIO_OUT_SET = (1u << LED_PIN); // Write a 1 to bit 15 of GPIO_OUT_SET to set GPIO 15 HIGH without affecting other GPIO outputs.
        sleep_ms(500);

        GPIO_OUT_CLR = (1u << LED_PIN); // Write a 1 to bit 15 of GPIO_OUT_CLR to set GPIO 15 LOW without affecting other GPIO outputs.
        sleep_ms(500);
    }
}
