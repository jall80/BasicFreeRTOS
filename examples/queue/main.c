#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>

QueueHandle_t xQueue; // Shared queue

void vSenderTask(void *pvParameters)
{
    int value = (int)pvParameters;
    while (1)
    {
        printf("Sender %d: Sending %d\n", value, value);
        xQueueSend(xQueue, &value, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vReceiverTask(void *pvParameters)
{
    int receivedValue;
    while (1)
    {
        if (xQueueReceive(xQueue, &receivedValue, portMAX_DELAY) == pdTRUE)
        {
            printf("Receiver %s received: %d\n", (char *)pvParameters, receivedValue);
            vTaskDelay(pdMS_TO_TICKS(50)); // Add a small delay to allow other receiver to execute
        }
    }
}

int main(void)
{
    xQueue = xQueueCreate(5, sizeof(int));

    // Create multiple sender tasks
    xTaskCreate(vSenderTask, "Sender1", configMINIMAL_STACK_SIZE, (void *)1, 1, NULL);
    xTaskCreate(vSenderTask, "Sender2", configMINIMAL_STACK_SIZE, (void *)2, 1, NULL);

    // Create multiple receiver tasks
    xTaskCreate(vReceiverTask, "ReceiverA", configMINIMAL_STACK_SIZE, "A", 1, NULL);
    xTaskCreate(vReceiverTask, "ReceiverB", configMINIMAL_STACK_SIZE, "B", 1, NULL);

    vTaskStartScheduler();

    while (1)
        ;
    return 0;
}
