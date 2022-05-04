#ifndef _TG_H_
#define _TG_H_

#include <cmath>
#include "stdlib.h"
#include <vector>
#include "global_defs.hpp"
#include "cbuff.hpp"

#define PI 3.141592654
#define OFF 127

using namespace std;


class WaveGen{
	public:
	enum WAVE_TYPE{SIN = 0, TRI, SAW, SQU};	
	
	WaveGen(){
		setSampleRate(96000);	//set sample rate and calculate sample period
		setWaveRes(1024);			//calculate 1024 points in sine wave
		setWaveType(SIN);			//set wave type to SINE by default
        pressNote(55, 120);
        pressNote(53,100);
	}
	
	void setSampleRate(unsigned int rate){
		this->mSampleRate = rate;														//set sample rate
		this->mSamplePeriod_us = 1000000.0/((float)rate);	//recalculate sample period
	}
	
	unsigned int getSampleRate(){
		return this->mSampleRate;
	}
	
	void setWaveRes(unsigned int res){	//set wave resoloution up to 1024 (size of sine table array limits this)
		this->mWaveRes = (res <= 1024)? res : 1024;	//bound res to upper limit of 1023
		float angle;
		for (int x = 0; x < mWaveRes; x++){		//
			angle = ((float)x/mWaveRes)*(2*PI);				//calculate angle increment
			mSineTable[x] = (std::sinf(angle)+1.0f)/2;		//generate wavetable
		}
	}
	
	unsigned int getWaveRes(){
		return this->mWaveRes;
	}


	void setWaveType(WAVE_TYPE type){			//select wave type
		this->mWaveType = type;
	}

	void pressNote(char noteNum, char velocity){	//add note to wave gen, and turn on wave gen
        toneParams note;
		float step = 0.0;
		note.MIDInum = noteNum;
		note.period_us = note_periods_us[noteNum];					//select note period using MIDI note index
        note.Wo = 0.0f;
		step = mSamplePeriod_us/note.period_us;						//Calculate how much each time step (samplePeriod) changes the angle
		note.velocity = ((float)velocity/127);							//calculate velocity multiplier
		note.angularStep = step*(mWaveRes - 1);												//denormalise angular step to produce an index for sineTable
		note.active = true;																//turn the wave generator on
		
        playList.push_back(note);
        //playList.put(note);
	}

    void releaseNote(int MIDInum){
        toneParams note;
        for (int i; i < playList.size(); i++){
            note = playList.at(i);
            if (note.MIDInum == MIDInum){
                playList.erase(playList.begin()+i); //if note has been released by the player, remove it from 
                break; //break from loop if note has been erased
            }
        }
    }

    void readMIDI(MIDI_cmd_t cmd){
        switch(cmd.type){
            case NOTE_ON:
                pressNote(cmd.param1, cmd.param2); //tell synth which note to play, and how looud
                break;
            case NOTE_OFF:
                releaseNote(cmd.param1); //tell synth what not to stop playing
                break;

            case CONTROL_CHANGE:
                break;

            case PITCHWHEEL:
                mPitchwheel = cmd.param1;
                break;


        }

    }
	
	float produceSample(){
        toneParams note; 
		volatile float sample = 0.0f;	//if note is NOT active, output will remain 0.0

        for(int i = 0; i < playList.size(); i++){
            note = playList.at(i);  //read the next note on the play list
            if(note.active){	//if note is on, calculate the next output
                note.Wo += note.angularStep;	//increment angle
                note.Wo = (note.Wo > ((float)(mWaveRes - 1)))?(note.Wo - ((float)(mWaveRes - 1))):(note.Wo); //bound angle to limits
                switch(mWaveType){
                    case SIN:
                        sample += (mSineTable[(int)note.Wo])*note.velocity;	//round angle to nearest intager within wave resoloution and multiply by velocity
                        break;
                    case TRI:
                        sample += ((note.Wo >= (float)mWaveRes/2)?(2*(note.Wo/(mWaveRes-1))):(-2*(note.Wo/(mWaveRes-1))))*note.velocity;   //convert sine to triangle wave
                        break;
                    
                    case SAW:
                        sample += (note.Wo/(mWaveRes-1))*note.velocity;                                                             //convert sine to saw
                        break;
                    
                    case SQU:
                        sample += ((note.Wo >= (float)mWaveRes/2)?1:0)*note.velocity;                                                      //convert sine to square
                        break;
                }
                playList.erase(playList.begin() + i );         //remove old note from playlist
                playList.insert(playList.begin() + i,note);    //insert modified note back into playlist
                //printf("Note: %d\tWo: %5.4f\tout: %5.4f \r\n", note.MIDInum, note.Wo,out);
            }
        }
        //Master Clipping
        sample = (sample/playList.size());
        sample = (sample > 1.0f)?1.0f:sample;
        sample = (sample < 0.0f)?0.0f:sample;
		return sample;   //return output after scaling
	}
	
