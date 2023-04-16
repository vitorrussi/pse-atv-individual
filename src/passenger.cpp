#include "passenger.h"

/* Standard includes. */
#include <stdlib.h>
// #include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <stdarg.h>

#include "queue.h"


#include "car.h"



int Passenger::counter = 0;

Passenger::Passenger() {
    id = counter++;
    std::string taskName = "PassengerTask" + std::to_string(id);
    xTaskCreate(&_task, taskName.c_str(), PTHREAD_STACK_MIN, this, 1, &task_handler);
}

Passenger::~Passenger() {
    
}

int Passenger::board() {
    if (xQueueSend(Car::carArray[0].slots, this, 100) != pdPASS) {
        std::cout << "Passageiro " << id << " não entrou" << std::endl;
        return -1;   
    }
    state(PassengerState::WAITING_IN_CAR);
    vTaskDelay(5000);
    
    return 0;
}

void Passenger::run() {
    state(PassengerState::RUNNING);
    vTaskDelay(5000);
}

void Passenger::unboard() {
    state(PassengerState::WAITING_IN_LINE);
}

void Passenger::begin() {
    std::cout << "Passenger " << id << " begin" << std::endl;

    if (board() < 0) {
        std::cout << "Passenger " << id << " foi embora triste" << std::endl;
        return;
    }
    run();
    unboard();

    std::cout << "Passenger " << id << " foi embora feliz" << std::endl;

}

void Passenger::_task(void *param) {
    Passenger *passenger = (Passenger*) param;
    passenger->begin();
    vTaskDelete(NULL);
}
