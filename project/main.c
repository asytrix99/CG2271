/* Includes */
// Default libraries
#include "globals.h"

char send_buffer[MAX_MSG_LEN];

QueueHandle_t queue;
QueueHandle_t sensorQueue;        // moisture readings → UART TX
QueueHandle_t ledQueue;           // LED commands
SemaphoreHandle_t uartMutex;      // protect shraed variable send_buffer (accessed by uartTxTask + alertTask)
SemaphoreHandle_t alertSemaphore; // binary semaphore for signalling between uartTxTask + alertTask

int main(void)
{

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();

#endif

    GPIO_Init();
    ADC_Init();
    initUART2(BAUD_RATE);

    // creating queues and semaphores
    queue = xQueueCreate(QLEN, sizeof(TMessage));
    sensorQueue = xQueueCreate(QLEN, sizeof(int)); // hold single moisture value
    ledQueue = xQueueCreate(QLEN, sizeof(int));    // holds single command number
    uartMutex = xSemaphoreCreateMutex();
    alertSemaphore = xSemaphoreCreateBinary();

    // Initializing tasks
    xTaskCreate(soilMoisturePollingTask, "soil_poll", configMINIMAL_STACK_SIZE + 100, NULL, 2, NULL);
    xTaskCreate(alertTask, "alert", configMINIMAL_STACK_SIZE + 100, NULL, 2, NULL);
    xTaskCreate(uartTxTask, "uart_tx", configMINIMAL_STACK_SIZE + 100, NULL, 3, NULL);
    xTaskCreate(uartRxTask, "uart_rx", configMINIMAL_STACK_SIZE + 100, NULL, 3, NULL);
    xTaskCreate(ledControlTask, "led_ctrl", configMINIMAL_STACK_SIZE + 100, NULL, 2, NULL);

    // Start the scheduler
    vTaskStartScheduler();

    /* Force the counter to be placed into memory. */
    volatile static int i = 0;
    /* Enter an infinite loop, just incrementing a counter. */
    while (1)
    {
        i++;
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile("nop");
    }
    return 0;
}
