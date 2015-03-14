#include <stdint.h>

#include <stm32f0xx_gpio.h>
#include <stm32f0xx_rcc.h>

struct led {
	GPIO_TypeDef *port;
	uint16_t mask;
	uint8_t level;
};

static struct led leds[] = {
	{ GPIOC, GPIO_Pin_8, 0 },
	{ GPIOC, GPIO_Pin_6, 0 },
	{ GPIOC, GPIO_Pin_9, 0 },
	{ GPIOC, GPIO_Pin_7, 0 },
};

void led_init(void)
{
	int i;
	GPIO_InitTypeDef gpio_conf;

	/* Clear the GPIO Structure */
	GPIO_StructInit(&gpio_conf);

	/* turn on debug port and clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

	gpio_conf.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_conf.GPIO_Mode = GPIO_Mode_OUT;
	gpio_conf.GPIO_OType = GPIO_OType_PP;

	for (i = 0; i < 4; ++i){
		gpio_conf.GPIO_Pin = leds[i].mask;
		GPIO_Init(GPIOC, &gpio_conf);
	}

}

void led_set(uint8_t led)
{
	leds[led].level = 1;
	GPIO_SetBits(leds[led].port, leds[led].mask);
}

void led_clear(uint8_t led)
{
	leds[led].level = 0;
	GPIO_ResetBits(leds[led].port, leds[led].mask);
}

void led_toggle(uint8_t led)
{
	if (leds[led].level == 0) {
		leds[led].level = 1;
		GPIO_SetBits(leds[led].port, leds[led].mask);
	} else {
		leds[led].level = 0;
		GPIO_ResetBits(leds[led].port, leds[led].mask);
	}

}
