#include "DM113Z_BAR_RECT.h"

#define _SET_SEG(cond,sy)	DrawBox(_pixel_gap, sy,_pixel_gap + _pixel_width, sy + _pixel_height, (cond) ? _COLOUR_ON : _COLOUR_OFF, TRUE);


DM113Z_BAR_RECT::DM113Z_BAR_RECT(int pixel_width, int pixel_height, int pixel_gap) : _pixel_width(pixel_width), _pixel_height(pixel_height), _pixel_gap(pixel_gap)
{
	assert(_count <= 8);

	_handle = 0;
	_height = _pixel_gap * 2 + _pixel_height * (2 * _count - 1);
	_width = _pixel_gap * 2 + _pixel_width;
	_seg.d8 = 0x00;
}

void DM113Z_BAR_RECT::init(void)
{
	int ret = MakeScreen(_width, _height);

	_seg.d8 = 0x00;

	assert(ret != -1);
	_handle = ret;
}

void DM113Z_BAR_RECT::set(union segment_t* seg)
{
	memcpy(&_seg, seg, sizeof(_seg));
}

int DM113Z_BAR_RECT::draw(void)
{
	int y = 0;

	SetDrawScreen(_handle);

	DrawBox(0, 0, _width, _height, _COLOUR_BG, TRUE);

	for (int y = 0; y < 8; y++) {
		_SET_SEG((_seg.d8 & (1 << (7 - y))), _pixel_gap + (_pixel_height * 2) * y);
	}

	SetDrawScreen(DX_SCREEN_BACK);

	return _handle;
}
