#ifndef _I2SI_H_
#define _I2SI_H_
#include "mbed.h"
#include <cstdint>
#include <stdint.h>
#include "global_defs.hpp"

typedef enum {LEFT = 0, RIGHT} LR_SELECT; //enum to describe left and right output for sterio output
extern I2S_HandleTypeDef hi2s2;

class I2S{
    public:
        I2S(PinName sdat_pin, PinName LRsel_pin, PinName clk_pin):sdat(sdat_pin), LRsel(LRsel_pin), clk(clk_pin, 0){
            Init_Pins();
        }
        void MX_I2S2_Init(void){
            hi2s2.Instance = SPI2;
            hi2s2.Init.Mode = I2S_MODE_MASTER_TX;
            hi2s2.Init.Standard = I2S_STANDARD_PHILIPS;
            hi2s2.Init.DataFormat = I2S_DATAFORMAT_16B_EXTENDED;
            hi2s2.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
            hi2s2.Init.AudioFreq = I2S_AUDIOFREQ_44K;
            hi2s2.Init.CPOL = I2S_CPOL_LOW;
            hi2s2.Init.FirstBit = I2S_FIRSTBIT_MSB;
            hi2s2.Init.WSInversion = I2S_WS_INVERSION_DISABLE;
            hi2s2.Init.Data24BitAlignment = I2S_DATA_24BIT_ALIGNMENT_RIGHT;
            hi2s2.Init.MasterKeepIOState = I2S_MASTER_KEEP_IO_STATE_DISABLE;
            if (HAL_I2S_Init(&hi2s2) != HAL_OK)
            {
                while(1){
                    //Well, something went wrong here...
                }
            }
        }

        void Init_Pins(){
            
            RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN | RCC_AHB4ENR_GPIOCEN; //turn on GPIO busses

            GPIOB->MODER &= ~((3u<<(2*LRck))|(3u<<(2*SDAT)));
            GPIOB->MODER |= ((1u<<(2*LRck))|(1u<<(2*SDAT)));    //set SDAT and LRclk to output on port B
            GPIOB->PUPDR &= ~((3u<<(2*LRck)) | (3u<<(2*SDAT))); //reset PUPD resistor values for LRck and SDAT
            GPIOB->PUPDR |= (2u<<(2*LRck)) | (2u<<(2*SDAT));    //set pulldown resistor on LRck and SDAT
            GPIOB->OSPEEDR &= ~((3u<<(2*LRck)) | (3u<<(2*SDAT)));//reset OSPEEDER
            GPIOB->OSPEEDR |= ((3u<<(2*LRck)) | (3u<<(2*SDAT))); //set OSPEEDER to HIGH SPEED

            GPIOC->MODER &= ~(3u<<(2*Bclk));                    
            GPIOC->MODER |= (1u<<(2*Bclk));                     //set Bclk to output on port C
            GPIOC->PUPDR &= ~(3u<<(2*Bclk));                    //set pulldown resistor on Bclk
            GPIOC->PUPDR |= (2u<<(2*Bclk));                     //set pulldown resistor on Bclk
            GPIOC->OSPEEDR &= ~(3u<<(2*Bclk));                  //set OSPEEDER to FAST
            GPIOC->OSPEEDR |= (3u<<(2*Bclk));

        }

