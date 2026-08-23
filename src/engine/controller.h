#pragma once

/// Most of the code here comes from MBDesign's work : <https://mbdesigns.itch.io/ps1-homebrew-source-code-rendering-3d-graphics-with-psyq>

#include <libetc.h>
#include <libapi.h>

extern int	_sys_pad;
extern int  _sys_pad_trigger;

/// check if button is held
#define	pad_check(_p_)          (_sys_pad & (_p_))
/// check if button is pressed
#define	pad_check_pressed(_p_)	(_sys_pad_trigger & (_p_))

#define pad1Up			_PAD(0, PADLup)
#define pad1Down		_PAD(0, PADLdown)
#define pad1Left		_PAD(0, PADLleft)
#define pad1Right		_PAD(0, PADLright)
#define pad1Triangle	_PAD(0, PADRup)
#define pad1Cross		_PAD(0, PADRdown)
#define pad1Square		_PAD(0, PADRleft)
#define pad1Circle		_PAD(0, PADRright)
#define pad1L1			_PAD(0, PADL1)
#define pad1L2			_PAD(0, PADL2)
#define pad1R1			_PAD(0, PADR1)
#define pad1R2			_PAD(0, PADR2)
#define pad1Start		_PAD(0, PADstart)
#define pad1Select		_PAD(0, PADselect)
#define pad2Up			_PAD(1, PADLup)
#define pad2Down		_PAD(1, PADLdown)
#define pad2Left		_PAD(1, PADLleft)
#define pad2Right		_PAD(1, PADLright)
#define pad2Triangle	_PAD(1, PADRup)
#define pad2Cross		_PAD(1, PADRdown)
#define pad2Square		_PAD(1, PADRleft)
#define pad2Circle		_PAD(1, PADRright)
#define pad2L1			_PAD(1, PADL1)
#define pad2L2			_PAD(1, PADL2)
#define pad2R1			_PAD(1, PADR1)
#define pad2R2			_PAD(1, PADR2)
#define pad2Start		_PAD(1, PADstart)
#define pad2Select		_PAD(1, PADselect)

void _pad_initialize();
void pad_reset();
void _pad_update();