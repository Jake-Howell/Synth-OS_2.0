#ifndef _RE_H_
#define _RE_H_
#include "AudioFX.h"
#include "mbed.h"
#include "global_defs.hpp"
#include "/IO_interface.h"
#include "IO_checker.h"
#include <cstdint>

typedef enum{LFO_RATE = 0, LFO_GAIN}LFO_Control_t;
extern EventQueue PrintQueue;

class RotaryEncoder : public IO{
    public:
        RotaryEncoder(IO_Checker* DeviceUpdater, RE_Pins RE, FX* audioEffect, LFO_Control_t ctrlParam, const char * Name);
        int update_IO() override;
        void controlFX(float scalar);
        uint8_t getPos();
        float getPosf();
        uint8_t getGC();
        const char * getName() override;

    private:
        FX* fx;
        LFO_Control_t controlParam;
        BusIn bus;
        uint8_t pos; //stores clast read position of the rotary encoder (0 - 127)
        uint8_t GC_latest;
        const char * name;
        uint8_t convert_GreyCode(uint8_t GC);
}; 
#endif
