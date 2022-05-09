#include "BM_DAC.hpp"

	
BM_DAC::BM_DAC(char port, unsigned short pin){
	this->port = port;
	this->pin = pin;
	init_DAC();
}


void BM_DAC::init_DAC(void)
{
	GPIO_TypeDef dac_port; 
	switch(port){
		case 'A':
			RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;			//clock port A enable
			GPIOA->MODER|=(3u<<(2*this->pin));			//DAC output pin set as anaglogue
			break;
		case 'B':
			RCC->AHB1ENR|=RCC_AHB1ENR_GPIOBEN;			//clock port B enable
			GPIOB->MODER|=(3u<<(2*this->pin));			//DAC output pin set as anaglogue
			break;
		case 'C':
			RCC->AHB1ENR|=RCC_AHB1ENR_GPIOCEN;			//clock port C enable
			GPIOC->MODER|=(3u<<(2*this->pin));			//DAC output pin set as anaglogue
			break;
		case 'D':
			RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN;			//clock port D enable
			GPIOD->MODER|=(3u<<(2*this->pin));			//DAC output pin set as anaglogue
			break;
		case 'E':
			RCC->AHB1ENR|=RCC_AHB1ENR_GPIOEEN;			//clock port E enable
			GPIOE->MODER|=(3u<<(2*this->pin));			//DAC output pin set as anaglogue
			break;
		case 'F':
			RCC->AHB1ENR|=RCC_AHB1ENR_GPIOFEN;			//clock port F enable
			GPIOF->MODER|=(3u<<(2*this->pin));			//DAC output pin set as anaglogue
			break;
		case 'G':
			RCC->AHB1ENR|=RCC_AHB1ENR_GPIOGEN;			//clock port G enable
			GPIOG->MODER|=(3u<<(2*this->pin));			//DAC output pin set as anaglogue
			break;
	}	
	
	RCC->APB1ENR	|=RCC_APB1ENR_DACEN;			//DAC clock enable
	DAC->CR				|=DAC_CR_EN2;							//DAC 2 enabled
}

void BM_DAC::write(unsigned short data){
	DAC->DHR12R2=(data & 0x0FFF);					//write data 12 bits to DAC 2 output register
}
