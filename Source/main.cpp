#include "mbed.h"
#include "global_defs.hpp"
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
UnbufferedSerial PC_Coms(USBTX, USBRX);



SPI audio_in(adc.MOSI, adc.MISO, adc.SCLK, adc.CS);



Circular_Buff<float> DAC_buffer(1024);

//AnalogOut dac(PA_5);
BM_DAC dac('A', 5);
WaveGen Synth(SAMPLE_RATE);
MIDI Midi;

RotaryEncoder RE_D(RE_D_Pins);

Thread PrintThread, SampleProducerThread(osPriorityHigh), MIDI_Thread, IOCheckThread;
osThreadId_t PrintThreadID, mainThreadID, SampleProducerThreadID, outputStreamThreadID, MIDI_ThreadID;


EventQueue PrintQueue;
Ticker sampleTimer;

//Callback functions
void printer();
void sampleGen();   //SampleProducer thread callback
void outputSample();    //outputStream thread callback
void getUserInput();    //usart interupt callback
void MIDI_Converter();  //main thread
void updateIO();
void timerCallback();
void HW_TIMER_Callback();

int main()
{
    INIT_GLOBAL_FLAG();
    //TOGGLE_GLOBAL_FLAG();
    
    /*
    while(1){
        dac.write(1.0f);
        ThisThread::sleep_for(10ms);
        dac.write(0.0f);
        ThisThread::sleep_for(10ms);
    }
    //*/

    //init_HWTimer_ISR();
    mainThreadID = ThisThread::get_id();
    PC_Coms.baud(115200);
    SystemCoreClockUpdate();
    printf("\r\nSystem Core Clck:\t%d MHz\r\n", (SystemCoreClock/1000000));

    Synth.pressNote(55, 120);
    Synth.pressNote(70,100);
    PC_Coms.attach(&getUserInput, SerialBase::RxIrq);   //call get user input on usart rx interupt 


    //start print queue
    PrintThread.start(printer);


    //start updating IO 
    IOCheckThread.start(updateIO);

    //start converting MIDI data to control device
    MIDI_Thread.start(MIDI_Converter);
    
    //start producing samples (High Priority thread)
    SampleProducerThread.start(sampleGen);              //start sample producer thread
    
    //start outputting samples (High Priority thread)
    //outputStreamThread.start(outputSample);                 //start thread to output to DAC
    //*/
    //sampleTimer.attach(HW_TIMER_Callback, 10us);

    init_HWTimer_ISR(SAMPLE_RATE);
    NVIC_SetVector(HW_TIMER_IRQn, (uint32_t)&HW_TIMER_Callback); //point HW Timer to ISR

    //oscR WAS HWERE AND STRUGGLING TO TYPE
    while (true) {
        sleep();
    }
}


void HW_TIMER_Callback(){
    static uint16_t sample = 0;
    bool err;
    HW_TIMER_MODULE->SR &= ~TIM_SR_UIF;    //clear interupt flag
    //outputStreamThread.flags_set(SAMPLE_FLAG);
    PULSE_GLOBAL_FLAG();
    sample = (uint16_t)(4095*DAC_buffer.nbGet(&err));
    //sample = (sample + 1)  & 0x0FFF;
    //dac.write(sample);
    DAC->DHR12R2=(sample & 0x0FFF);
}

void updateIO(){
    while(1){
        RE_D.update_pos();
        uint8_t pos = RE_D.getPos();
        uint8_t GC = RE_D.getGC();
        //PrintQueue.call(printf,"RE_D Pos: %d\tGC: %d\r\n", pos, GC);
        ThisThread::sleep_for(50ms);
    }
}


//Interrupt ISR Callback
void getUserInput(){
    char d;

    PC_Coms.read(&d, 1);    //copy data from UART Reg to char d
    PC_Coms.write(&d, 1);
    MIDI_Thread.flags_set(1);   //signal MIDI Converter, to start converting data
    Midi.serialBuffer.put(d);   //put data onto buffer
}


//convert serial data into midi commands
void MIDI_Converter(){
    MIDI_ThreadID = ThisThread::get_id();
    char d;
    MIDI_cmd_t cmd;
    while(true){  //loop until buffer is empty
        //wait for signal to produce more samples
        ThisThread::flags_wait_any(1);
        while(!Midi.serialBuffer.isEmpty()){  //while MIDI buffer not empty
            d = Midi.serialBuffer.get();      //get char of midi buffer
            cmd = Midi.pc_keyMap(d);          //turn PC keyboard chars into midi data and return midi cmd
        }
        PrintQueue.call(printf, "Note: %d Vel: %d\r\n", cmd.param1, cmd.param2);
        Synth.readMIDI(cmd);    //send midi data to synth
    }
}


//sample producer thread callback
void sampleGen(){    //FIFO Producer Thread
    SampleProducerThreadID = ThisThread::get_id();
    PrintQueue.call(printf, "Starting sampleGen\r\n");
    
    //SampleProducerThread.flags_set(1);  //on boot, fill buffer
    float sample;
    while(true){
        //wait for outputStream to ask for more samples
        
        //PrintQueue.call(printf, "SampleGen producing samples\r\n");
        while(!DAC_buffer.isFull()){    //loop until dac buffer is full
            sample = Synth.produceSample();
            DAC_buffer.put(sample);               //put sample on buffer
        }
        PrintQueue.call(printf, "DAC buffer full. Latest Sample: %5.4f\r\n", sample);
        //dac.write(sample);
        ThisThread::flags_clear(BUFFER_BELOW_THRESHOLD);     //once buffer is full, clear flag
        ThisThread::flags_wait_any(BUFFER_BELOW_THRESHOLD);  //once buffer is full, sleep until buffer threshold flag set
    }
}


//outputSample callback for sample event queue
void outputSample(){    //Consumer Thread
    float OutSample;
    bool err;
    outputStreamThreadID = ThisThread::get_id();
    PrintQueue.call(printf, "Starting output\r\n");
    while(true){
        ThisThread::flags_wait_all(SAMPLE_FLAG);
        continue;
        OutSample = DAC_buffer.nbGet(&err);

        //PrintQueue.call(printf, "Dac Sample:\t%5.4f\r\n", OutSample);
        //TOGGLE_GLOBAL_FLAG();
        //dac.write(OutSample);
        if (DAC_buffer.belowThreshold()){ //if output buffer is too small, signal sampleGen to produce more samples 
            SampleProducerThread.flags_set(BUFFER_BELOW_THRESHOLD); //signal sampleGen to produce more samples
            //PrintQueue.call(printf, "sampleGen Signaled\r\n");
        }
        if (err){
            LED = 1;
        }
    }
}



void printer(){
    PrintThreadID = ThisThread::get_id();
    PrintQueue.call(printf, "Starting printer\r\n");
    PrintQueue.dispatch_forever();
}