        void init_I2S(){
            //Code Derrived from section 55.9.7 I2S Start-up Sequence of the STM32-H7A3 Referance Manual (page 2141) 

            const uint8_t I2S_DIV = 2;
            RCC->APB1LENR |= RCC_APB1LENR_SPI3EN; //turn on perpherial clock
            I2S_MODULE->CR1 &= ~SPI_CR1_SPE; //reset Serial Peripheral Enable to unlock SPI Registers
            
            //Configure I2S Module
            I2S_MODULE->I2SCFGR |= SPI_I2SCFGR_MCKOE;  //set Master Clock Output Enable, so MClk is produced
            I2S_MODULE->I2SCFGR &= ~SPI_I2SCFGR_CHLEN; //reset chanel length to 0 - this sets chanel length to 16Bits wide
            I2S_MODULE->I2SCFGR &= ~(0xFF << 16);      //reset I2S DIV
            I2S_MODULE->I2SCFGR |= SPI_I2SCFGR_ODD;    //SET Odd scaling to bring closer to 96KHz Sampling
            I2S_MODULE->I2SCFGR |= (I2S_DIV << 16);    //set I2S DIV

            //Program the serial interface protocol
            I2S_MODULE->I2SCFGR &= ~SPI_I2SCFGR_CKPOL;  //reset CKPOL to 0 (clock polarity)
            I2S_MODULE->I2SCFGR &= ~SPI_I2SCFGR_WSINV;  //reset WS inital value to 0
            I2S_MODULE->CFG2 &= ~SPI_CFG2_LSBFRST;      //RESET LSB First to 0
            I2S_MODULE->I2SCFGR &= ~SPI_I2SCFGR_CHLEN;  //reset CHLEN to 0 for 16 bits per chanel
            I2S_MODULE->I2SCFGR &= ~(3u<<8);            //reset DATLEN to 0
            I2S_MODULE->I2SCFGR |= (2u<<8);             //set DATLEN to 2 (32 bit data length)
            I2S_MODULE->I2SCFGR &= ~(3u<<4);            //reset I2SSTD to 0 (Philips Standard)
            I2S_MODULE->I2SCFGR &= ~(7u<<1);            //reset I2SCFG to 0 
            I2S_MODULE->I2SCFGR |=  (2u<<1);            //set I2SCFG to 2 (master transmit)
            I2S_MODULE->I2SCFGR |= (1u<<0);            //set I2SMOD to 1 (MODE SELECTION = I2S)

            //Adjust FIFO threshold
            I2S_MODULE->CFG1 &= ~(15<<5);               //set 1 data frame per data packet

            //clear all Status Flag Registers
            I2S_MODULE->IFCR = 0;                  

            //enable interupt flags     
            I2S_MODULE->IER |= SPI_IER_TXPIE;           //enable TxP Interupt

            NVIC_EnableIRQ(SPI3_IRQn);



            //Start I2S Module
            I2S_MODULE->CR1 |= SPI_CR1_SPE;     //Set Serial Peripheral Enable to lock cfgr registers, and unlock data registers
            I2S_MODULE->CR1 |= SPI_CR1_CSTART;  //start an I2S communication
        }

        void write(uint16_t R_data, uint16_t L_data){
            LRsel = RIGHT; //send right sample first
            bool d;
            LR_write(RIGHT);
            for (int i = 0; i<16; i++){        //loop through 16 bit data lsb first
                d = (((R_data)&(1u<<i))>>i);
                sdat_write(d); //mask with (1 << i) to extract bit at position i, then shift bit to pos 0 before writing output.
                pulse_clk();
            }
            LR_write(LEFT);
            for (int i = 0; i<16; i++){        //loop through 16 bit data lsb first
                d = (((L_data)&(1u<<i))>>i);
                sdat_write(d); //mask with (1 << i) to extract bit at position i, then shift bit to pos 0 before writing output.
                pulse_clk();
            }
        }

    private:
        DigitalOut sdat, LRsel, clk;
        uint16_t data;
        const int SDAT = 15;
        const int LRck = 12;
        const int Bclk = 6;


        void sdat_write(bool d){
            GPIOB->BSRR |= (!d<<(SDAT + 16))|(d<<SDAT);
            //GPIOB->ODR |= (d<<SDAT);
        }
        void LR_write(LR_SELECT sel){
            GPIOB->BSRR = (!sel<<(LRck+16))|(sel<<LRck);
        }
        void pulse_clk(){
            wait_ns(1);  
            GPIOC->BSRR |= (1U<<Bclk);      //set Bclk
            wait_ns(1);            
            GPIOC->BSRR |= (1U<<(Bclk+16)); //reset Bclk
        }
};

#endif