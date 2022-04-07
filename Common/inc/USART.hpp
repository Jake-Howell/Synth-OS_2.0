#ifndef _USART_H_
#define _USART_H_
#include "stm32h7xx.h"
#include <stdio.h>
#include <stdlib.h>



class USART {
	public:
	USART(int baudRate, USART_TypeDef * USART_MODULE, GPIO_TypeDef * port, int rx, int tx);
	void init();	
	char receive();
	void send(char d);
	void print(char string[]);
	void format_text(char string[], int txt_colour, int bkg_colour);
	void move_cursor(int x, int y);
	void clear(int clear, bool clearType);
	
	enum colours{
	RESET_COLOUR,
	BLACK_TEXT = 30,
	RED_TEXT,
	GREEN_TEXT,
	YELLOW_TEXT,
	BLUE_TEXT,
	MAGENTA_TEXT,
	CYAN_TEXT,
	WHITE_TEXT,
	
	BLACK_BKG = 40,
	RED_BKG,
	GREEN_BKG,
	YELLOW_BKG,
	BLUE_BKG,
	MAGENTA_BKG,
	CYAN_BKG,
	WHITE_BKG
};

enum clearScreenCodes{
	CURSOR_TO_END = 0,
	CURSOR_TO_BEGINING,
	CLEAR_ALL	
};
	private:
		uint8_t TX;
		uint8_t RX;
		unsigned int baudRate;
		USART_TypeDef * USART_MODULE;
		GPIO_TypeDef * USART_PORT;
		char * m_buffer;
		
	
	
};



#endif
