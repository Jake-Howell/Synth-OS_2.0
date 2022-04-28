#ifndef _I2SI_H_
#define _I2SI_H_
#include "mbed.h"
#include <cstdint>
#include <stdint.h>

class I2S{
    public:
        I2S(PinName sdat_pin, PinName LRsel_pin, PinName clk_pin):sdat(sdat_pin), LRsel(LRsel_pin), clk(clk_pin){

        }
        void write(uint16_t R_data, uint16_t L_data){
            LRsel = RIGHT; //send right sample first

            for (int i = 0; i<16; i++){        //loop through 16 bit data lsb first
                clk = 0;
                sdat = (((R_data)&(1u<<i))>>i); //mask with (1 << i) to extract bit at position i, then shift bit to pos 0 before writing output.
                pulse_clk();
            }
            for (int i = 0; i<16; i++){        //loop through 16 bit data lsb first
                sdat = (((L_data)&(1u<<i))>>i); //mask with (1 << i) to extract bit at position i, then shift bit to pos 0 before writing output.
                pulse_clk();
            }

        }

        void pulse_clk(){
            wait_ns(100);  
            clk = 1;
            wait_ns(100);            
            clk = 0;
        }
    private:
        enum LR_SELECT{LEFT = 0, RIGHT}; //enum to describe left and right output for sterio output
        DigitalOut sdat, LRsel, clk;
        uint16_t data;



};

#endif