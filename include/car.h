#ifndef __CAR_H__
#define __CAR_H__

#include <iostream>
#include <vector>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

enum class CarState {
    EMPTY,
    LOADING,
    RUNNING,
    RUN_FINISHED,
    UNLOADING
};

inline std::ostream& operator<<(std::ostream & os, CarState & carState) {
    const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(carState){
        PROCESS_VAL(CarState::EMPTY);
        PROCESS_VAL(CarState::LOADING);
        PROCESS_VAL(CarState::RUNNING);
        PROCESS_VAL(CarState::RUN_FINISHED);
        PROCESS_VAL(CarState::UNLOADING);
    }
#undef PROCESS_VAL
    return os << s;
}

class Car {
public:
    Car();
    ~Car();
    int id;
    int passengers[10];
    CarState _state = CarState::EMPTY;

    static const int SLOTS;
    static const int NUMBER_OF_CARS;

    static int counter;
    
    static std::vector<Car> carArray;

    QueueHandle_t slots;

    const CarState state() const {return _state;}
    void state(CarState state) {
        std::cout << "Car " << id << " from " << _state << " to " << state << std::endl;
        _state = state;
    }

    int load();
    void unload();
    void run();

    int slotsRemaining();
    int slotsOccupied();

    void begin();
    
    static void _task(void *param);

    friend std::ostream& operator<<(std::ostream& os, const Car& obj) {
        // write obj to stream
        return os;
    }

    void _runPassengers();
    void _unboardPassengers();
    void _wakePassengers();

};

#endif