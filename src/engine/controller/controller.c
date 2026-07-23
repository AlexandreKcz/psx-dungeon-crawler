#include "../controller.h"

int SysPad = 0;
int SysPadT = 0;

static unsigned char pad_buff[2][34];

void pad_initialize(){
	InitPAD(&pad_buff[0][0], 34, &pad_buff[1][0], 34);

	StartPAD();

	ChangeClearPAD(1);
}

void pad_reset(){
	SysPad = 0;
	SysPadT = 0;
}

void pad_update(){
	int pad = 0;

	if(pad_buff[0][0] == 0) {
		pad |= ~((pad_buff[0][2] << 8) | pad_buff[0][3]) & 0xFFFF;
	}

	if(pad_buff[1][0] == 0) { //For controller 2
        pad |= (~((pad_buff[1][2] << 8) | pad_buff[1][3]) & 0xFFFF) << 16;
    }

	SysPadT = pad & (pad ^ SysPad);
    SysPad = pad;
}