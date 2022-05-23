#include "mbed.h"
#include <cstdint>
#include "button.h"
//#include "IO-Devices.h"


Button::Button(IO_Checker* DeviceUpdater, PinName pin, const char * Name = nullptr):DigitalIn(pin){
    this->name = Name;
    this->DeviceChecker = DeviceUpdater;
    this->current_state = NOT_PRESSED;
    this->previous_state = NOT_PRESSED;
    DeviceChecker->add_to_IO_check_list(this);
    
}
//force programmer to implement control for button

const char * Button::getName(){
    return this->name;
}
//return true if control event should be called
int Button::update_IO(){             //time between measurements set by IO check thread ~50ms
    //printf("%s previous state %d\t current state: %d\r\n",name, previous_state, current_state);
    if(this->read() == true){   //update current state
        current_state = PRESSED;
    }else{
        current_state = NOT_PRESSED;
    }

    switch(previous_state){
        case NOT_PRESSED:           //when previous state is not pressed
        //if previous state is not pressed 
        //and current state is not pressed, do nothing
        break;
        case PRESSED:   //if previous state was pressed
            if(current_state == PRESSED){   //and current state is pressed
                current_state = DEBOUNCED;  //current state becomes debounced
            }
        break;
        case DEBOUNCED: //if previous state was debounced
            if(current_state == PRESSED){   //if button is still pressed
                current_state = DEBOUNCED;  //button is still debounced
            }else{
                current_state = RELEASED;
            }
        break;
        case RELEASED:
            if (current_state == PRESSED){
                current_state = NOT_PRESSED; //ignore fast double tap //possiblecause of issue
            }else{
                button_control();
            }
        break;
        default:
            PrintQueue.call(printf,"%s previous state %d\t current state: %d\r\n",name, previous_state, current_state);
            return -1;  //return error code if something strange happens
        break;

    }
    previous_state = current_state; //update previous state
    return 0;
}