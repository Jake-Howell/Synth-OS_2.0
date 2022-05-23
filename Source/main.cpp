#include "mbed.h"
#include "AudioFX.h"
#include "Threads.h"
#include "IO-Devices.h"
#include "BM_DAC.hpp"
#include "rtos/ThisThread.h"
#include "HW_Timer.h"
#include "cbuff.hpp"
#include "global_defs.hpp"
#include "MIDI_Decoder.hpp"
#include "RotaryEncoder.h"
#include <cstdint>
#include <cstdio>
#include <stdint.h>

DigitalOut LED(LED1);
UnbufferedSerial PC_Coms(USBTX, USBRX, 115200);
UnbufferedSerial RPi_Coms(PD_5, PD_6, 115200);


Circular_Buff<float> DAC_buffer(BUFFER_SIZE);

//Initalise bare metal DAC
BM_DAC dac('A', 5);
//Create Synthasizer
WaveGen Synth(SAMPLE_RATE);
//Create IO updater
IO_Checker DeviceUpdater;

//add effects to synthsizer 
Vibrato vib(&Synth, 0.0f, 0.02f);  //vibrato @ 10Hz, +-2% frequency 
Tremolo trem(&Synth, 0.0f, 0.02f);    //Tremolo @ 10Hz +- 2% gain

MIDI Midi;

//set up threads
Thread SampleProducerThread(osPriorityHigh, OS_STACK_SIZE, nullptr, "Sample Producer");
Thread PrintThread(osPriorityNormal, OS_STACK_SIZE, nullptr, "Print Thread");
Thread MIDI_Thread(osPriorityNormal, OS_STACK_SIZE, nullptr, "MIDI Thread");
Thread IOCheckThread(osPriorityNormal, OS_STACK_SIZE, nullptr, "IO Check Thread");
EventQueue PrintQueue;

//set up IO Devices
RotaryEncoder   RE_D(&DeviceUpdater, RE_D_Pins, &vib, LFO_GAIN, "Vibrato Gain control knob");
RotaryEncoder   RE_C(&DeviceUpdater, RE_C_Pins, &vib, LFO_RATE, "Vibrato Rate control knob");
WaveSelector    SineButton(&Synth, &DeviceUpdater, SIN, PC_8, "Sine Wave button");
WaveSelector    TriangleButton(&Synth, &DeviceUpdater, TRI, PC_9, "Triangle Wave button");
WaveSelector    SawToothButton(&Synth, &DeviceUpdater, SAW, PC_10, "SawTooth Wave button");
WaveSelector    SquareButton(&Synth, &DeviceUpdater, SQU, PC_11, "Square Wave button");


//ISR Callback functions
void getUserInput();    
void HW_TIMER_Callback();


int main()
{
    INIT_GLOBAL_FLAG();
    SystemCoreClockUpdate();
    printf("\r\nSystem Core Clck:\t%d MHz\r\n", (SystemCoreClock/1000000));
    Synth.setWaveType(TRI);
    //Synth.pressNote(50,100);
    Synth.pressNote(73,100);
    RPi_Coms.attach(&getUserInput, SerialBase::RxIrq);   //call get user input on usart rx interupt 
    DAC_buffer.setThreshold((BUFFER_SIZE-10));

    start_threads();

    init_HWTimer_ISR(SAMPLE_RATE);
    NVIC_SetVector(HW_TIMER_IRQn, (uint32_t)&HW_TIMER_Callback); //point HW Timer to ISR

    while (true) {
        sleep();
    }
}


void HW_TIMER_Callback(){
    static uint16_t sample = 0;
    bool err;
    //outputStreamThread.flags_set(SAMPLE_FLAG);
    //PULSE_GLOBAL_FLAG();
    sample = (uint16_t)(4095*DAC_buffer.nbGet(&err));
    //sample = (sample + 1)  & 0x0FFF;
    dac.write(sample);
    if(DAC_buffer.belowThreshold()){
        SampleProducerThread.flags_set(BUFFER_BELOW_THRESHOLD);
    }
    HW_TIMER_MODULE->SR &= ~TIM_SR_UIF;    //clear interupt flag
    //DAC->DHR12R2=(sample & 0x0FFF);
}

//Interrupt ISR Callback
void getUserInput(){
    char d;

    RPi_Coms.read(&d, 1);    //copy data from UART Reg to char d
    //PC_Coms.write(&d, 1);
    //PrintQueue.call(printf, "MIDI Data: %d\r\n", d);
    
    Midi.serialBuffer.put(d);   //put data onto buffer
    
    
    if (Midi.serialBuffer.size() >= 4){
        MIDI_Thread.flags_set(MIDI_DATA_READY);   //signal MIDI Converter, to start converting data
    }
}
