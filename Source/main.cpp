#include "mbed.h"
#include "global_defs.hpp"
#include "Threads.h"
//#include "i2S.h"
#include "BM_DAC.hpp"
#include "rtos/ThisThread.h"
#include "HW_Timer.h"
#include "cbuff.hpp"
#include "waveGen.cpp"
#include "MIDI_Decoder.hpp"
#include "RotaryEncoder.h"
#include <cstdint>
#include <cstdio>
#include <stdint.h>
DigitalOut LED(LED1);
UnbufferedSerial PC_Coms(USBTX, USBRX, 115200);
UnbufferedSerial RPi_Coms(PD_5, PD_6, 115200);

SPI audio_in(adc.MOSI, adc.MISO, adc.SCLK, adc.CS);

Circular_Buff<float> DAC_buffer(BUFFER_SIZE);

//AnalogOut dac(PA_5);
BM_DAC dac('A', 5);
WaveGen Synth(SAMPLE_RATE);
MIDI Midi;

RotaryEncoder RE_D(RE_D_Pins);

Thread PrintThread, SampleProducerThread(osPriorityHigh), MIDI_Thread, IOCheckThread;
osThreadId_t PrintThreadID, mainThreadID, SampleProducerThreadID, outputStreamThreadID, MIDI_ThreadID;

EventQueue PrintQueue;
Ticker sampleTimer;

//ISR Callback functions
void getUserInput();    
void HW_TIMER_Callback();


int main()
{
    INIT_GLOBAL_FLAG();

    mainThreadID = ThisThread::get_id();
    SystemCoreClockUpdate();
    printf("\r\nSystem Core Clck:\t%d MHz\r\n", (SystemCoreClock/1000000));

    //8Synth.pressNote(50,100);

//    Synth.pressNote(73,100);
    RPi_Coms.attach(&getUserInput, SerialBase::RxIrq);   //call get user input on usart rx interupt 
    DAC_buffer.setThreshold((BUFFER_SIZE-1));

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
