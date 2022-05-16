#ifndef _RE_H_
#define _RE_H_

#include "mbed.h"
#include "global_defs.hpp"
#include <cstdint>

extern EventQueue PrintQueue;

class RotaryEncoder{
    public:
        RotaryEncoder(RE_Pins RE):bus(RE.D0,RE.D1, RE.D2, RE.D3,RE.D4,RE.D5, RE.D6, RE.D7){
            this->pos = 0;
        }

        void update_pos(){
            GC_latest = bus.read();
            this->pos = convert_GreyCode(GC_latest);
        }
        uint8_t getPos(){
            return this->pos;
        }
        float getPosf(){
            return ((float)this->pos)/127;
        }
        uint8_t getGC(){
            return this->GC_latest;
        }
    private:
        BusIn bus;
        uint8_t pos; //stores clast read position of the rotary encoder (0 - 127)
        uint8_t GC_latest;

        uint8_t convert_GreyCode(uint8_t GC){
                uint8_t p = 0;
            switch(GC){
                case 127:
                    p = 0;
                    break;
                case 63:
                    p = 1;
                    break;
                case 62:
                    p = 2;
                    break;
                case 58:
                    p = 3;
                    break;
                case 56:
                    p = 4;
                    break;
                case 184:
                    p = 5;
                    break;
                case 152:
                    p = 6;
                    break;
                case 24:
                    p = 7;
                    break;
                case 8:
                    p = 8;
                    break;
                case 72:
                    p = 9;
                    break;
                case 73:
                    p = 10;
                    break;
                case 77:
                    p = 11;
                    break;
                case 79:
                    p = 12;
                    break;
                case 15:
                    p = 13;
                    break;
                case 47:
                    p = 14;
                    break;
                case 175:
                    p = 15;
                    break;
                case 191:
                    p = 16;
                    break;
                case 159:
                    p = 17;
                    break;
                case 31:
                    p = 18;
                    break;
                case 29:
                    p = 19;
                    break;
                case 28:
                    p = 20;
                    break;
                case 92:
                    p = 21;
                    break;
                case 76:
                    p = 22;
                    break;
                case 12:
                    p = 23;
                    break;
                case 4:
                    p = 24;
                    break;
                case 36:
                    p = 25;
                    break;
                case 164:
                    p = 26;
                    break;
                case 166:
                    p = 27;
                    break;
                case 167:
                    p = 28;
                    break;
                case 135:
                    p = 29;
                    break;
                case 151:
                    p = 30;
                    break;
                case 215:
                    p = 31;
                    break;
                case 223:
                    p = 32;
                    break;
                case 207:
                    p = 33;
                    break;
                case 143:
                    p = 34;
                    break;
                case 142:
                    p = 35;
                    break;
                case 14:
                    p = 36;
                    break;
                case 46:
                    p = 37;
                    break;
                case  38:
                    p = 38;
                    break;
                case 6:
                    p = 39;
                    break;
                case 2:
                    p = 40;
                    break;
                case 18:
                    p = 41;
                    break;
                case 82:
                    p = 42;
                    break;
                case 83:
                    p = 43;
                    break;
                case 211:
                    p = 44;
                    break;
                case 195:
                    p = 45;
                    break;
                case 203:
                    p = 46;
                    break;
                case 235:
                    p = 47;
                    break;
                case 239:
                    p = 48;
                    break;
                case 231:
                    p = 49;
                    break;
                case 199:
                    p = 50;
                    break;
                case 71:
                    p = 51;
                    break;
                case 7:
                    p = 52;
                    break;
                case 23:
                    p = 53;
                    break;
                case 19:
                    p = 54;
                    break;
                case 3:
                    p = 55;
                    break;
                case 1:
                    p = 56;
                    break;
                case 9:
                    p = 57;
                    break;
                case 41:
                    p = 58;
                    break;
                case 169:
                    p = 59;
                    break;
                case 233:
                    p = 60;
                    break;
                case 225:
                    p = 61;
                    break;
                case 229:
                    p = 62;
                    break;
                case 245:
                    p = 63;
                    break;
                case 247:
                    p = 64;
                    break;
                case 243:
                    p = 65;
                    break;
                case 227:
                    p = 66;
                    break;
                case 163:
                    p = 67;
                    break;
                case 131:
                    p = 68;
                    break;
                case 139:
                    p = 69;
                    break;
                case 137:
                    p = 70;
                    break;
                case 129:
                    p = 71;
                    break;
                case 128:
                    p = 72;
                    break;
                case 132:
                    p = 73;
                    break;
                case 148:
                    p = 74;
                    break;
                case 212:
                    p = 75;
                    break;
                case 244:
                    p = 76;
                    break;
                case 240:
                    p = 77;
                    break;
                case 242:
                    p = 78;
                    break;
                case 250:
                    p = 79;
                    break;
                case 251:
                    p = 80;
                    break;
                case 249:
                    p = 81;
                    break;
                case 241:
                    p = 82;
                    break;
                case 209:
                    p = 83;  
                    break;
                case 193:
                    p = 84; 
                    break;
                case 197:
                    p = 85; 
                    break;
                case 196:
                    p = 86;
                    break;
                case 192:
                    p = 87;
                    break;
                case 64:
                    p = 88;
                    break;
                case 66:
                    p = 89;
                    break;
                case 74:
                    p = 90;
                    break;
                case 106:
                    p = 91;
                    break;
                case 122:
                    p = 92;
                    break;
                case 120:
                    p = 93;
                    break;
                case 121:
                    p = 94;
                    break;
                case 125:
                    p = 95;
                    break;
                case 253:
                    p = 96;
                    break;
                case 252:
                    p = 97;
                    break;
                case 248:
                    p = 98;
                    break;
                case 232:
                    p = 99;
                    break;
                case 224:
                    p = 100;
                    break;
                case 226:
                    p = 101;
                    break;
                case 98:
                    p = 102;
                    break;
                case 96:
                    p = 103;
                    break;
                case 32:
                    p = 104;
                    break;
                case 33:
                    p = 105;
                case 37:
                    p = 106;
                    break;
                case 53:
                    p = 107;
                    break;
                case 61:
                    p = 108;
                    break;
                case 60:
                    p = 109;
                    break;
                case 188:
                    p = 110;
                    break;
                case 190:
                    p = 111;
                    break;
                case 254:
                    p = 112;
                    break;
                case 126:
                    p = 113;
                    break;
                case 124:
                    p = 114;
                    break;
                case 116:
                    p = 115;
                    break;
                case 112:
                    p = 116;
                    break;
                case 113:
                    p = 117;
                    break;
                case 49:
                    p = 118;
                    break;
                case 48:
                    p = 119;
                    break;
                case 16:
                    p = 120;
                    break;
                case 144:
                    p = 121;
                    break;
                case 146:
                    p = 122;
                    break;
                case 154:
                    p = 123;
                    break;
                case 158:
                    p = 124;
                    break;
                case 30:
                    p = 125;
                    break;
                case 94:
                    p = 126;
                    break;
                case 95:
                    p = 127;
                    break;
                default:
                    p = 0;
                    PrintQueue.call(printf, "Corrupted Gray Code: 0x%2X\t", GC);
                    break;
            }
            return p;
        }
        //*/
}; 
#endif
