#ifndef _FX_H_
#define _FX_H_
#include "waveGen.h"
#include "LFO.h"
class FX{
    public:
        FX(WaveGen* synth):lfo(synth, 2, 0.5){    //set default LFO with frq of 2Hz and gain of 0.5
            this->Synth = synth;    //get synth context
        }
    protected:
        LFO lfo;                    //use key as LFO
        float controlParameter;     //give control parameter
        WaveGen * Synth;            //get Synth pointer
        virtual void control(float controlParam) = 0;
        void updateFX(){
            control(lfo.getSample());
        }

};

#endif