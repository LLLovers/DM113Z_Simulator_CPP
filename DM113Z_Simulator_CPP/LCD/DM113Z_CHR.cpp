#include "DM113Z_CHR.h"

DM113Z_CHR::DM113Z_CHR()
{
	/*デフォルトコンストラクタ*/
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="pixel_width">１ドットあたりの幅</param>
/// <param name="pixel_height">１ドットあたりの高さ</param>
/// <param name="pixel_gap">ドット間の間隔</param>
DM113Z_CHR::DM113Z_CHR(int pixel_width, int pixel_height, int pixel_gap) : _pixel_width(pixel_width), _pixel_height(pixel_height), _pixel_gap(pixel_gap), _chr_handle(0)
{
	assert(count_x <= 8);

	_chr_height = (_pixel_gap + _pixel_height) * count_y;
	_chr_width = (_pixel_gap + _pixel_width) * count_x;
	_chr = new uint8_t[count_x];
	memset(_chr, 0x00, count_y);
}

/// <summary>
/// 初期化
/// </summary>
/// <param name=""></param>
void DM113Z_CHR::init(void)
{
	int ret = MakeScreen(_chr_width, _chr_height);
	memset(_chr, 0x00, count_y);
	assert(ret != -1);
	_chr_handle = ret;
}

/// <summary>
/// フォントデータセット
/// </summary>
/// <param name="src">フォントの先頭ポインタ</param>
void DM113Z_CHR::set(uint8_t* src)
{
	memcpy(_chr, src, count_y);
}

/// <summary>
/// クリア
/// </summary>
/// <param name=""></param>
void DM113Z_CHR::clear(void)
{
	memset(_chr, 0x00, count_y);
}

/// <summary>
/// 描画
/// </summary>
/// <param name=""></param>
/// <returns>描画されたグラフィックへのハンドル</returns>
int DM113Z_CHR::draw(void)
{
	int x, y;

	SetDrawScreen(_chr_handle);

	DrawBox(0, 0, _chr_width, _chr_height, _COLOUR_BG, TRUE);

	for (y = 0; y < count_y; y++) {
		for (x = 0; x < count_x; x++) {
			bool cond = _chr[y] & (uint8_t)(1 << (count_x - 1 - x));
			int sx = (_pixel_width + _pixel_gap) * x;
			int sy = (_pixel_height + _pixel_gap) * y;

			DrawBox(sx, sy, sx + _pixel_width, sy + _pixel_height, (cond) ? _COLOUR_ON : _COLOUR_OFF, TRUE);
		}
	}

	SetDrawScreen(DX_SCREEN_BACK);

	return _chr_handle;
}
