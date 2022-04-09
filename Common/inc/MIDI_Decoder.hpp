#ifndef _MIDI_H_
#define _MIDI_H_
//#ifdef __cplusplus
//extern "C" {
//#endif
#include "stm32h755xx.h"                  // Device header
#include "USART.hpp"
#include "cbuff.hpp"
#include "global_defs.hpp"
#define POLY_COUNT 10


	
class MIDI{
	public:

		
		MIDI();
		void put_MIDI_data(char d);
		void decode();
		note_t get_note(unsigned int index);					//pops a note off the note on list
		void pc_keyMap(char d);
		enum NOTE_LIST_INDEXES{NOTE = 0, VEL};	//provide lables for notes list indexes
		enum MIDI_TYPES
		{
			NOTE_ON = 1,
			NOTE_OFF,
			CONTROL_CHANGE,
			PITCHWHEEL
		};
		
	private:

		Circular_Buff <MIDI_cmd_t> m_Buff;
		char m_data[4];
		MIDI_cmd_t m_currentCMD = {0};
		unsigned int m_byte_count = 0;
		note_t mNotes[POLY_COUNT];	//store 10 note numbers and their velocities 
		
		//TODO - figure out how to use DMA to make these accessable to CM7 core
		char control_list[128];
		char pitchwheel;
};
	
	
//#ifdef __cplusplus
//}
//#endif

#endif