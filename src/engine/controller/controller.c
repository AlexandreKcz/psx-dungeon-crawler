#include "../controller.h"

/// int used to store Psy-Q's pad state of one button
int _sys_pad = 0;
/// same as _sys_pad but to check if a button has been pressed only once
int _sys_pad_trigger = 0;
/// double short buffer to store each pad state as binary state
static unsigned char _pad_buff[2][34];

/**
 * @brief initialize both pad (dualshock compatible in theorys)
 * 
 */
void _pad_initialize(){
	InitPAD(&_pad_buff[0][0], 34, &_pad_buff[1][0], 34);

	StartPAD();

	ChangeClearPAD(1);
}

/**
 * @brief reset button state _sys_pad and _sys_pad_trigger
 * 
 */
void pad_reset(){
	_sys_pad = 0;
	_sys_pad_trigger = 0;
}

/**
 * @brief handle update of the controller state
 * 
 */
void _pad_update(){
	int pad = 0;

	if(_pad_buff[0][0] == 0) {
		pad |= ~((_pad_buff[0][2] << 8) | _pad_buff[0][3]) & 0xFFFF;
	}

	if(_pad_buff[1][0] == 0) { //For controller 2
        pad |= (~((_pad_buff[1][2] << 8) | _pad_buff[1][3]) & 0xFFFF) << 16;
    }

	_sys_pad_trigger = pad & (pad ^ _sys_pad);
    _sys_pad = pad;
}