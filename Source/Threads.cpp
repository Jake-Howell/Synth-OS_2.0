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
    PrintQueue.call(printf, "Starting Midi Converter\r\n");
    MIDI_cmd_t cmd;
    MIDI_Serial_Bloak_t chunk;  //store 4 bytes of midi data to convert to a command
    while(true){  //loop until buffer is empty
        //wait for signal to decode more cmds
        ThisThread::flags_wait_all(MIDI_DATA_READY);
        while(Midi.serialBuffer.size() >= 4){           //while MIDI buffer not empty
            chunk.d0 = Midi.serialBuffer.get();         //get char of midi buffer
            chunk.d1 = Midi.serialBuffer.get();         //get char of midi buffer
            chunk.d2 = Midi.serialBuffer.get();         //get char of midi buffer
            chunk.cs = Midi.serialBuffer.get();         //get char of midi buffer 
            Midi.serialToMIDIconverter(chunk);
            //cmd = Midi.pc_keyMap(d);          //turn PC keyboard chars into midi data and return midi cmd
        }
        if(!(Midi.cmdBuffer.isEmpty())){    //if data has been converted and tansmitted correctly
            cmd = Midi.cmdBuffer.get();     //get cmd
            Synth.readMIDI(cmd);            //send midi cmd to synth
        }
        
    }
}

void printer(){
    PrintQueue.call(printf, "Starting printer\r\n");
    PrintQueue.dispatch_forever();
}

void updateIO(){
    PrintQueue.call(printf, "Starting IO Check\r\n");
    while(1){
        RE_D.update_pos();
        uint8_t pos = RE_D.getPos();
        uint8_t GC = RE_D.getGC();
        //PrintQueue.call(printf,"RE_D Pos: %d\tGC: %d\r\n", pos, GC);
        ThisThread::sleep_for(50ms);
    }
}