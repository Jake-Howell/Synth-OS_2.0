#ifndef _WAVE_GEN_H_
#define _WAVE_GEN_H_

#define PI 3.141592654
#define OFF 127
#include "note_periods.cpp"
#include <cmath>
#include "stdlib.h"
#include <vector>
#include "global_defs.hpp"
#include "Key.h"


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

    private:
        vector<toneParams> playList;        //create a list of waveforms to add to sample
        Key *keys[KEY_COUNT];
        WAVE_TYPE mWaveType;                //Type of wave to be generated (SIN, SAW, TRI, SQU)
        char mPitchwheel;
        //Synth Params
        unsigned int mSampleRate = 96000;   //96KHz sample rate
        float mSamplePeriod_us;            //10.41667 us @ 96KHz
        unsigned int active_keys;           //store the number of active keys
		unsigned int mWaveRes = 1024;       //resoloution of wave table
		float mSineTable[1024];		    //sinetable from 0 to 90 degrees

};

#endif