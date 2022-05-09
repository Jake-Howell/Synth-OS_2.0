#ifndef _THREADS_H_
#define _THREADS_H_
#include "mbed.h"
#include "rtos/ThisThread.h"
#include "HW_Timer.h"
#include "cbuff.hpp"
#include "waveGen.cpp"
#include "MIDI_Decoder.hpp"
#include "RotaryEncoder.h"



void start_threads();

void sampleGen();
void MIDI_Converter();
void printer();
void updateIO();

#endif
