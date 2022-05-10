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
		void serialToMIDIconverter(MIDI_Serial_Bloak_t d);                     //converts char stream into midi commands
		MIDI_cmd_t pc_keyMap(char d);
		enum NOTE_LIST_INDEXES{NOTE = 0, VEL};	//provide lables for notes list indexes
        Circular_Buff <MIDI_cmd_t> cmdBuffer;
        Circular_Buff <char> serialBuffer;	
};
	
	
//#ifdef __cplusplus
//}
//#endif

#endif