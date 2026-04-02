/*
 * tasks.c
 *
 *  Created on: 22 Mar 2026
 *      Author: andre
 */
#include "rtos_tasks.h"

// Polling task for soil
void soilMoisturePollingTask(void *pvParams)
{
	while (1)
	{
		/// task waits and re-triggers every 2s
		PRINTF("Priority 2 soilMoisturePollingTask starts\r\n");
		ADC_Start(ADC_SE0);
		vTaskDelay(pdMS_TO_TICKS(300));
		PRINTF("Next ADC firing...\r\n");
	}
}

void alertTask(void *pvParams)
{
	while (1)
	{
		// task sleeps until ISR fires
		PRINTF("Priority 2 alertTask starts\r\n");
		// Received alert sempahore signal from uartTxTask
		if (xSemaphoreTake(alertSemaphore, portMAX_DELAY) == pdTRUE)
		{
			PRINTF("alertSemaphore taken\r\n");

			// enter CS: mutex to lock shared variable - send_buffer
			xSemaphoreTake(uartMutex, portMAX_DELAY);
			PRINTF("uartMutex taken\r\n");
			// only reaches here whens soil is critically dry
			// notifies ESP, servo control on ESP
			snprintf(send_buffer, MAX_MSG_LEN, "<A,D>\n");
			UART2->C2 |= UART_C2_TE_MASK | UART_C2_TIE_MASK; // kicks off TX of alert,dry  flag

			xSemaphoreGive(uartMutex);
			PRINTF("uartMutex released\r\n");
			// end of CS
		}
	}
}
