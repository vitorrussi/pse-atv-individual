#ifndef __PASSENGER_H__
#define __PASSENGER_H__

#include <iostream>
#include <vector>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"


enum class PassengerState{
    WAITING_IN_LINE,
    WAITING_IN_CAR,
    RUNNING,
    RUN_FINISHED
};

inline std::ostream& operator<<(std::ostream & os, PassengerState & passengerState) {
    const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(passengerState){
        PROCESS_VAL(PassengerState::WAITING_IN_LINE);
        PROCESS_VAL(PassengerState::WAITING_IN_CAR);
        PROCESS_VAL(PassengerState::RUNNING);
        PROCESS_VAL(PassengerState::RUN_FINISHED);
    }
#undef PROCESS_VAL
    return os << s;
}

class Passenger {
public:
    Passenger();
    ~Passenger();
    int id;
    int car_id = -1;
    int rides = 0;
    
    PassengerState _state = PassengerState::WAITING_IN_LINE;

    static const int NUMBER_OF_PASSENGERS;
    static bool all_passengers_created;

    TaskHandle_t task_handler;

    const PassengerState state() const {return _state;}
    void state(PassengerState state) {
        std::cout << "Passenger " << id << " from " << _state << " to " << state << std::endl;
        _state = state;
    }

    static int counter;
    static std::vector<Passenger> passengerArray;


    // static void wake_passengers_waiting_in_line();

    int board();
    void run();
    void unboard();

    void begin();
    static void _task(void *param);

    void _yield();
};

#endif