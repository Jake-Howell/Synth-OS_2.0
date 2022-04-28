#ifndef _MIDI_H_
#define _MIDI_H_
//#ifdef __cplusplus
//extern "C" {
//#endif

#include "cbuff.hpp"
#include "global_defs.hpp"
#define POLY_COUNT 10


	
class MIDI{
	public:

		
		MIDI();
		void serialToMIDIconverter(char d);                     //converts char stream into midi commands
        void controler();
		char get_note_num(unsigned int index);					//pops a note off the note on list
		char get_note_vel(unsigned int index);					//pops a note off the note on list
		MIDI_cmd_t pc_keyMap(char d);
		enum NOTE_LIST_INDEXES{NOTE = 0, VEL};	//provide lables for notes list indexes
        Circular_Buff <MIDI_cmd_t> cmdBuffer;
        Circular_Buff <char> serialBuffer;
		
	private:

		
		char m_data[4];
		MIDI_cmd_t m_currentCMD = {0};
		unsigned int m_byte_count = 0;
		note_t mNotes[POLY_COUNT] = {0};	//store 10 note numbers and their velocities 
		
		//TODO - figure out how to use DMA to make these accessable to CM7 core
		char control_list[128];
		char pitchwheel;

        //key mapping for PC Playing
        //char keys[50][2] = {'z','s','x','d','c','f','v','g','b','h','n','j','m'};
};
	
	
//#ifdef __cplusplus
//}
//#endif

#endif