/* STM32F0xx Serial terminal example
 *
 * Jeff Ciesielski <jeff.ciesielski@gmail.com>
 */

/* NOTE: This example assumes that you are attaching a serial
 * terminal to USART1 (PA9/PA10) via something like an FTDI cable */

#include <FreeRTOS.h>
#include <task.h>

#include <stm32f0xx_misc.h>
#include <blinky.h>

#include <stdio.h>
#include <usart.h>
#include <shell.h>
#include <alloca.h>

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

static void tasks_help(void)
{
	printf("tasks - show runtime task information\n");
}

static void tasks_handler(int argc, char **argv)
{
	int task_count = uxTaskGetNumberOfTasks();
	char *tbuf = alloca(task_count * 40);
	printf("Name         State     Prio    Stack  Num\n");
	printf("-----------------------------------------\n");
	vTaskList((signed char*)tbuf);
	puts(tbuf);
}

void setup_task(void *params)
{
	struct shell_cmd tasks_cmd = {
		.command = "tasks",
		.action = tasks_handler,
		.help = tasks_help,
	};
	
	(void)params;

	/* Unbuffered standard IO */
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	usart_init(USART_1, 115200);
	usart_enable_autocrlf(USART_1, true);
	newlib_register_dbg_usart(USART_1);
	
	shell_init();

	shell_register_command(&tasks_cmd);
	xTaskCreate(blinky_task,
		    (signed portCHAR *) "Blink",
		    configMINIMAL_STACK_SIZE,
		    NULL,
		    (tskIDLE_PRIORITY + 2),
		    NULL);

	vTaskDelete(NULL);

	/* Shouldn't ever get here */
	while(1);
}

int main(void)
{
	xTaskCreate(setup_task,
		    (signed portCHAR *) "Setup",
		    configMINIMAL_STACK_SIZE,
		    NULL,
		    (tskIDLE_PRIORITY + 2),
		    NULL);

	vTaskStartScheduler();
	return 0;
}
