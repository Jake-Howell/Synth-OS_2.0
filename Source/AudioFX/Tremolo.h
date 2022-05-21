#ifndef _TREMOLO_H_
#define _TREMOLO_H_
#include "mbed.h"
#include "global_defs.hpp"
#include "FX.h"

class Tremolo : public FX{
    public:
    Tremolo(WaveGen * Synth, float frequency, float gain):FX(Synth){
        name = "Tremolo";
        updateLFO(0, 0);
        addToSynth();
    }
    void updateFX() override{
        useLFOGainScalar();
    }
    
};

#endif
