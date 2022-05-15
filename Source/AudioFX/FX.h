#ifndef _FX_H_
#define _FX_H_
#include "waveGen.h"
#include "LFO.h"
class FX{
    public:
        FX(WaveGen* synth):lfo(synth, 2.0f, 1.0f){    //set default LFO with frq of 2Hz and gain of 0.5
            this->Synth = synth;    //get synth context
        }
        //force programmer to implement updateFX()
        //to chose what synth parameter they 
        //want the LFO to modify
        virtual void updateFX() = 0;   
        void addToSynth(){
            Synth->addFX(this);
        }
        void useLFOFrequencyScalar(){
            Synth->setFrequencyScalar(lfo.getScalar());   
        }
        void useLFOGainScalar(){
            Synth->setGainScalar(lfo.getScalar());
        }
        void updateLFO(float frq, float gain){
                lfo.setGain(gain);
                lfo.setFrq(frq);
        }
    private:
        LFO lfo;                    //use key as LFO
        float controlParameter;     //give control parameter
        WaveGen * Synth;            //get Synth pointer        
};

#endif