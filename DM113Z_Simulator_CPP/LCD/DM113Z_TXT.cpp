#include "DM113Z_TXT.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="count_x">1行当たりの桁数</param>
/// <param name="count_y">行数</param>
/// <param name="pixel_width">１ドットあたりの幅</param>
/// <param name="pixel_height">１ドットあたりの高さ</param>
/// <param name="pixel_gap">ドット間の間隔</param>
DM113Z_TXT::DM113Z_TXT(int count_x, int count_y, int pixel_width, int pixel_height, int pixel_gap) :
	_count_x(count_x), _count_y(count_y), _pixel_width(pixel_width), _pixel_height(pixel_height), _pixel_gap(pixel_gap), _txt_handle(0), _chr_handle(0)
{
	_count = _count_x * _count_y;
	assert(_count > 0);

	_chr = new DM113Z_CHR * [_count];

	for (int i = 0; i < _count; i++) {
		_chr[i] = new DM113Z_CHR(_pixel_width, _pixel_height, _pixel_gap);
	}

	_txt_width = _pixel_gap + (_chr[0]->get_width() + _pixel_width + _pixel_gap) * _count_x;
	_txt_height = _pixel_gap + (_chr[0]->get_height()) * _count_y;
}

DM113Z_TXT::~DM113Z_TXT(void)
{
	delete[] _chr;

	DeleteGraph(_chr_handle);
	DeleteGraph(_txt_handle);
}

void DM113Z_TXT::init(void)
{
	int ret = MakeScreen(_txt_width, _txt_height);

	for (int i = 0; i < _count; i++) {
		_chr[i]->init();
	}

	assert(ret != -1);
	_txt_handle = ret;
}

void DM113Z_TXT::clear(void)
{
	for (int i = 0; i < _count; i++) {
		_chr[i]->clear();
	}
}

void DM113Z_TXT::set(int x, int y, uint8_t* font)
{
	_chr[y * _count_x + x]->set(font);
}

int DM113Z_TXT::draw()
{
	int x, y;

	SetDrawScreen(_txt_handle);

	DrawBox(0, 0, _txt_width, _txt_height, _COLOUR_BG, TRUE);

	for (y = 0; y < _count_y; y++) {
		for (x = 0; x < _count_x; x++) {
			int sx = _pixel_gap + (_chr[0]->get_width() + _pixel_width + _pixel_gap) * x;
			int sy = _pixel_gap + (_chr[0]->get_height()) * y;

			_chr_handle = _chr[y * _count_x + x]->draw();

			SetDrawScreen(_txt_handle);
			DrawGraph(sx, sy, _chr_handle, TRUE);
		}
	}

	SetDrawScreen(DX_SCREEN_BACK);

	return _txt_handle;
}
