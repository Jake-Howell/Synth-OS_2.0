#ifndef _DAC_ADC_H
#define _DAC_ADC_H

	
#include "mbed.h"
#include "math.h"		

//#define ADC_input_port			GPIOB	//define port for potentiometer
//#define ADC_input_L_pin				0			//define potentiometer pin number
//#define ADC_Channel				8			//define potentiometer chanel
//#define ADC_res 4096								//define resoloution of ADC (12-bit)


//											
//#define DAC_port	GPIOA							//define
//#define DAC_pin		5

class BM_DAC{
	
	public:
		BM_DAC(char port, unsigned short pin);
		void write(unsigned short data);
	
	private:
		void init_DAC(void);
		char port;
		unsigned short pin;
	
};

class AnalogIn{
	
	public:
		AnalogIn(char port, unsigned short pin, unsigned short chanel);
		unsigned short read(void);
	
	private:
		void init_ADC(void);								//initalise ADC
		char port;
		unsigned short pin;
		unsigned short chanel;
	
	
};

#endif
