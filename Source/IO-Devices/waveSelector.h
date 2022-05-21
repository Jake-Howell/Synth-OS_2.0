#ifndef _WAVE_SELECTOR_H
#define _WAVE_SELECTOR_H
#include "waveGen.h"
#include "button.h"
extern EventQueue PrintQueue;
class WaveSelector: public Button{
    public:
    WaveSelector(WaveGen* Synth, WAVE_TYPE controlParam, PinName pin, const char * Name = nullptr)
    :Button(pin, Name){
        this->synth = Synth;
        this->control_param = controlParam;
    }
    void control() override{
        if(updateEvent() == true){  // if button has been debounced
            synth->setWaveType(control_param);  //change synth wave type
            PrintQueue.call(printf, "%s selected\r\n",this->getName());
        }
    }

    private:
        WaveGen * synth;
        WAVE_TYPE control_param;

};



#endif
