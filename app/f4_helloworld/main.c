/* STM32F4xx Hello World project (via blinkenlights)
 *
 * Jeff Ciesielski <jeff.ciesielski@gmail.com>
 */

#include <FreeRTOS.h>
#include <task.h>

#include <stm32f4xx_misc.h>
#include <blinky.h>

void vApplicationTickHook(void)
{
}

void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName )
{
	(void)pxTask;
	(void)pcTaskName;
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	xTaskCreate(blinky_task,
		    (signed portCHAR *) "blinkenlights",
		    configMINIMAL_STACK_SIZE,
		    NULL,
		    (tskIDLE_PRIORITY + 4),
		    NULL);

	vTaskStartScheduler();
	return 0;
}
