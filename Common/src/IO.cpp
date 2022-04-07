#ifdef __cplusplus
extern "C" {
#endif
#include "stm32h7xx.h"                  // Device header

#include "../inc/IO.hpp"
	
IO::IO(GPIO_TypeDef * port, unsigned short pin, unsigned short mode){
	init();
}
IO::~IO(){
	//reset all registers
	reset_MODE();
	reset_PUPDR();
	reset_OSPEEDR();
	reset_OTYPER();
	reset_AF();
}

void IO::write(bool value){
		mPORT->ODR = value;
}

bool IO::read(){
	bool value;
	value = mPORT->IDR;
	return value;
}

void IO::init(){
	//turn on all ports
	RCC->AHB4ENR|=RCC_AHB4ENR_GPIOAEN;			//clock port A enable
	RCC->AHB4ENR|=RCC_AHB4ENR_GPIOBEN;			//clock port B enable
	RCC->AHB4ENR|=RCC_AHB4ENR_GPIOCEN;			//clock port C enable
	RCC->AHB4ENR|=RCC_AHB4ENR_GPIODEN;			//clock port D enable	
	RCC->AHB4ENR|=RCC_AHB4ENR_GPIOEEN;			//clock port E enable			
	RCC->AHB4ENR|=RCC_AHB4ENR_GPIOFEN;			//clock port F enable			
	RCC->AHB4ENR|=RCC_AHB4ENR_GPIOGEN;			//clock port G enable			
	RCC->AHB4ENR|=RCC_AHB4ENR_GPIOHEN;			//clock port G enable			
	set_MODE(mMODE);													//once AHB is enabled, set IO mode
}

void IO::set_AF(unsigned short AF){
	this->mAF = AF;
	unsigned short AFRnum =  mPIN % 8; //select AHRL or AFRH register to write to
	unsigned int clear_bits = (0xF<<(4*(this->mPIN - (AF*8))));
	unsigned short set_bits = (AF<<(4*(this->mPIN - (AF*8))));
	
	mPORT->AFR[AFRnum] &=~(clear_bits);
	mPORT->AFR[AFRnum] |= set_bits;

}

void IO::reset_AF(){
		unsigned short AFRnum =  mPIN % 8; //select AHRL or AFRH register to write to
		unsigned int clear_bits = (0xF<<(4*(this->mPIN - (mAF*8))));
		mPORT->AFR[AFRnum] &=~(clear_bits);
}

void IO::set_MODE(unsigned short MODE){
	unsigned int clear_bits = (3u<<(2*this->mPIN));
	unsigned short set_bits = (MODE<<(2*this->mPIN));

	mPORT->MODER &=~(clear_bits);
	mPORT->MODER |= set_bits;

}

void IO::reset_MODE(){
	unsigned int clear_bits = (3u<<(2*this->mPIN));
	mPORT->MODER &=~(clear_bits);
}

void IO::set_OTYPER(unsigned short TYPE){
	this->mTYPE = TYPE;
	unsigned int clear_bits = 1u<<(this->mPIN);
	unsigned int set_bits = TYPE<<(this->mPIN);
	mPORT->OTYPER &=~clear_bits;
	mPORT->OTYPER |= set_bits;
}
void IO::reset_OTYPER(){
	unsigned int clear_bits = 1u<<(this->mPIN);
	mPORT->OTYPER &=~(clear_bits);
}

void IO::set_OSPEEDR(unsigned short SPEED){
	this->mSPEED = SPEED;
	unsigned int clear_bits = 3u<<(2*(this->mPIN));
	unsigned int set_bits = SPEED<<(2*(this->mPIN));

	mPORT->OSPEEDR &=~(clear_bits);		//clear bits
	mPORT->OSPEEDR |= (set_bits);			//set bits
}

void IO::reset_OSPEEDR(){
	unsigned int clear_bits = 3u<<(2*(this->mPIN));
	mPORT->OSPEEDR &=~(clear_bits);		//clear bits
}



void IO::set_PUPDR(unsigned short UPDOWN){
	this->mUPDOWN = UPDOWN;
	unsigned int clear_bits = 3u<<(2*(this->mPIN));
	unsigned int set_bits 	= UPDOWN<<(2*(this->mPIN));

	GPIOA->PUPDR &=~(clear_bits);		//clear bits
	GPIOA->PUPDR |= (set_bits);			//set bits
}

void IO::reset_PUPDR(){
	unsigned int clear_bits = 3u<<(2*(this->mPIN));

	GPIOA->PUPDR &=~(clear_bits);		//clear bits
}


#ifdef __cplusplus
}
#endif
