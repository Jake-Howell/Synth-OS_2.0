#include "MIDI_Decoder.hpp"


MIDI::MIDI():cmdBuffer(128){
}

void MIDI::serialToMIDIconverter(MIDI_Serial_Bloak_t d){
    MIDI_cmd_t currentCMD;
    if(d.cs == (d.d0 + d.d1 + d.d2)){	//if check sum matches
        currentCMD.type   = d.d0;		//format data
        currentCMD.param1 = d.d1;
        currentCMD.param2 = d.d2;
        cmdBuffer.put(currentCMD);				//add formatted data to buffer
    }else{
        PrintQueue.call(printf, "Corrupted MIDI CMD\tType: %d\tParam1: %d\tParam2: %d\tcksum: %d\r\n", d.d0, d.d1, d.d2, d.cs);
        cmdBuffer.reset();
    }
}

MIDI_cmd_t MIDI::pc_keyMap(char d){
	unsigned int note_offset = 80;
    MIDI_cmd_t cmd;
	cmd.type = NOTE_ON;
	cmd.param2 = 127;	//by default, set velocity to ma
	switch(d){
		case 'z':
			cmd.param1 = note_offset + 1;
			break;
		case 's':
			cmd.param1 = note_offset + 2;
			break;
		case 'x':
			cmd.param1 = note_offset + 3;
			break;
		case 'd':
			cmd.param1 = note_offset + 4;
			break;
		case 'c':
			cmd.param1 = note_offset + 5;
			break;
		case 'v':
			cmd.param1 = note_offset + 6;
			break;
		case 'g':
			cmd.param1 = note_offset + 7;
			break;
		case 'b':
			cmd.param1 = note_offset + 8;
			break;
		case 'h':
			cmd.param1 = note_offset + 9;
			break;
		case 'n':
			cmd.param1 = note_offset + 10;
			break;
		case 'j':
			cmd.param1 = note_offset + 11;
			break;
		case 'm':
			cmd.param1 = note_offset + 12;
			break;
		case ' ':
			cmd.type = NOTE_OFF;
			cmd.param1 = 127;
			cmd.param2 = 0;
			break;
		default:
			cmd.type = NOTE_OFF;
			cmd.param1 = 127;
			cmd.param2 = 0;
			break;
		
	}
	
	//buffer.put(cmd);//
    return cmd;
}
