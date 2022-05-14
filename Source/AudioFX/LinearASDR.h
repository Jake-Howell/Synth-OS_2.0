#ifndef _L_ASDR_H_
#define _L_ASDR_H_
#include "../envelope.h"
#include "../global_defs.hpp"
#include "mbed.h"
#include <cstdint>

class Key;
class LinearASDR : public Envelope{
    public:
        LinearASDR(Key* k, EnvParams_t envSetup);
        float Attack(uint32_t time_steps) override;
        float Decay(uint32_t time_steps) override;
        float Sustain() override;
        float Release(uint32_t time_steps) override;
    private:
        Key* key;   //store the address of the key this will modify
        uint32_t time_steps;    //keep track of time steps for envelope
        uint32_t attack_steps;  //define how many time steps in attack phase
        uint32_t decay_steps;   //define how many time steps in decay phase
        uint32_t release_steps; //define number of time steps in release phase
        float attack_gain;
        float sustain_gain;  //define gain level for sustain phase
};

#endif