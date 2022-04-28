#include "mbed.h"
#include "global_defs.hpp"
#include "i2S.h"
#include "rtos/ThisThread.h"
#include "HW_Timer.h"
#include "cbuff.hpp"
#include "waveGen.cpp"
#include "MIDI_Decoder.hpp"
#include <cstdint>
#include <cstdio>
#include <stdint.h>
DigitalOut LED(LED1);
UnbufferedSerial PC_Coms(USBTX, USBRX);

bool SAMPLE_FLAG = 0;

SPI audio_in(adc.MOSI, adc.MISO, adc.SCLK, adc.CS);



Circular_Buff<uint16_t> DAC_buffer(1024);
Timer sampleTimer_us;
AnalogOut dac(PA_5);
WaveGen Synth;
MIDI Midi;
I2S AUDIO_OUT(ext_dac.SDAT, ext_dac.LRck, ext_dac.Bclk);
//Ticker sampleTimer;
Thread PrintThread, SampleProducerThread, outputStreamThread, MIDI_Thread;
osThreadId_t mainThreadID, PrintThreadID, SampleProducerThreadID, outputStreamThreadID, MIDI_ThreadID;


EventQueue PrintQueue;


//Callback functions
void printer();
void sampleGen();   //SampleProducer thread callback
void outputSample();    //outputStream thread callback
void getUserInput();    //usart interupt callback
void MIDI_Converter();  //main thread

int main()
{
    init_HWTimer_ISR(7, 186); //tested for 96KHz opperation

    mainThreadID = ThisThread::get_id();
    PC_Coms.baud(115200);
    SystemCoreClockUpdate();
    printf("\r\nSystem Core Clck:\t%d MHz\r\n", (SystemCoreClock/1000000));
    int psc = (SystemCoreClock/1000) - 1; //devide clock frequency by 1,000 to get 1ns
    int arr = 10467 -1; //multiply 1ns by 10,467 to get a period of 10.47us, roughly 96KHz

    //init_HWTimer_ISR(psc, arr);
    PC_Coms.attach(&getUserInput, SerialBase::RxIrq);   //call get user input on usart rx interupt 
    PrintThread.start(printer);

    MIDI_Thread.start(MIDI_Converter);
    MIDI_Thread.set_priority(osPriorityHigh);
    
    SampleProducerThread.start(sampleGen);              //start sample producer thread
    SampleProducerThread.set_priority(osPriorityHigh);
    
    outputStreamThread.start(outputSample);                 //start thread to output to DAC
    outputStreamThread.set_priority(osPriorityHigh);   //output must be highest priority to reduce smaple jitter

    while (true) {
        sleep();
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
        while(!Midi.serialBuffer.isEmpty()){
            d = Midi.serialBuffer.get();      
            cmd = Midi.pc_keyMap(d);          //turn keyboard chars into midi data and return midi cmd
        }
        PrintQueue.call(printf, "Note: %d Vel: %d", cmd.param1, cmd.param2);
        Synth.readMIDI(cmd);    //add midi data to synth
    }
}


//sample producer thread callback
void sampleGen(){
    SampleProducerThreadID = ThisThread::get_id();
    PrintQueue.call(printf, "Starting sampleGen\r\n");
    //SampleProducerThread.flags_set(1);  //on boot, fill buffer
    while(true){
        //wait for outputStream to ask for more samples
        
        //PrintQueue.call(printf, "SampleGen producing samples\r\n");
        printf("Producing samples\r\n");
        while(!DAC_buffer.isFull()){    //loop until dac buffer is full
            uint16_t out_16Bit = (uint16_t)Synth.produceSample();
            DAC_buffer.put(out_16Bit);               //put sample on buffer
        }
        //PrintQueue.call(printf, "DAC buffer full\r\n");
        printf("DAC Buffer Full\r\n");
        ThisThread::flags_wait_any(1);
        ThisThread::flags_clear(1);     //once buffer is full, clear flag
    }
}

//TODO make this work id us_ticker
//highest priority, outputStream thread callback
void outputSample(){
    outputStreamThreadID = ThisThread::get_id();
    uint16_t d;
    DAC_buffer.setThreshold(512);       //set threshold, if size is less than threshold, signal sampleGen to produce more samples
    PrintQueue.call(printf, "Starting outputSample\r\n");

    while(true){
        
        if (DAC_buffer.belowThreshold()){ //if output buffer is too small, signal sampleGen to produce more samples 
            SampleProducerThread.flags_set(1); //signal sampleGen to produce more samples
            PrintQueue.call(printf, "sampleGen Signaled\r\n");
        }

        //wait_ns(10466);
        d = DAC_buffer.get();
        //dac.write(d); //update output
        while(!SAMPLE_FLAG);
        AUDIO_OUT.write(3000,3000);
//        PC_Coms.write(&d, 2);
//        PrintQueue.call(printf, "%d",d);
    }
}

void printer(){
    PrintThreadID = ThisThread::get_id();
    PrintQueue.call(printf, "Starting printer\r\n");
    PrintQueue.dispatch_forever();
}


