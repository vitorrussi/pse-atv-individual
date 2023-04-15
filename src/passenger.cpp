/* Standard includes. */
#include <stdlib.h>
// #include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdarg.h>

#include "car.h"
#include "passenger.h"

int Passenger::counter = 0;

Passenger::Passenger() {
    id = counter++;
    xTaskCreate(&_task, "PassengerTask " + id, 1024, this, 1, NULL);
}

Passenger::~Passenger() {
    
}

int Passenger::board() {
    if (xSemaphoreTake(Car::carArray[0].slots, 100) != pdTRUE) {
        std::cout << "Passageiro " << id << " não entrou" << std::endl;
        return -1;
        
    } 
    std::cout << "Passageiro " << id << " entrou" << std::endl;
    state(PassengerState::WAITING_IN_CAR);

    

    return 0;
}

void Passenger::unboard() {

}

void Passenger::begin() {
    std::cout << "Passenger " << id << " begin" << std::endl;

    board();

}

void Passenger::_task(void *param) {
    Passenger *passenger = (Passenger*) param;
    passenger->begin();
    vTaskDelete(NULL);
}
