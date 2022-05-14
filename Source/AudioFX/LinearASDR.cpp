#include "LinearASDR.h"
#include "Key.h"
#include <cstdint>
extern EventQueue PrintQueue;
LinearASDR::LinearASDR(Key* k, EnvParams_t envSetup){
    this->key = k;  //store pointer to key
    this->time_steps = 0;
    this->attack_steps = envSetup.attack_steps;
    this->decay_steps = envSetup.decay_steps;
    this->release_steps = envSetup.release_steps;
    this->attack_gain = envSetup.attack_gain;
    this->sustain_gain = envSetup.sustain_gain;  
}
float LinearASDR::Attack(uint32_t time_steps){
    //float gain = this->sustain_gain;
    float gain = attack_gain*((float)time_steps/attack_steps);
    return gain;
}
float LinearASDR::Decay(uint32_t time_steps){
    //reduce gain down from attack peak, to sustain gain
    float gain = attack_gain - ((attack_gain - sustain_gain)*((float)time_steps/decay_steps));
    return gain;
}
float LinearASDR::Sustain(){
    //hold gain level until key is released
    float gain = this->sustain_gain;
    return gain;
}
float LinearASDR::Release(uint32_t time_steps){
    //reduce gain from sustain gain to 0
    float gain = 1.0f;
    gain = sustain_gain - sustain_gain*((float)time_steps/release_steps);
    return gain;
}