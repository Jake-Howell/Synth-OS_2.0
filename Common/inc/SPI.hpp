#ifndef _SPI_H_
#define _SPI_H_
#ifdef __cplusplus
extern "C" {
#endif
#include "stm32h755xx.h"                  // Device header
#include "../inc/IO.hpp"
	
class SPI{
	public:
		//SPI(PinName MOSI, PinName MISO, PinName SCLK, PinName CS);
		uint8_t sendData(char data);
		uint8_t receiveData();
		void set_CS(bool state);
	
	private:
		IO m_MOSI;
		IO m_MISO;
		IO m_SCLK;
		IO m_CS;
};
	
	
#ifdef __cplusplus
}
#endif
#endif