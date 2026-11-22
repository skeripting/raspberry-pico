/*
In this project, I am experimenting with interrupts.

I have a button interrupt that turns on an external LED for 1.5 seconds when pressed.
I also have a timer interrupt that toggles the built in LED every 250 milliseconds.

When the interrupt priority of the timer > the interrupt priority of the button, the timer is prioritized and the built in LED will always toggle, even when the external LED is on.
*/

#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/timer.h"

#define TIMER_LED_PIN 25
#define BUTTON_LED_PIN 15
#define BUTTON_PIN 14
#define TIMER_INTERVAL_MS 250

uint timer_alarm;
volatile bool timer_led_on = false;

void timer_callback(uint alarm_num) {
    timer_led_on = !timer_led_on;
    gpio_put(TIMER_LED_PIN, timer_led_on);
    hardware_alarm_set_target(alarm_num, make_timeout_time_ms(TIMER_INTERVAL_MS));
}

void button_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_PIN && (events & GPIO_IRQ_EDGE_FALL)) {
        gpio_put(BUTTON_LED_PIN, true);
        busy_wait_ms(1500);
        gpio_put(BUTTON_LED_PIN, false);
    }
}

int main() {
    gpio_init(TIMER_LED_PIN);
    gpio_set_dir(TIMER_LED_PIN, GPIO_OUT);

    gpio_init(BUTTON_LED_PIN);
    gpio_set_dir(BUTTON_LED_PIN, GPIO_OUT);

    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);
    irq_set_priority(IO_IRQ_BANK0, 0x40); // button interrupt has higher numerical priority -> lower interrupt priority

    timer_alarm = hardware_alarm_claim_unused(true);
    hardware_alarm_set_callback(timer_alarm, timer_callback);
    irq_set_priority(hardware_alarm_get_irq_num(timer_alarm), 0x80); // timer has lower numerical priority -> higher interrupt priority
    hardware_alarm_set_target(timer_alarm, make_timeout_time_ms(TIMER_INTERVAL_MS));

    while (true) {
        tight_loop_contents();
    }
}