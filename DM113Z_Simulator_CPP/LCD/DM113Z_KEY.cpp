#include "DM113Z_KEY.h"

DM113Z_KEY::DM113Z_KEY(int gap) : _gap(gap)
{
	_width = 0;
	_height = 0;

	_seg_handle = 0;
	_handle = 0;

	_seg = new DM113Z_SEG * [2];
	_sign = new DM113Z_SIGN;

	for (int i = 0; i < 2; i++) {
		_seg[i] = new DM113Z_SEG();

	}
}

DM113Z_KEY::~DM113Z_KEY(void)
{
	delete[] _seg;
	delete _sign;

	DeleteGraph(_seg_handle);
	DeleteGraph(_handle);
}

void DM113Z_KEY::init(void)
{
	int ret;
	_sign->init();
	_seg[0]->init();
	_seg[1]->init();

	_width = _seg[0]->get_width() * 2 + _sign->get_width() + _gap * 2;
	_height = _seg[0]->get_height();

	ret = MakeScreen(_width, _height, TRUE);
	assert(ret != -1);

	_handle = ret;
}

void DM113Z_KEY::set(int idx, uint8_t d8)
{
	DM113Z_SEG::segment_t s;
	switch (idx) {
	case 0:
		_sign->set(d8);
		break;
	case 1:
		s.d8 = d8;
		_seg[0]->set(&s);
		break;
	case 2:
		s.d8 = d8;
		_seg[1]->set(&s);
		break;
	}
}

int DM113Z_KEY::draw(void)
{
	int x;

	int y = (_seg[0]->get_height() - _sign->get_height()) / 2;

	_seg_handle = _sign->draw();

	SetDrawScreen(_handle);

	DrawBox(0, 0, _width, _height, _COLOUR_BG, TRUE);

	DrawGraph(0, y, _seg_handle, TRUE);

	for (x = 0; x < 2; x++) {
		int sx = _sign->get_width() + (_seg[0]->get_width() + _gap) * x;
		_seg_handle = _seg[x]->draw();
		SetDrawScreen(_handle);
		DrawGraph(sx, 0, _seg_handle, TRUE);
	}

	SetDrawScreen(DX_SCREEN_BACK);

	return _handle;
}
