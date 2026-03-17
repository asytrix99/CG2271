/*Initializations*/
// Initialize semaphore (globally)
SemaphoreHandle_t sema;
// Set the blink flag to 0 (main)
sema = xSemaphoreCreateBinary();
// Creating task (main)
// xTaskCreate(taskfun, taskname, stacksize, paramter, priority, handle);
// higher # = higher priority
xTaskCreate(task1, "Task1", configMINIMAL_STACK_SIZE + 100, NULL, 1, NULL);
// Start the scheduler (main)
vTaskStartScheduler();

/*Functions*/
// Release CPU
vTaskDelay(pdMS_TO_TICKS(500));
// Example 1:
// Prints Hello every 500 ms
static void helloTask(void *p)
{
    while (1)
    {
        PRINTF("Hello!\r\n");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// ISR
BaseType_t hpw = pdFALSE;
xSemaphoreGiveFromISR(sema, &hpw);
portYIELD_FROM_ISR(hpw);
// Example 1: button press -> ISR -> give sema -> task wakes up -> toggle LED
void PORTA_IRQHandler()
{
    static int count = 0;
    BaseType_t hpw = pdFALSE;

    count++;
    PRINTF("ISR triggered. count = %d\r\n", count);
    NVIC_ClearPendingIRQ(PORTA_IRQn);
    if (PORTA->ISFR & (1 << SW_PIN))
    {
        if ((count % 5) == 0)
        {
            // increments semaphore by 1
            xSemaphoreGiveFromISR(sema, &hpw);
            // release CPU voluntarily
            portYIELD_FROM_ISR(hpw);
        }
    }
    PORTA->ISFR |= (1 << SW_PIN);
}

// Run task using semaphore
if (xSemaphoreTake(sema, portMAX_DELAY))
{
    toggleLED(RED);
}
// Example 1:
// Task blocks (sleeps) until ISR gives semaphore
// Zero CPU usage while waiting
static void blinkLEDTask(void *p)
{
    PRINTF("BlinkLED Task Started\r\n");
    while (1)
    {
        if (xSemaphoreTake(sema, portMAX_DELAY) == pdTRUE)
            toggleLED(RED);
    }
}