	private:
        typedef struct{
            bool    active;         //if note is not active, skip any calculations
            float  angularStep;    //calculated angular step per sample
            float  Wo;             //angular velocity
            char    MIDInum;        //MIDI code for note
            float  period_us;      //wavelength
            float  velocity;       //loudness of note (gain)
            
        }toneParams;

        vector<toneParams> playList;        //create a list of waveforms to add to sample
        //Circular_Buff<toneParams> playList;
        char mPitchwheel;
        //Synth Params
        unsigned int mSampleRate = 96000;   //96KHz sample rate
        float mSamplePeriod_us;            //10.41667 us @ 96KHz
        WAVE_TYPE mWaveType;                //Type of wave to be generated (SIN, SAW, TRI, SQU)
		unsigned int mWaveRes = 1024;       //resoloution of wave table
		float mSineTable[1024];		    //sinetable from 0 to 90 degrees

		float note_periods_us[121] = {
					65160,//C0		0
					57720,//C#0		1
					54480,//D0		2
					51420,//D#0		3
					48540,//E0		4
					45820,//FO		5
					43240,//F#0		6
					40820,//G0		7
					38520,//G#0		8
					36360,//A0		9
					34320,//A#0		10
					32400,//B0		11
					30580,//C1		12
					28860,//C#1		13
					27240,//D1		14
					25710,//D#1		15
					24270,//E1		16
					22910,//F1		17
					21620,//F#1		18
					20410,//G1		19
					19260,//G#1		20
					18180,//A1		21
					17160,//A#1		22
					16200,//B1		23
					15290,//C2		24
					14290,//C#2		25
					13620,//D2		26
					12860,//D#2		27
					12130,//E2		28
					11450,//F2		29
					10810,//F#2		30	
					10200,//G2		31
					96310,//G#2		32
					90910,//A2		33
					85810,//A#2		34
					80990,//B2		35
					76450,//C3		36
					72160,//C#3		37
					68110,//D3		38
					64280,//D#3		39
					60680,//E3		40
					57270,//F3		41
					54050,//F#3		42
					51020,//G3		43
					48160,//G#3		44
					45450,//A3		45
					42900,//A#3		46
					40500,//B3		47
					38220,//C4		48
					36080,//C#4		49
					34050,//D4		50
					32140,//D#4		51
					30340,//E4		52
					28630,//F4		53
					27030,//F#4		54
					25510,//G4		55
					24080,//G#4		56
					22730,//A4		57
					21450,//A#4		58
					20250,//B4		59
					19100,//C5		60
					18040,//C#5		61
					17030,//D5		62
					16070,//D#5		63
					15170,//E5		64
					14320,//F5		65
					13510,//F#5		66
					12760,//G5		67
					12040,//G#5		68
					11360,//A5		69
					10730,//A#5		70
					10120,//B5		71
					9556,	//C6		72
					9020,	//C#6		73
					8513,	//D6		74
					8034,	//D#6		75
					7584,	//E6		76
					7159,	//F6		77
					6757,	//F#6		78
					6378,	//G6		79
					6020,	//G#6		80
					5682,	//A6		81
					5363,	//A#6		82
					5062,	//B6		83
					4778,	//C7		84
					4510,	//C#7		85
					4257,	//D7		86
					4018,	//D#7		87
					3792,	//E7		88
					3580,	//F7		89
					3378,	//F#7		90
					3189,	//G7		91
					3010,	//G#7		92
					2841,	//A7		93
					2681,	//A#7		94
					2531,	//B7		95
					2389,	//C8		96
					2255,	//C#8		97
					2129,	//D8		98
					2009,	//D#8		99
					1896,	//E8		100
					1790,	//F8		101
					1689,	//F#8		102
					1595,	//G8		103
					1505,	//G#8		104
					1421,	//A8		105
					1341,	//A#8		106
					1266,	//B8		107
					1195,	//C9		108
					1128,	//C#9		109
					1065,	//D9		110
					1005,	//D#9		111
					948,	//E9		112
					895,	//F9		113
					845,	//F#9		114
					798,	//G9		115
					753,	//G#9		116
					711,	//A9		117
					671,	//A#9		118
					633,	//B9		119
					597,	//C10		120
				};

};

#endif