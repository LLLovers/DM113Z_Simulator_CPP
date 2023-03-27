#pragma once

#include<stdint.h>
#include"DXLib.h"

#include "DM113Z_ARROW.h"
#include "DM113Z_BANK_PGM.h"
#include "DM113Z_BAR_ARC.h"
#include "DM113Z_BAR_RECT.h"
#include "DM113Z_CH.h"
#include "DM113Z_GFX.h"
#include "DM113Z_KEY.h"
#include "DM113Z_MIDI.h"
#include "DM113Z_MODE.h"
#include "DM113Z_PAN.h"
#include "DM113Z_PART.h"
#include "DM113Z_ROUND.h"
#include "DM113Z_TXT.h"
#include "font.h"

class DM113Z
{
public:

	enum seg_e {
		SEG_MIC = 10,
		SEG_LINE = 11,
		SEG_CH_LEFT = 12,
		SEG_CH_RIGHT = 13,
		SEG_BANK_PGM_LEFT = 14,
		SEG_BANK_PGM_RIGHT = 15,
		SEG_ARROW_MIDI = 0,
		SEG_ARROW_BANK = 1,
		SEG_ARROW_PGM = 2,
		SEG_ARROW_VOL = 3,
		SEG_ARROW_EXP = 4,
		SEG_ARROW_PAN = 5,
		SEG_ARROW_REV = 6,
		SEG_ARROW_CHO = 7,
		SEG_ARROW_VAR = 8,
		SEG_ARROW_KEY = 9
	};

	enum param_e {
		PARAM_VOL,
		PARAM_EXP,
		PARAM_PAN,
		PARAM_REV,
		PARAM_CHO,
		PARAM_VAR,
		PARAM_KEY,
		PARAM_MODE
	};

	DM113Z();
	~DM113Z();

	void init(void);

	void clear(void);

	void set_char_txt(int, int, uint8_t);

	void set_char_part(int, uint8_t);

	void set_char_midi(int, uint8_t);

	void set_gfx(uint8_t*);

	void set_segment(seg_e, uint8_t);

	void set_param(param_e, uint8_t*);

	void set_cgram(uint8_t, uint8_t*);

	int callback(void);

	volatile int isReady;
private:
	void _set_resolution(void);

	DM113Z_TXT* _text;
	DM113Z_ROUND* _line, * _mic;
	DM113Z_CH* _ch;
	DM113Z_BANK_PGM* _bank_pgm;
	DM113Z_PART* _part;
	DM113Z_MIDI* _midi;
	DM113Z_GFX* _gfx;
	DM113Z_BAR_RECT* _vol, * _exp;
	DM113Z_PAN* _pan;
	DM113Z_BAR_ARC* _rev, * _cho, * _var;
	DM113Z_KEY* _key;
	DM113Z_MODE* _mode;
	DM113Z_ARROW* _aMIDI, * _aBANK, * _aPGM, * _aVOL, * _aEXP, * _aPAN, * _aREV, * _aCHO, * _aVAR, * _aKEY;

	int _font_handle;
	int _handle;

	uint8_t* _cgram;

	int _width, _height;
};

