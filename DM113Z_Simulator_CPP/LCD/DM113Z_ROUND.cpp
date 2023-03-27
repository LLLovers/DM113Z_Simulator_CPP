#include "DM113Z_ROUND.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

DM113Z_ROUND::DM113Z_ROUND(int width, int height, char* str) : _width(width), _height(height)
{
	_font_handle = 0;
	_font_size = 0;
	_handle = 0;
	_state = 0;
	_str_width = 0;
	strcpy_s(_str, str);
}

DM113Z_ROUND::~DM113Z_ROUND(void)
{
	DeleteGraph(_handle);
}

void DM113Z_ROUND::init(int font_handle, int font_size)
{
	int ret;
	_font_handle = font_handle;
	_font_size = font_size;

	assert(_font_handle != -1);

	/*ƒXƒNƒŠ[ƒ“ì¬*/
	ret = MakeScreen(_width, _height, TRUE);
	assert(ret != -1);

	_handle = ret;

	/*•¶Žš—ñ‚Ì•‚ðŽæ“¾‚·‚é*/
	ret = GetDrawStringWidthToHandle(_str, strlen(_str), _font_handle);
	assert(ret <= _width);

	assert(ret != -1);

	_state = 0;
	_str_width = ret;
}

void DM113Z_ROUND::set(int state)
{
	_state = state;
}

int DM113Z_ROUND::draw(void)
{
	SetDrawScreen(_handle);

	DrawBox(0, 0, _width, _height, _COLOUR_BG, TRUE);

	DrawRoundRect(0, 0, _width, _height, (int)(0.1f * MIN(_width, _height)), (int)(0.1f * MIN(_width, _height)), (_state) ? _COLOUR_ON : _COLOUR_OFF, TRUE);

	int sx = (_width - _str_width) / 2;
	int sy = (_height - _font_size) / 2;

	DrawStringToHandle(sx, sy, _str, _COLOUR_BG, _font_handle);

	SetDrawScreen(DX_SCREEN_BACK);

	return _handle;
}
