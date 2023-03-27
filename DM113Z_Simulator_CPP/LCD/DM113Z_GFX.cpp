#include "DM113Z_GFX.h"

DM113Z_GFX::DM113Z_GFX(int gfx_width, int gfx_height, int pixel_width, int pixel_height, int pixel_gap) : _gfx_width(gfx_width), _gfx_height(gfx_height), _pixel_width(pixel_width), _pixel_height(pixel_height), _pixel_gap(pixel_gap)
{
	_gfx = new uint8_t[(_gfx_width / 8) * _gfx_height];
	memset(_gfx, 0x00, (_gfx_width / 8) * _gfx_height);
	_gfx_handle = 0;

	_width = _pixel_gap + (_pixel_width + _pixel_gap) * _gfx_width;
	_height = _pixel_gap + (_pixel_height + _pixel_gap) * _gfx_height;
	_pixel_count = _gfx_width * _gfx_height;

}

DM113Z_GFX::~DM113Z_GFX(void)
{
	delete[] _gfx;
}

void DM113Z_GFX::init(void)
{
	int ret = MakeScreen(_width, _height);

	memset(_gfx, 0x00, (_gfx_width / 8) * _gfx_height);
	assert(ret != -1);
	_gfx_handle = ret;
}

void DM113Z_GFX::clear(void)
{
	memset(_gfx, 0x00, (_gfx_width / 8) * _gfx_height);
}

void DM113Z_GFX::set(uint8_t* gfx)
{
	memcpy(_gfx, gfx, _pixel_count / 8);

}

int DM113Z_GFX::draw(void)
{
	int x, y;

	SetDrawScreen(_gfx_handle);

	DrawBox(0, 0, _width, _height, _COLOUR_BG, TRUE);

	for (y = 0; y < _gfx_height; y++) {
		for (x = 0; x < _gfx_width; x++) {
			int arr_pos = (_gfx_width / 8) * y + (x / 8);
			int bit_pos = 7 - (x % 8);
			bool cond = (uint8_t)_gfx[arr_pos] & (uint8_t)(1 << bit_pos);

			int sx = _pixel_gap + (_pixel_width + _pixel_gap) * x;
			int sy = _pixel_gap + (_pixel_height + _pixel_gap) * y;

			DrawBox(sx, sy, sx + _pixel_width, sy + _pixel_height, (cond) ? _COLOUR_ON : _COLOUR_OFF, TRUE);
		}
	}

	SetDrawScreen(DX_SCREEN_BACK);

	return _gfx_handle;
}
