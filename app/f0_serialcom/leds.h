#ifndef _LEDS_H_
#define _LEDS_H_

#include <stdint.h>

#define LED_1 0
#define LED_2 1
#define LED_3 2
#define LED_4 3

void led_init(void);
void led_set(uint8_t led);
void led_clear(uint8_t led);
void led_toggle(uint8_t led);

#endif
