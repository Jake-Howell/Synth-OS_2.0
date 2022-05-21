#ifndef _KEY_H_
#define _KEY_H_
#include "mbed.h"
#include "envelope.h"
#include "AudioFX/LinearASDR.h"
#include "global_defs.hpp"
#include <cstdint>
class WaveGen;
class Key{
    public:
        Key(WaveGen* Synth);
        float getSample();
        uint8_t getMIDInum();
        void setKeyParams(unsigned int noteNum, unsigned int velocity);
        void releaseKey();
        bool isActive();
        
    private:
        void updateFrequencyScalar();
        void updateGainScalar();
        float runEnv();
        float getSin();
        float getTri();
        float getSaw();
        float getSqu();
        WaveGen * Synth;
        unsigned int waveRes;
        bool   active;         //if note is not active, skip any calculations
        float  angularStep;    //calculated angular step per sample
        float  angle;          //angle
        char   MIDInum;        //MIDI code for note
        float  period_us;      //wavelength
        volatile float  velocity;       //loudness of note (gain)
        float frequencyScalar;
        float gainScalar;

        Envelope* envelope;
        ASDR_STATE_t adsr_state;
        uint32_t time_steps;    //keep track of time steps for envelope
        EnvParams_t envSetup;

};


#endif