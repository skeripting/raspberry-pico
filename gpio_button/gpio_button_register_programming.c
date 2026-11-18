/*

For this one, I actually got an led to work fully in binary too!

GPIO_OUT_ENABLE_SET = //0b10000000000000000000000000u;
GPIO_OUT_VALUE_SET = // 0b10000000000000000000000000u; works wonders!
*/

#include "pico/stdlib.h"

#define GPIO_OUT_ENABLE_SET (*(volatile uint32_t *)0xD0000024) // controls whether a pin is output
#define GPIO_OUT_ENABLE_CLR (*(volatile uint32_t *)0xD0000028) // clears the output-enable register for a pin, making it an input

#define GPIO_OUT_VALUE_SET (*(volatile uint32_t *)0xD0000014) // controls whether the output is high
#define GPIO_OUT_VALUE_CLR (*(volatile uint32_t *)0xD0000018) // clears the output value register for a pin, making it low

#define GPIO_IN (*(volatile uint32_t *)0xD0000004)

#define LED_PIN 25
#define BUTTON_PIN 14

int main() {
    gpio_init(LED_PIN);
    gpio_init(BUTTON_PIN);

    GPIO_OUT_ENABLE_SET = (1 << LED_PIN); // Set the LED pin as an output
    GPIO_OUT_ENABLE_CLR = (1 << BUTTON_PIN); // Writes 1 to the clear register, which makes the button pin of the output-enable register 0. 

    gpio_pull_up(BUTTON_PIN); 

    while (true) {
        uint32_t mask = (1 << BUTTON_PIN);

        bool pressed = !(GPIO_IN & mask); 
        /*
        When its not pressed:
        GPIO_IN:  0100 0000 0000 0000 (the button pin is high because of the pull-up resistor)
        AND mask: 0100 0000 0000 0000 
        Result:  0100 0000 0000 0000 (non-zero, true)
        
        When its pressed:
        GPIO_IN:  0000 0000 0000 0000 (the button pin is low because it goes to ground)
        AND mask: 0100 0000 0000 0000 
        Result:  0000 0000 0000 0000 (zero, false)
        */

        if (pressed) {
            GPIO_OUT_VALUE_SET = (1 << LED_PIN); // Set the LED pin high
        }
        else {
            GPIO_OUT_VALUE_CLR = (1 << LED_PIN); // Set the LED pin low
        }

        sleep_ms(10);
    }

}
