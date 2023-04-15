/* Standard includes. */
#include <stdlib.h>
// #include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdarg.h>

#include "car.h"

int Car::counter = 0;
std::vector<Car> Car::carArray;

Car::Car() {
    if (counter == 0) {
        carArray.reserve(NUMBER_OF_CARS);
    }
    id = counter++;
    slots = xQueueCreateCountingSemaphore(SLOTS, SLOTS);
    carArray[id] = *this;
    xTaskCreate(&_task, "CarTask " + id, 1024, this, 1, NULL);
}

Car::~Car() {
    
}

int Car::load() {
    state(CarState::LOADING);
    while (uxSemaphoreGetCount(slots) > 0) {
        vTaskDelay(100);
    }
    run();
    return 0;
}

void Car::unload() {

}

void Car::run() {
    state(CarState::RUNNING);
}

int Car::slotsRemaining() {

    return 0;
}

int Car::slotsOccupied() {

    return 0;
}

void Car::begin() {
    std::cout << "Car " << id << " begin" << std::endl;
    load();

}

void Car::_task(void *param) {
    Car *car = (Car*) param;
    car->begin();
    vTaskDelete(NULL);
}
