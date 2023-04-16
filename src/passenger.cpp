#include "passenger.h"

/* Standard includes. */
#include <stdlib.h>
// #include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <stdarg.h>

#include "queue.h"

#include "debug.h"
#include "car.h"



int Passenger::counter = 0;

Passenger::Passenger() {
    id = counter++;
    std::string taskName = "PTask" + std::to_string(id);
    xTaskCreate(&_task, taskName.c_str(), PTHREAD_STACK_MIN, this, 1, &task_handler);
}

Passenger::~Passenger() {
    
}

int Passenger::board() {
    Passenger* ptr1 = this;
    if (xQueueSend(Car::carArray[0].slots, &ptr1, 100) != pdPASS) {
        Debug() << "Passageiro " << id << " não entrou" << '\n';
        return -1;   
    }
    state(PassengerState::WAITING_IN_CAR);
    Debug() << "P" << id << "addr: " << this << '\n';
    _yield();
    
    return 0;
}

void Passenger::run() {
    state(PassengerState::RUNNING);
    _yield();
}

void Passenger::unboard() {
    state(PassengerState::WAITING_IN_LINE);
}

void Passenger::begin() {
    Debug() << "Passenger " << id << " begin" << '\n';

    if (board() < 0) {
        Debug() << "Passenger " << id << " foi embora triste" << '\n';
        return;
    }
    run();
    unboard();

    Debug() << "Passenger " << id << " foi embora feliz" << '\n';

}

void Passenger::_yield() {
    // vTaskDelay(5000);
    vTaskSuspend(task_handler);
}

void Passenger::_task(void *param) {
    Passenger *passenger = (Passenger*) param;
    passenger->begin();
    vTaskDelete(NULL);
}
