#ifndef _MIDI_H_
#define _MIDI_H_
//#ifdef __cplusplus
//extern "C" {
//#endif
#include "stm32h755xx.h"                  // Device header
#include "USART.hpp"
#include "cbuff.hpp"

	
class MIDI{
	public:
		typedef struct{
			char type;
			char param1;
			char param2;
		} MIDI_cmd_t;
		
		typedef struct{
			char num;
			char velocity;
		}note_t;
		
		MIDI(USART_TypeDef * USART_MODULE, GPIO_TypeDef * port, int Rx, int Tx, int baudrate);
		void get_MIDI_data();
		void decode();
		enum MIDI_TYPES
		{
			NOTE_ON = 1,
			NOTE_OFF,
			CONTROL_CHANGE,
			PITCHWHEEL
		};
	
	private:
		USART	m_input;
		Circular_Buff <MIDI_cmd_t> m_Buff;
		char note_on_list[128];
		char control_list[128];
		char pitchwheel;
};
	
	
//#ifdef __cplusplus
//}
//#endif

#endif