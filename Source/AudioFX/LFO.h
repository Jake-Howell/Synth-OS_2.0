#ifndef LFO_H_
#define LFO_H_
#include "global_defs.hpp"
#include "waveGen.h"


class LFO{
    public:
    LFO(WaveGen* synth){
        this->Synth = synth;
        this->samplePeriod_us = Synth->getSamplePeriod_us();

        this->waveRes = Synth->getWaveRes();
        setWaveType(SIN);
        setFrq(0);
        setGain(0);
        angle = 0.0f;    
        active = true;
    }
    LFO(WaveGen* synth, float frq, float gain){
        this->Synth = synth;
        this->samplePeriod_us = Synth->getSamplePeriod_us();

        this->waveRes = Synth->getWaveRes();
        setWaveType(SIN);
        setFrq(frq);
        setGain(gain);
        angle = 0.0f;    
        active = true;
    }

    float getScalar(){
        float scalar = 1.0; //if note note active scalar = 1
        if(active){
            angle += angularStep;
            angle = (angle > ((float)(waveRes - 1)))?(angle - ((float)(waveRes - 1))):(angle); //bound angle to limits
            switch(Synth->getWaveType()){
                case SIN:
                    scalar = getSin();      //
                    break;
                case TRI:
                    scalar = getTri();      //convert sine to tri
                    break;
                
                case SAW:
                    scalar = getSaw();      //convert sine to saw
                    break;
                
                case SQU:
                    scalar = getSqu();      //convert sine to square
                    break;
            }
        }
        //printf("key Sample: %d\tangle: %d\r\n",(int)(100*sample), (unsigned int)angle);
        //return scalar centered around 1.0
        //Scalar     = 1.0 + LFOgain*scalar = output Scalar
        //max scalar = 1.0 + (2* 0.5) = 2.0
        //min scalar = 1.0 + (2*-0.5) = 0.0
        //printf("LFO");
        return (1.0 + LFOgain*scalar);
    }

    //returns sin value from -0.5 to +0.5
    float getSin(){
        //round angle to nearest intager within wave resoloution
        float sample = (Synth->sin_lut((int)angle));
        //PrintQueue.call(printf,"key Sample: %d\tangle: %d\r\n",(int)(100*sample), (unsigned int)angle);
        return sample;	
    }
    float getTri(){
        float sample = getSin();
        sample = ((angle >= (float)waveRes/2)?(2*(angle/(waveRes-1))):(-2*(angle/(waveRes-1))));   //convert sine to triangle wave
        return sample;
    }
    float getSaw(){
        float sample = (angle/(waveRes-1));
        return sample;
    }
    float getSqu(){
        float sample = ((angle >= (float)waveRes/2)?1:0);  
        return sample;
    }

    void setGain(float g){  //how much should the scalar vary
        //clamping
        g = (g > LFO_MAX_GAIN)? LFO_MAX_GAIN : g;
        g = (g < LFO_MIN_GAIN)? LFO_MIN_GAIN : g;
        //set gain
        this->LFOgain = g;
    }
    void setFrq(float frq){
        //clamping
        frq = (frq > LFO_MAX_FREQ)? LFO_MAX_FREQ : frq;    //clamp upper limit to 40Hz
        frq = (frq < LFO_MIN_FREQ)? LFO_MIN_FREQ : frq;    //clamp lower limit to 0.2Hz
        //set gain
        this->LFOfreq = frq;
        cycle_period_us = 1000000/(this->LFOfreq);
        float step = (Synth->getSamplePeriod_us())/cycle_period_us;//Calculate how much each time step (samplePeriod) changes the angle
        angularStep = step*(waveRes - 1);
    }
    void setWaveType(WAVE_TYPE wt){
        this->wave_type = wt;
    }
    private:
        WaveGen * Synth;
        bool active;
        WAVE_TYPE wave_type;
        float LFOgain;
        float samplePeriod_us;
        float cycle_period_us;
        unsigned int waveRes;
        float LFOfreq;
        float angle;
        float angularStep;
};

#endif