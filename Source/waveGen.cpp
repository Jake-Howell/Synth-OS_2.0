#include "waveGen.h"
#include "AudioFX.h"
using namespace std;

extern EventQueue PrintQueue;

WaveGen::WaveGen(unsigned int sampleRate){
    for (int i = 0; i<KEY_COUNT; i++){  //create 10 key objects
        keys[i] = new Key(this);
    }
    fx_count = 0;
    setSampleRate(sampleRate);	//set sample rate and calculate sample period
    setWaveRes(1024);			//calculate 1024 points in sine wave
    setWaveType(SIN);			//set wave type to SINE by default
    pressNote(88, 127);
    pressNote(92, 127);
}

void WaveGen::setSampleRate(unsigned int rate){
    this->mSampleRate = rate;														//set sample rate
    this->mSamplePeriod_us = 1000000.0/((float)rate);	//recalculate sample period
}

unsigned int WaveGen::getSampleRate(){
    return this->mSampleRate;
}

unsigned int WaveGen::getSamplePeriod_us(){
    return this->mSamplePeriod_us;
}

void WaveGen::setWaveRes(unsigned int res){	//set wave resoloution up to 1024 (size of sine table array limits this)
    this->mWaveRes = (res <= 1024)? res : 1024;	//bound res to upper limit of 1024
    float angle;
    for (int x = 0; x < mWaveRes; x++){		//loop from 0 to waveRes -1 (0 - 1023 )
        angle = (((float)x*2*PI)/mWaveRes);				//calculate angle increment
        mSineTable[x] = 0.5f*(std::sinf(angle));		//generate wavetable
    }
}

unsigned int WaveGen::getWaveRes(){
    return this->mWaveRes;
}

void WaveGen::setWaveType(WAVE_TYPE type){			//select wave type
    this->mWaveType = type;
}

void WaveGen::pressNote(char noteNum, char velocity){	//add note to wave gen, and turn on wave gen

    for (int i = 0; i < KEY_COUNT; i++){
        if (!(keys[i]->isActive())){    //if key is not active, use key to play note
            keys[i]->setKeyParams(noteNum, velocity);
            active_keys++;
            return;
        }
    }
    PrintQueue.call(printf, "All Keys in use\r\n");
    return;
}

void WaveGen::releaseNote(int MIDInum){
    for (int i = 0; i < KEY_COUNT; i++){
        if (!(keys[i]->isActive()) && (keys[i]->getMIDInum() == MIDInum)){    //if key is not active, use key to play note
            keys[i]->releaseKey();
            active_keys--;
            return;
        }
    }
}

void WaveGen::readMIDI(MIDI_cmd_t cmd){
    switch(cmd.type){
        case NOTE_ON:
            pressNote(cmd.param1, cmd.param2); //tell synth which note to play, and how looud
            PrintQueue.call(printf, "NOTE_ON:  %d Vel: %d\r\n", cmd.param1, cmd.param2);
            break;
        case NOTE_OFF:
            releaseNote(cmd.param1); //tell synth what not to stop playing
            PrintQueue.call(printf, "NOTE_OFF: %d \r\n", cmd.param1);
            break;

        case CONTROL_CHANGE:
            PrintQueue.call(printf, "CONTROL_CHANGE: %d \t%d\r\n", cmd.param1, cmd.param2);
            break;

        case PITCHWHEEL:
            mPitchwheel = cmd.param1;
            PrintQueue.call(printf, "PITCH_WHEEL: %d \t%d\r\n", cmd.param1);
            break;
    }
}
WAVE_TYPE WaveGen::getWaveType(){
    return this->mWaveType;
}
float WaveGen::sin_lut(unsigned int angle){
    return this->mSineTable[angle];
}

float WaveGen::produceSample(){
    //toneParams note; 
    volatile float sample = 0.0f;	//if note is NOT active, output will remain 0.0
    for(int i = 0; i < fx_count; i++){
        audioFX[i]->updateFX();
    }
    for (int i = 0; i<KEY_COUNT; i++){
        sample += keys[i]->getSample();      //get sample from each key
    }
    if (active_keys>1){
        sample = sample/active_keys; //scale RMS
    }
    //Master Clipping
    sample += 0.5f; //Level shift sample - 0 to 1
    sample = (sample > 1.0f)?1.0f:sample;
    sample = (sample < 0.0f)?0.0f:sample;
    return sample;   //return output after scaling
}

void WaveGen::addFX(FX* fx){
    if(fx_count < (MAX_FX-1)){  //if there is space for a new FX object
        audioFX[fx_count] = fx; //add FX object to array
        fx_count++;             //increment fx_count
    }else{
        PrintQueue.call(printf, "No space for new Audio effects\r\n");
    }
}
void WaveGen::setFrequencyScalar(float scalar){
    scalar = (scalar>2.0f)?2.0f:scalar; //set upper limit to 2X frequency
    scalar = (scalar<0.5f)?0.5f:scalar; //set lower limit to 0.5X frequency
    this->frqScalar = scalar;
}
float WaveGen::getFrequencyScalar(){
    return this->frqScalar;
}
void WaveGen::setGainScalar(float scalar){
    scalar = (scalar>1.0f)?1.0f:scalar; //set upper gain limit of 1
    scalar = (scalar < 0.0f)?0.0f:scalar; //set lower limit of 0      
    this->gainScalar = scalar;
}
float WaveGen::getGainScalar(){
    return this->gainScalar;
}