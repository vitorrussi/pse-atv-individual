#include "car.h"

/* Standard includes. */
#include <stdlib.h>
// #include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdarg.h>

#include "debug.h"
#include "passenger.h"


int Car::counter = 0;
std::vector<Car> Car::carArray;


void printTaskList() {
    // char buffer[1000];
    // vTaskList(buffer);
    // Debug() << "Name          State   Priority  Stack  Num" << '\n';
    // Debug() << "******************************************" << '\n';
    // Debug() << buffer << '\n';
    // Debug() << "******************************************" << '\n';
}

Car::Car() {
    if (counter == 0) {
        carArray.reserve(NUMBER_OF_CARS);
    }
    id = counter++;
    slots = xQueueCreate(SLOTS, sizeof(Passenger*));
    carArray[id] = *this;
    xTaskCreate(&_task, "CTask " + id, PTHREAD_STACK_MIN, this, 1, NULL);
}

Car::~Car() {
    
}

int Car::load() {
    state(CarState::LOADING);
    while (slotsRemaining() > 0) {
        vTaskDelay(100);
    }
    return 0;
}

void Car::run() {
    state(CarState::RUNNING);
    _wakePassengers();
    vTaskDelay(1000);
    state(CarState::RUN_FINISHED);   
}

void Car::unload() {
    state(CarState::UNLOADING);
    _wakePassengers();
    vTaskDelay(1000);
}

int Car::slotsRemaining() {
    return uxQueueSpacesAvailable(slots);
}

int Car::slotsOccupied() {

    return 0;
}

void Car::begin() {
    Debug() << "Car " << id << " begin" << '\n';
    load();
    run();
    unload();
    Debug() << "Car " << id << " end" << '\n';
}

void Car::_task(void *param) {
    Car *car = (Car*) param;
    car->begin();
    vTaskDelete(NULL);
}

void Car::_runPassengers() {
    Passenger *passenger;
    for(auto i = 0; i < SLOTS; i++) {
        xQueueReceive(slots, passenger, 100);
        // passenger->run();
        xQueueSend(slots, passenger, 100);
    }
}

void Car::_unboardPassengers() {
    Passenger *passenger;
    for(auto i = 0; i < SLOTS; i++) {
        xQueueReceive(slots, passenger, 100);
        vTaskResume(passenger->task_handler);
        xQueueSend(slots, passenger, 100);
    }
}

void Car::_wakePassengers() {
    Passenger *passenger;
    for(auto i = 0; i < SLOTS; i++) {
        xQueueReceive(slots, &passenger, 100);
        Debug() << "Waking passenger " << passenger->id << " with addr " << passenger << '\n';
        // xTaskAbortDelay(passenger->task_handler);
        vTaskResume(passenger->task_handler);
        // TaskStatus_t task_status;
        // vTaskGetInfo(passenger->task_handler, &task_status, pdTRUE,eInvalid);
        // Debug() << "Task name: " << task_status.pcTaskName << '\n';
        xQueueSend(slots, &passenger, 100);
    }
}