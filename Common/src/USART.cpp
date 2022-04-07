#include "..\..\Common\inc\USART.hpp"
#include "main.hpp"
USART::USART(int baudRate, USART_TypeDef * USART_MODULE, GPIO_TypeDef * port, int rx, int tx){
	this->baudRate = baudRate;
	this->USART_MODULE = USART_MODULE;
	this->USART_PORT = port;
	this->RX = rx;
	this->TX = tx;
	//init();	//class doesn't need INIT as USART3 is initalised in main
}

void USART::init(){
	///*
	
	
	__disable_irq();
	unsigned char i1,i2;
	RCC->AHB4ENR|=RCC_AHB4ENR_GPIODEN;		//usart port clock enable
	
	USART_PORT->MODER&=~(										//clear pin function bits
			(3u<<(2*TX))
			|(3u<<(2*RX))
			);
	USART_PORT->MODER|=(										//reset pin function bits (alternate function)
			(2u<<(2*TX))
			|(2u<<(2*RX))
			);
	
	i1=TX/8;											// used to select AFR[reg] for TX_pin ... pins 0-7 AFR[0] pins 8-15 AFR[1]
	i2=RX>>3;											// same as above for RX_pin (uses shift to divide)

		// ALTERNATE FUNCTION SELECT BITS
	USART_PORT->AFR[i1]&=~(0x0f<<(4*(TX-(i1*8))));    //clear pin function
	USART_PORT->AFR[i1]|=(0x07<<(4*(TX-(i1*8))));			// set usart as alternate function for TX_pin
	USART_PORT->AFR[i2]&=~(0x0f<<(4*(RX-(i2*8))));		// clear pin function
	USART_PORT->AFR[i2]|=(0x07<<(4*(RX-(i2*8))));			//set usart as alternate function for RX_pin
	
	RCC->APB1LENR|=RCC_APB1LENR_USART2EN;		//usart clock enable
	unsigned int temp  = ((((SystemCoreClock)/4)<<5)/(16*baudRate));
	unsigned int result = (temp>>1) + (temp & 0x1);
	
	USART_MODULE->BRR		= result;		//set baud rate
	//USART_MODULE->BRR		= 0x674f;
	USART_MODULE->CR2  |= USART_CR2_STOP_1;
	USART_MODULE->CR1|=(										//USART CONFIG
			USART_CR1_TE												//transmit enable
			|USART_CR1_FIFOEN										//enable FIFO buffer
			|USART_CR1_RE												//receive enable
			|USART_CR1_RXNEIE										//Receive Not Empty Interupt Enable
				);
	
	USART_MODULE->CR1 |= USART_CR1_UE;												//usart main enable bit
	
	//NVIC_EnableIRQ(USART3_IRQn);										//enable USART3 Interrupt																								
	__enable_irq();
	//*/
}


char USART::receive(){
	while(!(USART2->ISR & 0x20)){ 			//check if Receive Not Empty flag is set
		__NOP();
	}
	return USART_MODULE->RDR;
}

void USART::send(char d){
	while(!(USART_MODULE->ISR & USART_ISR_TC));	//wait until data register is not in use
	USART_MODULE->TDR=d;
}
void USART::print(char string[]){
	for(int i = 0; string[i] != 0; i++){
		send(string[i]);
	}
}

void USART::format_text(char string[], int txt_colour, int bkg_colour){

}

void USART::move_cursor(int x, int y){

}

void USART::clear(int clear, bool clearType){

}