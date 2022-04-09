#ifndef _GLOBAL_H_
#define _GLOBAL_H_
typedef struct{
	char type;
	char param1;
	char param2;
} MIDI_cmd_t;

typedef struct{
	char num;
	char velocity;
}note_t;

#endif