#include "../inc/MIDI_Decoder.hpp"


MIDI::MIDI():m_Buff(128){
	//m_input.print("Keyboard:\r\n");
}

note_t MIDI::get_note(unsigned int index){
	return mNotes[index];
}

void MIDI::put_MIDI_data(char d){
	m_data[m_byte_count++] = d;		//add data to array and increment index
	
	if (m_byte_count >= 4){				//if 4 bytes have been recieved (type, param1, param2, cksum)
		if(((m_data[0]+m_data[1]+m_data[2])&0xFF) == (m_data[3]&0xFF)){	//if check sum matches
			m_currentCMD.type = m_data[0];		//format data
			m_currentCMD.param1 = m_data[1];
			m_currentCMD.param2 = m_data[2];
			m_Buff.put(m_currentCMD);				//add formatted data to buffer
		}
		
		m_currentCMD = {0};	//reset data
		m_byte_count = 0;		//reset byte count
	}
	
}

void MIDI::decode(){
	MIDI_cmd_t MIDI_data = m_Buff.get();
	char noteNum = MIDI_data.param1;
	char noteVel = MIDI_data.param2;
	char mNotes_empty_index;
	bool mNotesListFull = true;
	switch(MIDI_data.type){
		
		case NOTE_ON:	//when NOTE_ON, param1 = note_number, param2 = velocity
			for (int i = 0; i < POLY_COUNT; i++){	//loop through notes list
				if (mNotes[i].num == noteNum){			//if note is already in mNotes list, break loop and do nothing
					break;
				}else if (mNotes[i].num == 127){	//if off note found, record it's index
					mNotes_empty_index = i;
					mNotesListFull = false;
				}
			}
			if(mNotesListFull == false){						//if an empty slot was found, add noteNum and noteVel to list
					mNotes[mNotes_empty_index].num = noteNum;
					mNotes[mNotes_empty_index].velocity = noteVel;
			}
			break;
		
		case NOTE_OFF: //when NOTE_OFF, param1 = note_number, param2 = velovity (should be 0)

			for (int i = 0; i < POLY_COUNT; i++){	//when note turned off, loop through notes list, and zero note if its there
				if (mNotes[i].num == noteNum){
					mNotes[i].num = 127;	//127 is NOTE_OFF
					mNotes[i].velocity = 0;
				}else if (noteNum == 127){	//if note is off note, turn all notes off
					mNotes[i].num = 127; 
					mNotes[i].velocity = 0;
				}
			}
			break;
		case CONTROL_CHANGE:
			break;
		
		case PITCHWHEEL:
			pitchwheel = MIDI_data.param1;
			break;
		
		default:
			break;
	
	};
}

void MIDI::pc_keyMap(char d){
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
	
	m_Buff.put(cmd);
}
