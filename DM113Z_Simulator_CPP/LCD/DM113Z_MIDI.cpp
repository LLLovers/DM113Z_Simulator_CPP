#include "DM113Z_MIDI.h"

DM113Z_MIDI::DM113Z_MIDI(int pixel_width, int pixel_height, int pixel_gap) : _pixel_width(pixel_width), _pixel_height(pixel_height), _pixel_gap(pixel_gap)
{
	_chr = new DM113Z_CHR * [3];

	for (int i = 0; i < 3; i++) {
		_chr[i] = new DM113Z_CHR(_pixel_width, _pixel_height, _pixel_gap);
	}

	_width = (_chr[0]->get_width() + _pixel_width + _pixel_gap) * 3;
	_height = (_chr[0]->get_height());

	_chr_handle = 0;
	_midi_handle = 0;
}

DM113Z_MIDI::~DM113Z_MIDI(void)
{
	delete[] _chr;

	DeleteGraph(_chr_handle);
	DeleteGraph(_midi_handle);
}

void DM113Z_MIDI::init(void)
{
	int ret = MakeScreen(_width, _height);

	for (int i = 0; i < 3; i++) {
		_chr[i]->init();
	}

	assert(ret != -1);
	_midi_handle = ret;
}

void DM113Z_MIDI::clear(void)
{
	for (int i = 0; i < 3; i++) {
		_chr[i]->clear();
	}
}

void DM113Z_MIDI::set(int idx, uint8_t* font)
{
	_chr[idx]->set(font);
}

int DM113Z_MIDI::draw(void)
{
	int x;

	SetDrawScreen(_midi_handle);

	DrawBox(0, 0, _width, _height, _COLOUR_BG, TRUE);

	for (x = 0; x < 3; x++) {
		int sx = (_chr[0]->get_width() + _pixel_width + _pixel_gap) * x;

		_chr_handle = _chr[x]->draw();

		SetDrawScreen(_midi_handle);
		DrawGraph(sx, 0, _chr_handle, TRUE);
	}

	SetDrawScreen(DX_SCREEN_BACK);

	return _midi_handle;
}
