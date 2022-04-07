
#ifndef _DATA_BUS_H__
#define _DATA_BUS_H__

#include "stm32h7xx.h"                  // Device header

#include "../inc/IO.hpp"

using namespace std;



class DataBus{
	
	public:
		DataBus(PinName *DataPinList, unsigned int busWidth, unsigned short mode);
		~DataBus();
		void init();
		void send_data(unsigned int data);
		int read_data();
		void setBusOut();	//set bus as output
		void setBusIn();		//set bus as input
		//void set_Rx_Ready(PinName Rx_Ready_pin);
		//void reset_Rx_Ready();
		//void set_Tx_Ready(PinName Tx_Ready_pin);
		//void reset_Tx_Ready();
	
	private:
		
		unsigned short mMode;
		unsigned short mBusWidth;
		IO* mRx_Ready;						//if needed, initalise
		IO* mTx_Ready;						//if needed, initalise
		PinName* mDataPinList;
		IO* mIO_List[16];
};

#endif
