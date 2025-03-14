#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>

// Create the semaphore
SemaphoreHandle_t xBinarySemaphore;

void vTask1(void *pvParameters)
{
    while (1)
    {
        // Attempt to take the semaphore
        if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE)
        {
            printf("Task 1 accesses the resource gg\n");
            vTaskDelay(pdMS_TO_TICKS(500));   // Simulate resource usage
            xSemaphoreGive(xBinarySemaphore); // Release the resource
        }
        // vTaskDelay(pdMS_TO_TICKS(1)); // Prevents starvation
        taskYIELD(); // Prevents starvation NO DELAY
    }
}

void vTask2(void *pvParameters)
{
    while (1)
    {
        // Attempt to take the semaphore
        if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE)
        {
            printf("Task 2 accesses the resource gg\n");
            vTaskDelay(pdMS_TO_TICKS(1000));  // Simulate resource usage
            xSemaphoreGive(xBinarySemaphore); // Release the resource
        }
        // vTaskDelay(pdMS_TO_TICKS(1)); // Prevents starvation
        taskYIELD(); // Prevents starvation NO DELAY
    }
}

int main(void)
{
    // Create the semaphore
    xBinarySemaphore = xSemaphoreCreateBinary();

    // Initialize the semaphore with an available value
    xSemaphoreGive(xBinarySemaphore);

    // Create tasks with different priorities
    xTaskCreate(vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    xTaskCreate(vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();

    while (1)
        ;
    return 0;
}
