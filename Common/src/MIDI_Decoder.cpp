#include "../inc/MIDI_Decoder.hpp"


MIDI::MIDI(USART_TypeDef * USART_MODULE, GPIO_TypeDef * port, int Rx, int Tx, int baudrate):m_Buff(512, "MIDI Buffer"), m_input(baudrate, USART_MODULE, port, Rx, Tx){

}

void MIDI::get_MIDI_data(){
	char cksum = 0;
	MIDI_cmd_t d;
	d.type = m_input.receive();
	d.param1 = m_input.receive();
	d.param2 = m_input.receive();
	
	cksum = m_input.receive();
	
	if ((d.type + d.param1 + d.param2) != cksum){
		m_input.send(1);	//send error code 1 data does not match check sum
	}else{
		m_Buff.put(d);			//get 3 chars wrap into struct and put on buffer
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