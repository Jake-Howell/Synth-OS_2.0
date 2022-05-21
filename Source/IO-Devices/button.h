#ifndef _BUTTONS_H_
#define _BUTTONS_H_
#include "mbed.h"
#include <cstdint>
#include "waveGen.h"
typedef enum {NOT_PRESSED = 0, PRESSED, DEBOUNCED, RELEASED}ButtonState_t;

class Button : public DigitalIn {
    public:
    Button(PinName pin, const char * Name = nullptr):DigitalIn(pin){
        this->name = Name;
        
        this->current_state = NOT_PRESSED;
        this->previous_state = NOT_PRESSED;
        
    }
    //force programmer to implement control for button
    virtual void control() = 0;
    const char * getName(){
        return this->name;
    }
    //return true if control event should be called
    bool updateEvent(){             //time between measurements set by IO check thread ~50ms
        if(this->read() == true){   //update current state
            current_state = PRESSED;
        }else{
            current_state = NOT_PRESSED;
        }

        switch(previous_state){
            case NOT_PRESSED:           //when previous state is not pressed
                previous_state = current_state; //update previous state
            //if previous state is not pressed 
            //and current state is not pressed, do nothing
            break;
            case PRESSED:   //if previous state was pressed
                if(current_state == PRESSED){   //and current state is pressed
                    current_state = DEBOUNCED;  //current state becomes debounced
                }
                previous_state = current_state; //update previous state
            break;
            case DEBOUNCED: //if previous state was debounced
                if(current_state == PRESSED){   //if button is still pressed
                    current_state = DEBOUNCED;  //button is still debounced
                }else{
                    current_state = RELEASED;
                }
                previous_state = current_state; //update previous state
            break;
            case RELEASED:
                if (current_state == PRESSED){
                    current_state = NOT_PRESSED; //ignore fast double tap
                }else{
                    return true;
                }
            break;
        }
        return false;
    }

    private:
        WaveGen* synth;
        WAVE_TYPE control_param;
        const char * name;
        ButtonState_t current_state;
        ButtonState_t previous_state;

};

#endif