#include "DM113Z_PART.h"

DM113Z_PART::DM113Z_PART(int pixel_width, int pixel_height, int pixel_gap) : _pixel_width(pixel_width), _pixel_height(pixel_height), _pixel_gap(pixel_gap)
{
	_chr = new DM113Z_CHR * [2];

	for (int i = 0; i < 2; i++) {
		_chr[i] = new DM113Z_CHR(_pixel_width, _pixel_height, _pixel_gap);
	}

	_part_width = (_chr[0]->get_width() + _pixel_width + _pixel_gap) * 2;
	_part_height = (_chr[0]->get_height());

	_chr_handle = 0;
	_part_handle = 0;
}

DM113Z_PART::~DM113Z_PART(void)
{
	delete[] _chr;

	DeleteGraph(_chr_handle);
	DeleteGraph(_part_handle);
}

void DM113Z_PART::init(void)
{
	int ret = MakeScreen(_part_width, _part_height);

	for (int i = 0; i < 2; i++) {
		_chr[i]->init();
	}

	assert(ret != -1);
	_part_handle = ret;
}

void DM113Z_PART::clear(void)
{
	for (int i = 0; i < 2; i++) {
		_chr[i]->clear();
	}
}

void DM113Z_PART::set(int pos, uint8_t* font)
{
	_chr[pos]->set(font);
}

int DM113Z_PART::draw(void)
{
	int x;

	SetDrawScreen(_part_handle);

	DrawBox(0, 0, _part_width, _part_height, _COLOUR_BG, TRUE);

	for (x = 0; x < 2; x++) {
		int sx = (_chr[0]->get_width() + _pixel_width + _pixel_gap) * x;

		_chr_handle = _chr[x]->draw();

		SetDrawScreen(_part_handle);
		DrawGraph(sx, 0, _chr_handle, TRUE);
	}

	SetDrawScreen(DX_SCREEN_BACK);

	return _part_handle;
}
