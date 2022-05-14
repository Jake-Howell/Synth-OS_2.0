#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include "mbed.h"
#include <cstdint>

#define I2S_MODULE SPI2

#define GLOBAL_FLAG_PORT GPIOD
#define GLOBAL_FLAG_PIN    3

//Event Flags
#define BUFFER_BELOW_THRESHOLD  1
#define SAMPLE_FLAG             2
#define MIDI_DATA_READY         4

#define SAMPLE_RATE 50000
#define BUFFER_SIZE 4096

#define KEY_COUNT 10


inline void INIT_GLOBAL_FLAG() {GLOBAL_FLAG_PORT->MODER &= ~(3u<<(GLOBAL_FLAG_PIN*2));GLOBAL_FLAG_PORT->MODER |=  (1u<<(GLOBAL_FLAG_PIN*2));GLOBAL_FLAG_PORT->PUPDR |= PullDown;}
inline void PULSE_GLOBAL_FLAG()     {GLOBAL_FLAG_PORT->BSRR |= (1u<<GLOBAL_FLAG_PIN); GLOBAL_FLAG_PORT->BSRR |= (1u<<(GLOBAL_FLAG_PIN + 16));}
inline void TOGGLE_GLOBAL_FLAG()    {GLOBAL_FLAG_PORT->ODR ^= (1u<<GLOBAL_FLAG_PIN);}
typedef enum {ATTACK = 0, DECAY, SUSTAIN, RELEASE, OFF}ASDR_STATE_t;
typedef enum {SIN = 0, TRI, SAW, SQU}WAVE_TYPE;	
typedef struct {
    uint32_t attack_steps;
    uint32_t decay_steps;
    uint32_t release_steps;
    float attack_gain;
    float sustain_gain;
}EnvParams_t;
typedef struct{
    bool    active;         //if note is not active, skip any calculations
    float  angularStep;    //calculated angular step per sample
    float  Wo;             //angle
    char    MIDInum;        //MIDI code for note
    float  period_us;      //wavelength
    float  velocity;       //loudness of note (gain)
    
}toneParams;
typedef struct{
	char type;
	char param1;
	char param2;
} MIDI_cmd_t;

typedef struct{
    uint8_t d0;
    uint8_t d1;
    uint8_t d2;
    uint8_t cs; //checksum
}MIDI_Serial_Bloak_t;

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
    PB_15,              //I2S2_SDO  AF5
    PB_12,              //I2S2_LR   AF5
    PC_6,               //I2SS_Mclk AF5
    PB_13               //I2S2_Bclk AF5
};

const SPI_Pins adc {
PB_5,
PA_6,
PA_5,
PD_14
};

typedef struct {
    PinName D0;
    PinName D1;
    PinName D2;
    PinName D3;
    PinName D4;
    PinName D5;
    PinName D6;
    PinName D7;
}RE_Pins;

const RE_Pins RE_D_Pins{
    PD_2,           //D1
    PE_5,           //D2
    PF_8,           //D3
    PF_9,           //D4
    PD_10,           //D5
    PF_7,           //D6
    PE_3,           //D7
    PE_4           //D8
};


#define SILENT_NOTE 0xFF

#endif