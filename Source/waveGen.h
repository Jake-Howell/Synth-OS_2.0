#ifndef _WAVE_GEN_H_
#define _WAVE_GEN_H_

#include <cstdint>
#define PI 3.141592654

#include "note_periods.cpp"
#include <cmath>
#include "stdlib.h"
#include <vector>
#include "global_defs.hpp"
#include "Key.h"

class FX;

class WaveGen{
    public:
        WaveGen(unsigned int sampleRate);
        void setSampleRate(unsigned int rate);
        unsigned int getSampleRate();
        unsigned int getSamplePeriod_us();
        void setWaveRes(unsigned int res);
        unsigned int getWaveRes();
        void setWaveType(WAVE_TYPE type);
        void pressNote(char noteNum, char velocity);
        void releaseNote(int MIDInum);
        void readMIDI(MIDI_cmd_t cmd);
        float produceSample();
        WAVE_TYPE getWaveType();
        float sin_lut(unsigned int angle);
        float getFrequencyScalar();
        float getGainScalar();
        void setFrequencyScalar(float scalar);
        void setGainScalar(float scalar);
        void addFX(FX* fx);
        void setASDR(ASDR_Params_t env);
        ASDR_Params_t getASDR_Params();
        void controlChange(uint8_t ctrl_num, uint8_t value);

    private:
        Key *keys[KEY_COUNT];
        FX *audioFX[MAX_FX];   //TODO add list of audio FX to run and call vibrato
        ASDR_Params_t ASDR_Params;
        uint16_t fx_count;
        WAVE_TYPE mWaveType;            //Type of wave to be generated (SIN, SAW, TRI, SQU)
        char mPitchwheel;
        //Synth Params
        unsigned int mSampleRate;       //48KHz sample rate
        float mSamplePeriod_us;         //10.41667 us @ 96KHz
        unsigned int active_keys;       //store the number of active keys
		unsigned int mWaveRes = 1024;   //resoloution of wave table
		float mSineTable[1024];		    //sinetable from 0 to 90 degrees
        //FX Params
        float frqScalar;
        float gainScalar;
};

#endif