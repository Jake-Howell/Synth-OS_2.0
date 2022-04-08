#include "../inc/MIDI_Decoder.hpp"



MIDI::MIDI(USART_TypeDef * USART_MODULE, GPIO_TypeDef * port, int Rx, int Tx, int baudrate):m_Buff(512, "MIDI Buffer"), m_input(baudrate, USART_MODULE, port, Rx, Tx){

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
	
	switch(MIDI_data.type){
		
		case NOTE_ON:	//when NOTE_ON, param1 = note_number, param2 = velocity
			note_on_list[MIDI_data.param1] = MIDI_data.param2; //add note velocity to note_on_list
			break;
		
		case NOTE_OFF: //when NOTE_OFF, param1 = note_number, param2 = velovity (should be 0)
			note_on_list[MIDI_data.param1] = 0; //set velocity to 0 when note off detected
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