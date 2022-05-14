#ifndef _VIBRATO_H_
#define _VIBRATO_H_
#include "mbed.h"
#include "global_defs.hpp"
#include "FX.h"
class Vibrato : public FX{
    Vibrato(WaveGen* Synth, float frequency, float gain):FX(Synth){
        lfo.setGain(gain);
        lfo.setFrq(frequency);
    }

    //override control to control frequency
    void control(float controlParam) override{
        Synth->setFrequencyScalar(controlParam);    
    }
    
};


#endif