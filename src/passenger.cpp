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
bool Passenger::all_passengers_created = false;
std::vector<Passenger> Passenger::passengerArray;

Passenger::Passenger() {
    if (counter == 0) {
        passengerArray.reserve(NUMBER_OF_PASSENGERS);
    }
    id = counter++;
    if (counter == NUMBER_OF_PASSENGERS) {
        all_passengers_created = true;
    }
    std::string taskName = "PTask" + std::to_string(id);
    passengerArray[id] = *this;
    auto status = xTaskCreate(&_task, taskName.c_str(), PTHREAD_STACK_MIN, this, 1, &task_handler);
    if (status != pdPASS) {
        Debug() << "Error creating task" << '\n';
    }
}

Passenger::~Passenger() {
    
}

void Passenger::begin() {
    Debug() << "Passenger " << id << " begin" << '\n';
    vTaskDelay(100);
    // if (board() < 0) {
    //     Debug() << "Passenger " << id << " foi embora triste" << '\n';
    //     return;
    // }
    while(rides < 99) {
        if (board() < 0) {
            vTaskDelay(1);
            // _yield();
            continue;
        }    
        run();
        unboard();
    }

    Debug() << "Passenger " << id << " foi embora feliz" << '\n';

}

int Passenger::board() {
    Passenger* ptr1 = this;
    for(uint8_t i = 0; i < Car::counter; i++) {
        Debug() << "Passageiro " << id << " tentando entrar no carro " << std::to_string(i) << '\n';
        if (xQueueSend(Car::carArray[i].slots, &ptr1, 0) == pdPASS) {
            Debug() << "Passageiro " << id << " entrou no carro " << std::to_string(i) << '\n';
            car_id = i;
            state(PassengerState::WAITING_IN_CAR);
            _yield();
            return 0;
        }
        Debug() << "Passageiro " << id << " não entrou no carro " << std::to_string(i) << '\n';
    }
    Debug() << "Passageiro " << id << " não entrou em nenhum carro" << '\n';
    return -1;
}

void Passenger::run() {
    state(PassengerState::RUNNING);
    _yield();
}

void Passenger::unboard() {
    car_id = -1;
    state(PassengerState::WAITING_IN_LINE);
    rides++;
}

void Passenger::_yield() {
    vTaskSuspend(task_handler);
}

void Passenger::_task(void *param) {
    Passenger *passenger = (Passenger*) param;
    passenger->begin();
    vTaskDelete(NULL);
}
