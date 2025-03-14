#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>

// Create the mutex
SemaphoreHandle_t xMutex;

void vTask1(void *pvParameters)
{
    while (1)
    {
        // Try to take the mutex
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
        {
            printf("Task 1: Accessing the shared resource\n");
            vTaskDelay(pdMS_TO_TICKS(1000)); // Simulate resource usage
            xSemaphoreGive(xMutex);          // Release the mutex
        }
        // vTaskDelay(pdMS_TO_TICKS(1)); // Prevents starvation
        taskYIELD(); // Prevents starvation NO DELAY
    }
}

void vTask2(void *pvParameters)
{
    while (1)
    {
        // Try to take the mutex
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
        {
            printf("Task 2: Accessing the shared resource\n");
            vTaskDelay(pdMS_TO_TICKS(500)); // Simulate resource usage
            xSemaphoreGive(xMutex);         // Release the mutex
        }
        // vTaskDelay(pdMS_TO_TICKS(1)); // Prevents starvation
        taskYIELD(); // Prevents starvation NO DELAY
    }
}

int main(void)
{
    // Create the mutex
    xMutex = xSemaphoreCreateMutex();

    // Create tasks
    xTaskCreate(vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();

    while (1)
        ;
    return 0;
}
