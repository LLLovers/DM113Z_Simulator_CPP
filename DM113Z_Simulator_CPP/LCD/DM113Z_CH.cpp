#include "DM113Z_CH.h"

const char _name[][3] = {	"A1","A2", 
						"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16",
						"17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31", "32",
};

DM113Z_CH::DM113Z_CH(int pixel_width, int pixel_height, int pixel_gap)
	: _pixel_width(pixel_width), _pixel_height(pixel_height), _pixel_gap(pixel_gap)
{
	_font_handle = 0;
	_font_size = 0;
	_handle = 0;
	_width = 0;
	_height = 0;
	
	_state = new uint8_t[34];
	_pos = new int[34];
}

DM113Z_CH::~DM113Z_CH(void)
{
	DeleteGraph(_handle);

	delete[] _pos;
	delete[] _state;
}

void DM113Z_CH::init(int font_handle, int font_size)
{
	int ret, sx, cx;
	_font_handle = font_handle;
	_font_size = font_size;

	_height = _font_size;
	_width = _pixel_gap + (((_pixel_gap + _pixel_width) * 5) + _pixel_width + _pixel_gap) * (34 / 2);

	ret = MakeScreen(_width, _height);
	assert(ret != -1);
	_handle = ret;

	/*ç¿ïWåvéZ*/
	for (int i = 0; i < 34 / 2; i++) {
		sx = _pixel_gap + (_pixel_width + _pixel_gap) * 6 * i;

		cx = _pixel_gap + (_pixel_width + _pixel_gap) * 1;

		ret = GetDrawStringWidthToHandle(_name[2 * i], strlen(_name[2 * i]), _font_handle);

		_pos[2 * i] = sx + cx - (ret / 2);

		cx = _pixel_gap + (_pixel_width + _pixel_gap) * 4;

		ret = GetDrawStringWidthToHandle(_name[2 * i + 1], strlen(_name[2 * i + 1]), _font_handle);

		_pos[2 * i + 1] = sx + cx - (ret / 2);
	}

	memset(_state, 0x00, 34);
}

void DM113Z_CH::set(int idx, uint8_t state)
{
	_state[idx] = state;
}

int DM113Z_CH::draw(void)
{
	SetDrawScreen(_handle);

	DrawBox(0, 0, _width, _height, _COLOUR_BG, TRUE);

	for (int i = 0; i < 34; i++) {
		DrawStringToHandle(_pos[i], 0, _name[i], (_state[i]) ? _COLOUR_ON : _COLOUR_OFF, _font_handle);
	}
	
	SetDrawScreen(DX_SCREEN_BACK);

	return _handle;
}
