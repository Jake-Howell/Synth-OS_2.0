#ifndef _TREMOLO_H_
#define _TREMOLO_H_
#include "mbed.h"
#include "global_defs.hpp"
#include "FX.h"

class Tremolo : public FX{
    Tremolo(WaveGen * Synth, float frequency, float gain):FX(Synth){
        updateLFO(frequency, gain);
        addToSynth();
    }
    void updateFX() override{
        useLFOGainScalar();
    }
};

#endif
