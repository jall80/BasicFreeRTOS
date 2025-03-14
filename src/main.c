#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>

// Recursive mutex handle
SemaphoreHandle_t xRecursiveMutex;

// Function to simulate a recursive operation
void vRecursiveFunction(int count)
{
    if (count <= 0)
    {
        return;
    }

    // Acquiring the recursive mutex
    printf("vRecursiveFunction: Acquiring recursive mutex, count = %d\n", count);

    if (xSemaphoreTakeRecursive(xRecursiveMutex, portMAX_DELAY) == pdTRUE)
    {
        printf("vRecursiveFunction: Mutex acquired, working, count = %d\n", count);
    }
    else
    {
        printf("vRecursiveFunction: Failed to acquire mutex, count = %d\n", count);
        return; // Exit if mutex acquisition fails
    }

    // Simulating work
    vTaskDelay(pdMS_TO_TICKS(100));

    // Recursive call
    vRecursiveFunction(count - 1);

    // Releasing the mutex
    printf("vRecursiveFunction: Releasing recursive mutex, count = %d\n", count);
    xSemaphoreGiveRecursive(xRecursiveMutex);
}

// Task that uses the recursive mutex
void vTaskA(void *pvParameters)
{
    printf("Task A: Starting recursive mutex test\n");

    vRecursiveFunction(3); // Acquire mutex recursively 3 times

    printf("Task A: Finished recursive mutex test\n");

    vTaskDelete(NULL);
}

int main(void)
{
    // Create the recursive mutex
    xRecursiveMutex = xSemaphoreCreateRecursiveMutex();

    if (xRecursiveMutex == NULL)
    {
        printf("Error creating recursive mutex!\n");
        return -1;
    }

    // Create task with increased stack size and higher priority
    xTaskCreate(vTaskA, "TaskA", configMINIMAL_STACK_SIZE * 2, NULL, 3, NULL);

    // Start the scheduler
    vTaskStartScheduler();

    // If the scheduler fails, enter infinite loop
    for (;;)
        ;
}
