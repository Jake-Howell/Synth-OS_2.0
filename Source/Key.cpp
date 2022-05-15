#include "Key.h"
#include "waveGen.h"



Key::Key(WaveGen* synth){
    Synth = synth;
    setKeyParams(127,0);    //set key params for OFF note
    adsr_state = ATTACK;    //start envelope in attack state
    //set Envelope params for key
    this->envSetup.attack_steps     = 1.0*SAMPLE_RATE;  //calculate number of steps for 0.5 secons
    this->envSetup.decay_steps      = 1.0*SAMPLE_RATE;  //calculate number of steps for 1 second decay
    this->envSetup.release_steps    = 3.0*SAMPLE_RATE;  //calculate number of steps for 3 second release
    this->envSetup.attack_gain      = 1.0f;
    this->envSetup.sustain_gain     = 0.6f;
    envelope = new LinearASDR(this, envSetup);
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
void Key::releaseKey(){
    adsr_state = RELEASE;
}
uint8_t Key::getMIDInum(){
    return this->MIDInum;
}
bool Key::isActive(){
    return this->active;
}
float Key::runEnv(){
    float envGain = 1.0f;
    if(active){
        time_steps += 1;
        switch(adsr_state){
            case ATTACK:
                envGain = envelope->Attack(time_steps);
                if(time_steps >= envSetup.attack_steps){ //if phase has ended, 
                    adsr_state =  DECAY;        //change state
                    time_steps = 0;             //reset step couter
                } 
                break;
            case DECAY:
                envGain = envelope->Decay(time_steps);
                if(time_steps >= envSetup.decay_steps){ //if phase has ended, 
                    adsr_state =  SUSTAIN;        //change state
                    time_steps = 0;             //reset step couter
                } 
                break;
            case SUSTAIN:
                envGain = envelope->Sustain();  //stay in sustain state until synth changes it
                break;
            case RELEASE:
                envGain = envelope->Release(time_steps);
                if(time_steps >= envSetup.release_steps){ //if phase has ended, 
                    adsr_state =  OFF;        //change state
                    time_steps = 0;             //reset step couter
                } 
                break;
            case OFF:
                active = false;         //turn key off
                time_steps = 0;
                adsr_state = ATTACK;    //change state to attack, ready for next key press
                break;
        }
    }
    return envGain;
}
float Key::getSample(){
    float sample = 0.0;
    float envGain = 1.0f;
    updateFrequencyScalar();
    //updateGainScalar();
    if (active){                        //if key not active, sample = 0
        angle += angularStep*frequencyScalar;
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
        envGain = runEnv();
        sample = sample*velocity*envGain;//*gainScalar;
        //printf("Sample Params:\tsample: %5.4f,\tvelocity: %5.4f\tenvGain: %5.4f\tgainScalar: %5.4f\r\n", sample, velocity, envGain, gainScalar);
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

void Key::updateFrequencyScalar(){
    float fs = Synth->getFrequencyScalar();
    fs = (fs>2.0f)?2.0f:fs; //clamp upper scaling limit to 2X
    fs = (fs<0.5f)?0.5f:fs; //clamp lower scaling limit to 1/2X
    this->frequencyScalar = fs;
}
void Key::updateGainScalar(){
    float gs = Synth->getGainScalar();
    gs = (gs>2.0f)?1.0f:gs; //clamp upper scaling limit to 2X
    gs = (gs<0.5f)?0.0f:gs; //clamp lower scaling limit to 1/2X  
}