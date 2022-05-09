#include "Threads.h"
extern Thread PrintThread, IOCheckThread, MIDI_Thread, SampleProducerThread;
extern osThreadId_t PrintThreadID, mainThreadID, SampleProducerThreadID, outputStreamThreadID, MIDI_ThreadID;
extern Circular_Buff<float> DAC_buffer;
extern WaveGen Synth;
extern MIDI Midi;
extern RotaryEncoder RE_D;

void start_threads(){
    //start print queue
    PrintThread.start(printer);
    //start updating IO 
    IOCheckThread.start(updateIO);
    //start converting MIDI data to control device
    MIDI_Thread.start(MIDI_Converter);
    //start producing samples (High Priority thread)
    SampleProducerThread.start(sampleGen);              //start sample producer thread
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
            TOGGLE_GLOBAL_FLAG();
        }
        
        ThisThread::flags_clear(BUFFER_BELOW_THRESHOLD);     //once buffer is full, clear flag
        ThisThread::flags_wait_all(BUFFER_BELOW_THRESHOLD);  //once buffer is full, sleep until buffer threshold flag set
    }
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

void printer(){
    PrintThreadID = ThisThread::get_id();
    PrintQueue.call(printf, "Starting printer\r\n");
    PrintQueue.dispatch_forever();
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