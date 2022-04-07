#ifdef __cplusplus
extern "C" {
#endif
	
#include "../inc/Data_Bus.hpp"
	

DataBus::DataBus(PinName *DataPinList, unsigned int busWidth, unsigned short mode):
mDataPinList(DataPinList),mBusWidth(busWidth), mMode(mode){

	//reset_Rx_Ready();
	//reset_Tx_Ready();
	init();
}

DataBus::~DataBus(){

}

void DataBus::init(){
	PinName DataPin;
	
	for (int i = 0; i < mBusWidth; i++){
		DataPin = mDataPinList[i];
		IO* DL = new IO(DataPin.port, DataPin.pin, IO::Output_mode);
		mIO_List[i] = DL;	//add IO object to IO list. This will be written and read from later
	}
}

void DataBus::send_data(unsigned int data){
	bool bit = 0;
	for (int i = 0; i < mBusWidth; i++){
		bit = (data>>i)&0xFFFE;	//only select LSB after bit shift
		mIO_List[i]->write(bit);
	}
}

int DataBus::read_data(){
	int data = 0;
	//TODO make mRx_Ready an IO object
	/*
	if(mRx_Ready.enabled){mRx_Ready = 0;}
	if(mTx_Ready.enabled){
		while(mTx_Ready = 0);	//block if Tx is not ready
	}
	
	for (int i = 0; i < mBusWidth; i++){
		data |= (mIO_List[i].read())<<i;  
	}*/
	return data;
}

void DataBus::setBusOut(){
	for (int i = 0; i < mBusWidth; i++){
		mIO_List[i]->reset_MODE();
		mIO_List[i]->set_MODE(IO::Output_mode);
	}
}
	
void DataBus::setBusIn(){
	for (int i = 0; i < mBusWidth; i++){
		mIO_List[i]->reset_MODE();
		mIO_List[i]->set_MODE(IO::Input_mode);
	}
}
	



#ifdef __cplusplus
}
#endif
