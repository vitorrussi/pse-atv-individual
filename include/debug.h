#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <iostream>
#include "FreeRTOS.h"
#include "semphr.h"

class Debug {
private:
    /* data */
public:
    Debug(bool print = true): _print(print) {}
    ~Debug() {}
    
    bool _print;
    // overload << operator to print to std::out with a timestamp and using a freertos mutex
    template <typename T>
    Debug& operator<<(const T& msg) {
        if(_print == false) return *this;
        static SemaphoreHandle_t mutex = xSemaphoreCreateMutex();
        xSemaphoreTake(mutex, portMAX_DELAY);
        std::cout << msg;
        xSemaphoreGive(mutex);
        return *this;
    }
};



#endif
