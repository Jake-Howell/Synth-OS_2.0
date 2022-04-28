#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include "mbed.h"
typedef struct{
	char type;
	char param1;
	char param2;
} MIDI_cmd_t;

typedef struct{
	char num;
	char velocity;
}note_t;

enum MIDI_TYPES
{
    NOTE_ON = 1,
    NOTE_OFF,
    CONTROL_CHANGE,
    PITCHWHEEL
};

typedef struct {
    PinName SDAT;
    PinName LRck;
    PinName Bclk;
}I2S_Pins;

typedef struct{
    PinName MOSI;
    PinName MISO;
    PinName SCLK;
    PinName CS;
}SPI_Pins;

const I2S_Pins ext_dac{
    PB_15,
    PB_12,
    PC_6,
};

const SPI_Pins adc {
PB_5,
PA_6,
PA_5,
PD_14
};

#define SILENT_NOTE 0xFF

#endif