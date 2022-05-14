#include "waveGen.h"

using namespace std;

extern EventQueue PrintQueue;

WaveGen::WaveGen(unsigned int sampleRate){
    for (int i = 0; i<KEY_COUNT; i++){  //create 10 key objects
        keys[i] = new Key(this);
    }
    setSampleRate(sampleRate);	//set sample rate and calculate sample period
    setWaveRes(1024);			//calculate 1024 points in sine wave
    setWaveType(SIN);			//set wave type to SINE by default
    pressNote(88, 127);
    pressNote(90, 127);
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
    /*
    unsigned int note_count = playList.size();

    for(int i = 0; i < note_count; i++){
        note = playList.at(i);  //read the next note on the play list

        note.Wo += note.angularStep;	//increment angle
        note.Wo = (note.Wo > ((float)(mWaveRes - 1)))?(note.Wo - ((float)(mWaveRes - 1))):(note.Wo); //bound angle to limits
        switch(mWaveType){
            case SIN:
                sample += (mSineTable[(int)note.Wo])*note.velocity;	//round angle to nearest intager within wave resoloution and multiply by velocity
                break;
            case TRI:
                sample += ((note.Wo >= (float)mWaveRes/2)?(2*(note.Wo/(mWaveRes-1))):(-2*(note.Wo/(mWaveRes-1))))*note.velocity;   //convert sine to triangle wave
                break;
            
            case SAW:
                sample += (note.Wo/(mWaveRes-1))*note.velocity;                                                             //convert sine to saw
                break;
            
            case SQU:
                sample += ((note.Wo >= (float)mWaveRes/2)?1:0)*note.velocity;                                                      //convert sine to square
                break;
        }
        playList.erase(playList.begin() + i );         //remove old note from playlist
        playList.insert(playList.begin() + i,note);    //insert modified note back into playlist
        //printf("Note: %d\tWo: %5.4f\tout: %5.4f \r\n", note.MIDInum, note.Wo,out);
    }*/
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
