#include "MIDI_Decoder.hpp"


MIDI::MIDI():cmdBuffer(128){
	//m_input.print("Keyboard:\r\n");
	for(int i = 0; i<POLY_COUNT; i++){
		mNotes[i].num = SILENT_NOTE;
		mNotes[i].velocity = 0;
	}
}

void MIDI::controler(){
//    while(!serialBuffer.isEmpty()){ //if serial buffer has data in it, convert and control synth
//    }

}

char MIDI::get_note_num(unsigned int index){
	return mNotes[index].num;
}
char MIDI::get_note_vel(unsigned int index){
	return mNotes[index].velocity;
}

void MIDI::serialToMIDIconverter(char d){
	m_data[m_byte_count++] = d;		//add data to array and increment index
	
	if (m_byte_count >= 4){				//if 4 bytes have been recieved (type, param1, param2, cksum)
		if(((m_data[0]+m_data[1]+m_data[2])&0xFF) == (m_data[3]&0xFF)){	//if check sum matches
			m_currentCMD.type = m_data[0];		//format data
			m_currentCMD.param1 = m_data[1];
			m_currentCMD.param2 = m_data[2];
			cmdBuffer.put(m_currentCMD);				//add formatted data to buffer
		}
		
		m_currentCMD = {0};	//reset data
		m_byte_count = 0;		//reset byte count
	}
	
}

MIDI_cmd_t MIDI::pc_keyMap(char d){
	MIDI_cmd_t cmd;
	cmd.type = NOTE_ON;
	cmd.param2 = 127;	//by default, set velocity to max
	switch(d){
		case 'z':
			cmd.param1 = 48;
			break;
		case 's':
			cmd.param1 = 49;
			break;
		case 'x':
			cmd.param1 = 50;
			break;
		case 'd':
			cmd.param1 = 51;
			break;
		case 'c':
			cmd.param1 = 52;
			break;
		case 'v':
			cmd.param1 = 53;
			break;
		case 'g':
			cmd.param1 = 54;
			break;
		case 'b':
			cmd.param1 = 55;
			break;
		case 'h':
			cmd.param1 = 56;
			break;
		case 'n':
			cmd.param1 = 57;
			break;
		case 'j':
			cmd.param1 = 58;
			break;
		case 'm':
			cmd.param1 = 59;
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
