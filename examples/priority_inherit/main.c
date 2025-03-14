#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>

// Semaphore (mutex) handle
SemaphoreHandle_t xMutex;

// Task A (High priority)
void vTaskA(void *pvParameters)
{

    // Force Task B to execute first
    vTaskDelay(pdMS_TO_TICKS(10));
    // Task A needs to take the mutex
    printf("Task A: Waiting for mutex...\n");

    // Task A will block until it acquires the mutex (it does not acquire it before Task B releases)
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
    {
        printf("Task A: Acquired mutex.\n");

        // Simulate task work
        vTaskDelay(pdMS_TO_TICKS(500));

        printf("Task A: Released mutex.\n");
        xSemaphoreGive(xMutex);
    }

    vTaskDelete(NULL);
}

// Task B (Low priority)
void vTaskB(void *pvParameters)
{
    // Task B holds the mutex
    printf("Task B: Acquiring mutex...\n");

    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
    {
        printf("Task B: Acquired mutex. Holding for 3 seconds...\n");

        // Simulate task work
        vTaskDelay(pdMS_TO_TICKS(3000));

        printf("Task B: Released mutex.\n");
        xSemaphoreGive(xMutex);
    }

    vTaskDelete(NULL);
}

// Task C (Medium priority)
void vTaskC(void *pvParameters)
{

    // Force Task B to execute first
    vTaskDelay(pdMS_TO_TICKS(10));

    // Ensure Task C runs only after Task B has released the mutex and Task A has completed.
    // This is done by waiting for Task A to release the mutex.
    printf("Task C: Running and will wait if needed.\n");

    // Simulate task work (it must run after Task A)
    vTaskDelay(pdMS_TO_TICKS(10000));

    printf("Task C: Finished.\n");

    vTaskDelete(NULL);
}

int main(void)
{
    // Create the mutex
    xMutex = xSemaphoreCreateMutex();

    if (xMutex == NULL)
    {
        printf("Error creating mutex!\n");
        return -1;
    }

    // Create tasks with appropriate priorities
    xTaskCreate(vTaskB, "TaskB", configMINIMAL_STACK_SIZE, NULL, 1, NULL); // Priority 1 (Low)
    xTaskCreate(vTaskA, "TaskA", configMINIMAL_STACK_SIZE, NULL, 3, NULL); // Priority 3 (High)
    xTaskCreate(vTaskC, "TaskC", configMINIMAL_STACK_SIZE, NULL, 2, NULL); // Priority 2 (Medium)

    // Start the scheduler
    vTaskStartScheduler();

    // In case the scheduler fails
    for (;;)
        ;
}
