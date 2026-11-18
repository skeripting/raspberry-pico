/*
Date: 11/18/2026
Author: Kushal

PULL UP RESISTORS:
Today, I learned about pull-up resistors. 

The microcontroller can measure the button as 1 or 0 or an impedence state (Hi z, floating input), which means it randomly reads 1 or 0 because of electrical noise.

The pull-up resistor is basically an internal resistor that connects the input pin to a resistor to VCC. 
If you don't use a pull-up resistor (commenting out the pull up line), I noticed the LED pin is always on, even when the button is not pressed. This is because the input pin is floating and can randomly read high or low due to electrical noise.
Further, if you connect the button directly to ground and the input pin to VCC, when the button is pressed, it would create a short circuit between VCC and ground. Very bad for the microcontroller.

*/
#include "pico/stdlib.h"

#define LED_PIN 25
#define BUTTON_PIN 14

int main() {
    // Configure the LED pin as an output and the button pin as an input with a pull-up resistor
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN); // The RP2040 enables the internal resistor to pull the input pin high when the button is not pressed.

    // Why don't we just directly connect the button to ground and the input pin to Vcc? Because when the button is pressed, it would create a short circuit between Vcc and ground, which could damage the microcontroller or the power supply.

    while (true) {
        bool pressed = !gpio_get(BUTTON_PIN); // Asking the microcontroller "what electrical state is being detected at gpio14?"
        gpio_put(LED_PIN, pressed);
        sleep_ms(10);
    }
}
