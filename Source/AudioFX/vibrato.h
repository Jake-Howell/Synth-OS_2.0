#ifndef _VIBRATO_H_
#define _VIBRATO_H_
#include "mbed.h"
#include "global_defs.hpp"
#include "FX.h"

class Vibrato : public FX{
    public:
    Vibrato(WaveGen* Synth, float frequency, float gain):FX(Synth){
        name = "Vibrato";
        updateLFO(frequency, gain);
        addToSynth();
    }
    //override control to control frequency
    void updateFX() override{
         //tell FX class to scale synth frequency with lfo output
         useLFOFrequencyScalar();  
    }
    
};



#endif