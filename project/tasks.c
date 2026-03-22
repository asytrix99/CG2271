/*
 * tasks.c
 *
 *  Created on: 22 Mar 2026
 *      Author: andre
 */
#include "tasks.h"

// Polling task for soil
void soilMoisturePollingTask(void *pvParams) {
	ADC_Start(ADC_SE0);

	while (1) {
		///task waits and re-triggers every 2s
		vTaskDelay(pdMS_TO_TICKS(2000));
		ADC_Start(ADC_SE0);
	}
}

void alertTask(void* pvParams) {
	while (1) {
		// task sleeps until ISR fires

		// Received alert sempahore signal from uartTxTask
		xSemaphoreTake(alertSemaphore, portMAX_DELAY);

		// enter CS: mutex to lock shared variable - send_buffer
		xSemaphoreTake(uartMutex, portMAX_DELAY);
		//only reaches here whens soil is critically dry
		//notifies ESP, servo control on ESP
		snprintf(send_buffer, MAX_MSG_LEN, "<A,D>\n");
		UART2->C2 |= UART_C2_TE_MASK | UART_C2_TIE_MASK; // kicks off TX of alert,dry  flag
		xSemaphoreGive(uartMutex);
		// end of CS
	}
}

