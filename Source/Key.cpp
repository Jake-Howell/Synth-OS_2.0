#include "Key.h"
#include "waveGen.h"

//extern EventQueue PrintQueue;

Key::Key(WaveGen* synth){
    Synth = synth;
    setKeyParams(127,0);    //set key params for OFF note
}
void Key::setKeyParams(unsigned int noteNum, unsigned int vel){
    float step = 0.0;
    MIDInum = noteNum;
    waveRes = Synth->getWaveRes();
    period_us = note_periods_us[noteNum];					//select note period using MIDI note index
    angle = 0.0f;
    step = (Synth->getSamplePeriod_us())/period_us;			//Calculate how much each time step (samplePeriod) changes the angle
    velocity = ((float)vel/127);						//calculate velocity multiplier
    angularStep = step*((Synth->getWaveRes()) - 1);			//denormalise angular step to produce an index for sineTable
    active = (noteNum == 127)?false:true;                   //if note = 127, key is off																//turn the wave generator on

}
bool Key::isActive(){
    return this->active;
}
float Key::getSample(){
    float sample = 0.0;
    if (active){                        //if key not active, sample = 0
        angle += angularStep;
        angle = (angle > ((float)(waveRes - 1)))?(angle - ((float)(waveRes - 1))):(angle); //bound angle to limits
        switch(Synth->getWaveType()){
            case SIN:
                sample = getSin();
                break;
            case TRI:
                sample = getTri();      //convert sine to tri
                break;
            
            case SAW:
                sample = getSaw();      //convert sine to saw
                break;
            
            case SQU:
                sample = getSqu();      //convert sine to square
                break;
        }
        sample = sample*velocity;
    }
    //printf("key Sample: %d\tangle: %d\r\n",(int)(100*sample), (unsigned int)angle);
    return sample;
}

float Key::getSin(){
    //round angle to nearest intager within wave resoloution
    float sample = (Synth->sin_lut((int)angle));
    //PrintQueue.call(printf,"key Sample: %d\tangle: %d\r\n",(int)(100*sample), (unsigned int)angle);
    return sample;	
}
float Key::getTri(){
    float sample = getSin();
    sample = ((angle >= (float)waveRes/2)?(2*(angle/(waveRes-1))):(-2*(angle/(waveRes-1))));   //convert sine to triangle wave
    return sample;
}
float Key::getSaw(){
    float sample = (angle/(waveRes-1));
    return sample;
}
float Key::getSqu(){
    float sample = ((angle >= (float)waveRes/2)?1:0);  
    return sample;
}