#include "prjmain.h"
#include <array>

/* FreeRTOS kernel includes. */

#include "passenger.h"
#include "car.h"
/* passengers */
const auto n = 5;
/* cars */
const auto m = 1;
/* car slots */
const auto C = 3; 

const int Car::NUMBER_OF_CARS = m;
const int Car::SLOTS = C;

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

int prjMain(void) {
    
    std::array<Car,m> cars;
    std::array<Passenger,n> passengers;
    

    // xTaskCreate(&vTask1, "Task 1", 1024, NULL, 1, NULL);
    // xTaskCreate(&vTask2, "Task 2", 1024, NULL, 1, NULL);
    vTaskStartScheduler();

    return 0;
}

void vTask1(void *pvParameters)
{
    for (;;)
    {
        printf("Task 1\r\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTask2(void *pvParameters)
{
    for (;;)
    {
        printf("Task 2\r\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
