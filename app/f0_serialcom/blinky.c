#include <FreeRTOS.h>
#include <timers.h>
#include <task.h>

#include "leds.h"

void blinky_task(void *params)
{
	(void)params;

	/* 250Ms delay */
	const portTickType heartbeat_delay = 250 / portTICK_RATE_MS;

	uint8_t i, j;

	/* Initialize the LED driver */
	led_init();

	/* Turn off all of the LEDs */
	for (i = 0; i < 4; i++)
		led_clear(i);

	for (;;) {
		for (i = 0; i < 4; i++) {
			vTaskDelay(heartbeat_delay);
			led_toggle(i);
		}

		for (i = 0; i < 5; i++) {
			vTaskDelay(heartbeat_delay);
			for (j = 0; j < 4; j++) {
				if (i == j)
					led_clear(j);
				else
					led_set(j);
			}
		}

		for (i = 0; i < 4; i++) {
			vTaskDelay(heartbeat_delay);
			led_toggle(i);
		}
	}
}
