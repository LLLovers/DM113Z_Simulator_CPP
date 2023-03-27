#include "DM113Z_BANK_PGM.h"

DM113Z_BANK_PGM::DM113Z_BANK_PGM(int pixel_width, int pixel_height, int pixel_gap)
	: _pixel_width(pixel_width), _pixel_height(pixel_height), _pixel_gap(pixel_gap)
{
	_font_handle = 0;
	_font_size = 0;
	_handle = 0;
	_width = 0;
	_height = 0;

	_state = new uint8_t[2];
	_pos = new int[4];
}

DM113Z_BANK_PGM::~DM113Z_BANK_PGM(void)
{
	delete[] _state;
	delete[] _pos;
}

void DM113Z_BANK_PGM::init(int font_handle, int font_size)
{
	int ret, sx, cx;
	_font_handle = font_handle;
	_font_size = font_size;

	_height = _font_size * 2;
	_width = _pixel_gap + (((_pixel_gap + _pixel_width) * 5) + _pixel_width + _pixel_gap) * (34 / 2);

	ret = MakeScreen(_width, _height);
	assert(ret != -1);
	_handle = ret;

	/*ç∂BANK*/
	sx = _pixel_gap + (_pixel_width + _pixel_gap) * 6 * 2;
	cx = _pixel_gap + (_pixel_width + _pixel_gap) * 3 - (_pixel_width / 2);
	ret = GetDrawStringWidthToHandle("BANK", strlen("BANK"), _font_handle);
	_pos[0] = sx + cx - (ret / 2);

	/*ç∂PGM#*/
	sx = _pixel_gap + (_pixel_width + _pixel_gap) * 6 * 6;
	cx = _pixel_gap + (_pixel_width + _pixel_gap) * 3 - (_pixel_width / 2);
	ret = GetDrawStringWidthToHandle("PGM#", strlen("PGM#"), _font_handle);
	_pos[1] = sx + cx - (ret / 2);

	/*âEBANK*/
	sx = _pixel_gap + (_pixel_width + _pixel_gap) * 6 * 10;
	cx = _pixel_gap + (_pixel_width + _pixel_gap) * 3 - (_pixel_width / 2);
	ret = GetDrawStringWidthToHandle("BANK", strlen("BANK"), _font_handle);
	_pos[2] = sx + cx - (ret / 2);

	/*âEPGM#*/
	sx = _pixel_gap + (_pixel_width + _pixel_gap) * 6 * 14;
	cx = _pixel_gap + (_pixel_width + _pixel_gap) * 3 - (_pixel_width / 2);
	ret = GetDrawStringWidthToHandle("PGM#", strlen("PGM#"), _font_handle);
	_pos[3] = sx + cx - (ret / 2);

	memset(_state, 0x00, 2);
}

void DM113Z_BANK_PGM::set(int idx, uint8_t state)
{
	_state[idx] = state;
}

int DM113Z_BANK_PGM::draw(void)
{
	SetDrawScreen(_handle);

	DrawBox(0, 0, _width, _height, _COLOUR_BG, TRUE);

	DrawStringToHandle(_pos[0], 0, "BANK", (_state[0]) ? _COLOUR_ON : _COLOUR_OFF, _font_handle);
	DrawStringToHandle(_pos[1], 0, "PGM#", (_state[0]) ? _COLOUR_ON : _COLOUR_OFF, _font_handle);
	DrawStringToHandle(_pos[2], 0, "BANK", (_state[1]) ? _COLOUR_ON : _COLOUR_OFF, _font_handle);
	DrawStringToHandle(_pos[3], 0, "PGM#", (_state[1]) ? _COLOUR_ON : _COLOUR_OFF, _font_handle);

	SetDrawScreen(DX_SCREEN_BACK);

	return _handle;
}
