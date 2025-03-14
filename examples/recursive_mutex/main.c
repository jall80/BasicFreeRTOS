#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>

// Mutex recursivo
SemaphoreHandle_t xRecursiveMutex;

// Función que simula una operación recursiva
void vRecursiveFunction(int count)
{
    if (count <= 0)
    {
        return;
    }

    // Adquiere el mutex
    printf("vRecursiveFunction: Acquiring mutex, count = %d\n", count);
    xSemaphoreTake(xRecursiveMutex, portMAX_DELAY);

    // Hacer algo con el recurso compartido
    printf("vRecursiveFunction: Mutex acquired, working, count = %d\n", count);

    // Llamada recursiva
    vRecursiveFunction(count - 1);

    // Liberar el mutex después de la llamada recursiva
    printf("vRecursiveFunction: Releasing mutex, count = %d\n", count);
    xSemaphoreGive(xRecursiveMutex);
}

// Tarea que utiliza el mutex recursivo
void vTaskA(void *pvParameters)
{
    // Se simula un trabajo que requiere varias adquisiciones del mismo mutex
    printf("Task A: Acquiring mutex recursively\n");

    // Adquirir el mutex recursivamente (3 veces)
    vRecursiveFunction(3);

    printf("Task A: Finished work and released mutex\n");

    // El mutex se liberará después de la última liberación en vRecursiveFunction
    vTaskDelete(NULL);
}

int main(void)
{
    // Crear el mutex recursivo
    xRecursiveMutex = xSemaphoreCreateRecursiveMutex();

    if (xRecursiveMutex == NULL)
    {
        printf("Error creating recursive mutex!\n");
        return -1;
    }

    // Crear la tarea con alta prioridad
    xTaskCreate(vTaskA, "TaskA", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

    // Iniciar el scheduler de FreeRTOS
    vTaskStartScheduler();

    // Si el scheduler falla, entramos en un bucle infinito
    for (;;)
        ;
}
