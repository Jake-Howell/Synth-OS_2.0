#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include "mbed.h"

#define I2S_MODULE SPI2
#define HW_TIMER_MODULE TIM17
#define GLOBAL_FLAG_PORT GPIOD
#define GLOBAL_FLAG_PIN    6
//#define PULSE_GLOBAL_FLAG()     {FLAG_PORT->BSRR |= (1u<<FLAG_PIN); FLAG_PORT->BSRR |= (1u<<(FLAG_PIN + 16));}
inline void PULSE_GLOBAL_FLAG()     {GLOBAL_FLAG_PORT->BSRR |= (1u<<GLOBAL_FLAG_PIN); GLOBAL_FLAG_PORT->BSRR |= (1u<<(GLOBAL_FLAG_PIN + 16));}

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
    PinName Mclk;
    PinName Bclk;
}I2S_Pins;

typedef struct{
    PinName MOSI;
    PinName MISO;
    PinName SCLK;
    PinName CS;
}SPI_Pins;

const I2S_Pins ext_dac{
    PB_15,              //I2S2_SDO AF5
    PB_12,              //I2S2_LR  AF5
    PC_6,               //
    PB_13               //I2S2_Ck  AF5
};

const SPI_Pins adc {
PB_5,
PA_6,
PA_5,
PD_14
};



#define SILENT_NOTE 0xFF

#endif