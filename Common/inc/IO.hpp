#ifndef _IO_H__
#define _IO_H__


#include "stm32h7xx.h"                  // Device header

class IO{
	public:
		IO(GPIO_TypeDef * port, unsigned short pin, unsigned short mode);
		~IO();
		enum{Input_mode = 0, Output_mode, AltFunc_mode, Analog_mode};
		enum{AF0 = 0,AF1,AF2,AF3,AF4,AF5,AF6,AF7,AF8,AF9,AF10,AF11,AF12,AF13,AF14,AF15};
		enum{NONE = 0, PULL_UP, PULL_DOWN};
		enum{LOW = 0, MEDIUM, HIGH, VERY_HIGH};
		enum{PUSH_PULL = 0, OPEN_DRAIN};
		
		void write(bool value);
		bool read();
		
		void set_MODE(unsigned short MODE);
		void reset_MODE();
		void set_PUPDR(unsigned short UPDOWN);
		void reset_PUPDR();
		void set_AF(unsigned short AF);
		void reset_AF();
		void set_OSPEEDR(unsigned short SPEED);
		void reset_OSPEEDR();
		void set_OTYPER(unsigned short TYPE);
		void reset_OTYPER();
		
	private:
			void init();
			unsigned short mPIN; 
			unsigned short mMODE; 
			unsigned short mUPDOWN;
			unsigned short mSPEED; 
			unsigned short mTYPE;
			unsigned short mAF;
			GPIO_TypeDef	* mPORT;
			
};


#endif
