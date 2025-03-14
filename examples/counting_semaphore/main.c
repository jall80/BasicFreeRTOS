#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include <stdio.h>

#define MAX_RESOURCES 3 // Number of available buffers
#define TOTAL_TASKS 6   // Total number of tasks

SemaphoreHandle_t xCountingSemaphore;

void vTaskFunction(void *pvParameters)
{
    while (1)
    {
        // Attempt to take a buffer
        if (xSemaphoreTake(xCountingSemaphore, portMAX_DELAY) == pdTRUE)
        {
            // Resource acquired, perform processing
            printf("Task %d is using a buffer\n", (int)pvParameters);

            vTaskDelay(pdMS_TO_TICKS(2000)); // Simulate resource usage

            // Release the resource
            xSemaphoreGive(xCountingSemaphore);
            printf("Task %d releases a buffer\n", (int)pvParameters);

            vTaskDelay(pdMS_TO_TICKS(1)); // Allow other tasks to execute. Tasks execute so fast that are always ready, making tasks out the semaphore size (5, 6) get into starvation
        }
    }
}

int main(void)
{
    // Create a counting semaphore with MAX_RESOURCES available
    xCountingSemaphore = xSemaphoreCreateCounting(MAX_RESOURCES, MAX_RESOURCES);

    if (xCountingSemaphore != NULL)
    {
        // Create TOTAL_TASKS tasks
        for (int i = 1; i <= TOTAL_TASKS; i++)
        {
            xTaskCreate(vTaskFunction, "Task", 1000, (void *)i, 1, NULL);
        }

        vTaskStartScheduler(); // Start the FreeRTOS scheduler
    }

    for (;;)
        ;
}
