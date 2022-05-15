#ifndef _FX_H_
#define _FX_H_
#include "waveGen.h"
#include "LFO.h"
extern EventQueue PrintQueue;
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
            float fs = lfo.getScalar();
            //PrintQueue.call(printf,"LFO Frequency Scalar: %5.4f\r\n", fs);
            Synth->setFrequencyScalar(fs);   
        }
        void useLFOGainScalar(){
            float gs = lfo.getScalar()-0.5f;
            //PrintQueue.call(printf,"LFO Gain Scalar: %5.4f\r\n", gs);
            Synth->setGainScalar(gs);
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