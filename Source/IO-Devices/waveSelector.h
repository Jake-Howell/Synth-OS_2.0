#ifndef _WAVE_SELECTOR_H
#define _WAVE_SELECTOR_H
#include "waveGen.h"
#include "button.h"
extern EventQueue PrintQueue;
class WaveSelector: public Button{
    public:
    WaveSelector(WaveGen* Synth, IO_Checker* DeviceControler, WAVE_TYPE controlParam, PinName pin, const char * Name = nullptr)
    :Button(DeviceControler, pin, Name){
        this->synth = Synth;
        this->control_param = controlParam;
    }
    void button_control() override{ 
        synth->setWaveType(control_param);  //change synth wave type
        PrintQueue.call(printf, "%s selected\r\n",this->getName());
    }

    private:
        WaveGen * synth;
        WAVE_TYPE control_param;

};



#endif
