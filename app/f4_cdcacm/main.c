/* STM32F4xx Hello World project (via blinkenlights)
 *
 * Jeff Ciesielski <jeff.ciesielski@gmail.com>
 */

#include <FreeRTOS.h>
#include <task.h>

#include <stm32f4xx_misc.h>
#include <blinky.h>
#include <usb_nyan.h>

#ifdef USE_ITM
#include <itm.h>
#endif

#include <stdio.h>

#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usbd_conf.h"
#include "usbd_desc.h"
#include <usbd_cdc_vcp.h>

void vApplicationTickHook(void)
{
}

void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName)
{
	(void)pxTask;
	(void)pcTaskName;
}


USB_OTG_CORE_HANDLE USB_OTG_dev __attribute__ ((aligned (4)));


void setup_task(void *params)
{

	/* Initilize USB cdc mutexes */
	vcp_setup();

	/* Initialize the USB hardware */
	USBD_Init(&USB_OTG_dev,
#ifdef USE_USB_OTG_HS 
		  USB_OTG_HS_CORE_ID,
#else            
		  USB_OTG_FS_CORE_ID,
#endif  
		  &USR_desc, 
		  &USBD_CDC_cb, 
		  &USR_cb);
	
	xTaskCreate(blinky_task,
		    (signed portCHAR *) "blinkenlights",
		    configMINIMAL_STACK_SIZE,
		    NULL,
		    (tskIDLE_PRIORITY + 4),
		    NULL);
	xTaskCreate(nyan_task,
		    (signed portCHAR *) "nyanyanyanya",
		    configMINIMAL_STACK_SIZE,
		    NULL,
		    (tskIDLE_PRIORITY + 4),
		    NULL);

	/* This kills the task */
	while (1) {
		vTaskSuspend(NULL);
	}

}

int main(void)
{
	/* Unbuffered standard IO */
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	xTaskCreate(setup_task,
		    (signed portCHAR *) "Setup task",
		    configMINIMAL_STACK_SIZE,
		    NULL,
		    (tskIDLE_PRIORITY + 4),
		    NULL);


	vTaskStartScheduler();
	return 0;
}
