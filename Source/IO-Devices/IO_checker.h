#ifndef _IO_CHECKER_H_
#define _IO_CHECKER_H_
#include "mbed.h"
#include <cstdint>
#include <ctime>
#include <chrono>
#define MAX_IO 16
#include "IO_Interface.h"

extern EventQueue PrintQueue;
using namespace std::chrono;

class IO_Checker{
    public:
        IO_Checker();
        void add_to_IO_check_list(IO * device);
        void setUpdateRate(milliseconds ms);
        milliseconds getSleepTime();
        int run_IOchecks();

    private:
        IO * IO_checkList[MAX_IO];
        unsigned int IO_count;
        milliseconds sleepTime;
};

#endif