#ifndef F_BUFF
#define F_BUFF
#include "mbed.h"
#include <cstdint>
#include <stdint.h>

class FlipFlopBuffer{
    public:
        FlipFlopBuffer(uint16_t capacity){
            this->capacity = capacity;
            buffer[0] = new uint16_t [capacity];
            buffer[1] = new uint16_t [capacity];                               //allocate memory for buffer
        }
        void put(uint16_t data){
            mutex_b1.lock();
            if(write_pos < capacity){
                buffer 
                //buffer1[write_pos++] = data; //write data and increment index
            }else{
                write_pos = 0;              //reset write position
            }
            buffer[write_pos];
            mutex_b1.unlock();
        }
        uint16_t get(){

        }
        void flipFlop(){    //swap read and write buffer
            uint16_t* temp;

            temp = buffer[0];
            buffer[0] = buffer[1];
            buffer[1] = temp;
            read_buffer = buffer[0];
            write_buffer = buffer[1];
        }
        bool readThreshold(){
            if ((double)read_pos/capacity > threshold){
                return false;
            }else{
                return true;
            }
        }

    private:
        Mutex mutex_b1, mutex_b0;
        uint16_t capacity;
        uint16_t *buffer[2];
        uint16_t *read_buffer;
        uint16_t *write_buffer;
        int read_pos, write_pos;
        float threshold = 0.5;
        
};


#endif