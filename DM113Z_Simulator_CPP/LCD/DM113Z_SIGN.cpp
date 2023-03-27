#include "DM113Z_SIGN.h"

DM113Z_SIGN::DM113Z_SIGN(void)
{
	_handle = 0;

	memset(_handle_on, 0x00, sizeof(_handle_on));
	memset(_handle_off, 0x00, sizeof(_handle_off));

	_width = 0;
	_height = 0;

	_state = 0;
}

DM113Z_SIGN::~DM113Z_SIGN(void)
{
	for (int i = 0; i < 2; i++) {
		DeleteGraph(_handle_on[i]);
		DeleteGraph(_handle_off[i]);
	}

	DeleteGraph(_handle);
}

void DM113Z_SIGN::init(void)
{
	int ret;
	/*テクスチャ読み込み*/
	for (int i = 0; i < 2; i++) {
		char name[16];
		sprintf_s(name, "%s%d_ON.png", _path, i);
		_handle_on[i] = LoadGraph(name);
		sprintf_s(name, "%s%d_OFF.png", _path, i);
		_handle_off[i] = LoadGraph(name);
		assert(_handle_on[i] != -1 && _handle_off[i] != -1);
	}

	/*サイズの取得*/
	ret = GetGraphSize(_handle_on[0], &_width, &_height);
	assert(ret != -1);

	/*スクリーン作成*/
	ret = MakeScreen(_width, _height, TRUE);
	assert(ret != -1);

	_state = 0;
	_handle = ret;
}

void DM113Z_SIGN::set(uint8_t state)
{
	_state = state;
}

int DM113Z_SIGN::draw(void)
{
	SetDrawScreen(_handle);

	DrawBox(0, 0, _width, _height, _COLOUR_BG, TRUE);

	DrawGraph(0, 0, (_state & (uint8_t)horizontal) ? _handle_on[0] : _handle_off[0], TRUE);
	DrawGraph(0, 0, (_state & (uint8_t)vertical) ? _handle_on[1] : _handle_off[1], TRUE);

	SetDrawScreen(DX_SCREEN_BACK);

	return _handle;
}
