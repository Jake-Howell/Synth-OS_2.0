#ifndef _BUTTONS_H_
#define _BUTTONS_H_
#include "mbed.h"
#include <cstdint>
#include "IO_Interface.h"
#include "IO_checker.h"
typedef enum {NOT_PRESSED = 0, PRESSED, DEBOUNCED, RELEASED}ButtonState_t;

class Button : public DigitalIn, public IO {
    public:
        Button(IO_Checker* DeviceChecker, PinName pin, const char * Name);
        //force programmer to implement control for button
        virtual void button_control() = 0;  //create pure virtual function to be overriden
        const char * getName() override;
        int update_IO() override;  //override IO interface functions

    private:
        IO_Checker* DeviceChecker;
        const char * name;
        ButtonState_t current_state;
        ButtonState_t previous_state;
};

#endif