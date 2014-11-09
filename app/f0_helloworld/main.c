/* STM32F0xx Hello World project (via blinkenlights)
 *
 * Jeff Ciesielski <jeff.ciesielski@gmail.com>
 */

#include <FreeRTOS.h>
#include <task.h>

#include <stm32f0xx_misc.h>
#include <blinky.h>

#ifdef USE_ITM
#include <itm.h>
#endif

#include <stdio.h>

void vApplicationTickHook(void)
{
}

void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName)
{
	(void)pxTask;
	(void)pcTaskName;
}

void HardFault_Handler(void)
{
	while(1)
		;
}

int main(void)
{
	/* Unbuffered standard IO */
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

#ifdef USE_ITM
	itm_init();
	printf("STM32F0xx Blinkenlights Demo\n");
#endif


	xTaskCreate(blinky_task,
		    (signed portCHAR *) "blinkenlights",
		    configMINIMAL_STACK_SIZE,
		    NULL,
		    (tskIDLE_PRIORITY + 2),
		    NULL);

	vTaskStartScheduler();
	return 0;
}
