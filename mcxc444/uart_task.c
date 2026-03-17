#define QLEN 5
QueueHandle_t queue;

// Queues are initialized
QueueHandle_t uartQueue;
QueueHandle_t ledQueue;

typedef struct tm
{
    char message[MAX_MSG_LEN];
} TMessage;

// Before this, tasks are added to uartQueue or ledQueue...

// Task 1: Higher Priority (2). Parses UART data and sends command to LED queue.
static void uartReceiveTask(void *p)
{
    TMessage msg;
    uint8_t cmd;
    while (1)
    {
        if (xQueueReceive(uartQueue, &msg, portMAX_DELAY) == pdTRUE)
        {
            if (strncmp(msg.message, "ON", 2) == 0)
            {
                cmd = 1;
                xQueueSend(ledQueue, &cmd, 0);
            }
            else if (strncmp(msg.message, "OFF", 3) == 0)
            {
                cmd = 0;
                xQueueSend(ledQueue, &cmd, 0);
            }
        }
    }
}

// Task 2: Lower Priority (1). Controls the actuator based on command.
static void ledControlTask(void *p)
{
    uint8_t cmd;
    while (1)
    {
        if (xQueueReceive(ledQueue, &cmd, portMAX_DELAY) == pdTRUE)
        {
            if (cmd == 1)
            {
                onLED(RED);
            }
            else
            {
                offLED(RED);
            }
        }
    }
}