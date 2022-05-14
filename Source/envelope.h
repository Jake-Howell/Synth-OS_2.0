#ifndef _ENVELOPE_H_
#define _ENVELOPE_H_
#include "mbed.h"

class Envelope{
    public:
        virtual float Attack(uint32_t time_steps) = 0;
        virtual float Decay(uint32_t time_steps) = 0;
        virtual float Sustain() = 0;
        virtual float Release(uint32_t time_steps) = 0;

};

#endif