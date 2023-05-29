#include "car.h"

/* Standard includes. */
#include <stdlib.h>
// #include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdarg.h>
#include <thread>
#include <chrono>

#include "debug.h"
#include "passenger.h"




int Car::counter = 0;
std::vector<Car> Car::carArray;


void printTaskList() {
    char buffer[1000];
    vTaskList(buffer);
    Debug() << "Name          State   Priority  Stack  Num" << '\n';
    Debug() << "******************************************" << '\n';
    Debug() << buffer << '\n';
    Debug() << "******************************************" << '\n';
}

Car::Car() {
    if (counter == 0) {
        carArray.reserve(NUMBER_OF_CARS);
    }
    id = counter++;
    slots = xQueueCreate(SLOTS, sizeof(Passenger*));
    std::string taskName = "CTask" + std::to_string(id);
    carArray[id] = *this;
    xTaskCreate(&_task, taskName.c_str(), PTHREAD_STACK_MIN, this, 2, NULL);
}

Car::~Car() {}

void Car::begin() {
    Debug() << "Car " << id << " begin" << '\n';
    vTaskDelay(100);
    while(rides < 99) {
        load();
        run();
        unload();
    }
    
    Debug() << "Car " << id << " end" << '\n';
}

int Car::load() {
    state(CarState::LOADING);
    if(rides > 0)
        _wakePassengersWaitingInLine();
    while (slotsRemaining() > 0) {
        vTaskDelay(100);
    }
    return 0;
}

void Car::run() {
    state(CarState::RUNNING);
    _wakePassengers();
    vTaskDelay(300);
    state(CarState::RUN_FINISHED);   
}

void Car::unload() {
    state(CarState::UNLOADING);
    rides++;
    _unboardPassengers();
    vTaskDelay(100);
    state(CarState::EMPTY);
}

void Car::_unboardPassengers() {
    Passenger *passenger;
    for(auto i = 0; i < SLOTS; i++) {
        xQueueReceive(slots, &passenger, 100);
        passenger->state(PassengerState::RUN_FINISHED);
        vTaskResume(passenger->task_handler);
    }
}

int Car::slotsRemaining() {
    return uxQueueSpacesAvailable(slots);
}

int Car::slotsOccupied() {
    return SLOTS - slotsRemaining();
}



void Car::_task(void *param) {
    Car *car = (Car*) param;
    car->begin();
    vTaskDelete(NULL);
}
//set the car id to the passenger

void Car::_wakePassengers() {
    Passenger *passenger;
    for(auto i = 0; i < SLOTS; i++) {
        xQueueReceive(slots, &passenger, 100);
        // Debug() << "Waking passenger " << passenger->id << " with addr " << passenger << '\n';
        vTaskResume(passenger->task_handler);
        xQueueSend(slots, &passenger, 100);
    }
}

void Car::_wakePassengersWaitingInLine() {
    for(uint8_t i = 0; i < Passenger::counter; i++) {
        if (Passenger::passengerArray[i].state() == PassengerState::WAITING_IN_LINE &&
            Passenger::passengerArray[i].task_handler != NULL  &&
            Passenger::passengerArray[i].car_id == -1) {
            auto taskstate = eTaskGetState(Passenger::passengerArray[i].task_handler);
            if(taskstate == eSuspended)
                vTaskResume(Passenger::passengerArray[i].task_handler);
                
            vTaskResume(Passenger::passengerArray[i].task_handler);
        }
    }
}
