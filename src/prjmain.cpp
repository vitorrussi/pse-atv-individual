#include "prjmain.h"
#include "defines.h"
#include <array>

/* FreeRTOS kernel includes. */

#include "passenger.h"
#include "car.h"
/* passengers */
// const auto n = 16;
// /* cars */
// const auto m = 2;
// /* car slots */
// const auto C = 3;

const int Car::NUMBER_OF_CARS = m;
const int Car::SLOTS = C;
const int Passenger::NUMBER_OF_PASSENGERS = n;

void vTimeout(void *pvParameters);

int prjMain(void) {
    
    std::array<Car,m> cars;
    std::array<Passenger,n> passengers;
    
    xTaskCreate(&vTimeout, "TIMEOUT", PTHREAD_STACK_MIN, NULL, 10, NULL);

    // delay 2 seconds before start scheduler
    // std::this_thread::sleep_for(std::chrono::seconds(2));

    vTaskStartScheduler();

    for(auto car : cars) {
        std::cout << "car " << car.id << " rides " << car.rides << std::endl;
    }
    for(auto passenger : passengers) {
        std::cout << "passenger " << passenger.id << " rides " << passenger.rides << std::endl;
    }

    return 0;
}

void vTimeout(void *pvParameters)
{
    std::cout << "timeout begin" << std::endl;
    vTaskDelay(pdMS_TO_TICKS(3000));
    std::cout << "timeout end" << std::endl;
    vTaskEndScheduler();
}
