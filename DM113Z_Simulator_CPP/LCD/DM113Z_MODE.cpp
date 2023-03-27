#include "DM113Z_MODE.h"

#define _SET_SEG(cond,idx)	DrawGraph(0,idx * (_img_height + _gap),(cond)? _handle_on : _handle_off,TRUE)

DM113Z_MODE::DM113Z_MODE(int gap) : _gap(gap)
{
	_handle = 0;
	_handle_on = 0;
	_handle_off = 0;

	_mode.d8 = 0x00;
	_width = 0;
	_height = 0;
	_img_height = 0;
}

DM113Z_MODE::~DM113Z_MODE(void)
{
	DeleteGraph(_handle_on);
	DeleteGraph(_handle_off);

	DeleteGraph(_handle);
}

void DM113Z_MODE::init(void)
{
	int ret;
	char name[20];
	/*テクスチャの読み込み*/
	sprintf_s(name, "%s1_ON.png", _path);
	_handle_on = LoadGraph(name);

	sprintf_s(name, "%s1_OFF.png", _path);
	_handle_off = LoadGraph(name);

	/*サイズの取得*/
	ret = GetGraphSize(_handle_on, &_width, &_img_height);
	assert(ret != -1);

	_height = _img_height * 4 + _gap * 3;

	/*スクリーンの作成*/
	ret = MakeScreen(_width, _height, TRUE);

	assert(ret != -1);

	_mode.d8 = 0x00;
	_handle = ret;
}

void DM113Z_MODE::set(mode_t* mode)
{
	memcpy(&_mode, mode, sizeof(_mode));
}

int DM113Z_MODE::draw(void)
{
	SetDrawScreen(_handle);

	DrawBox(0, 0, _width, _height, _COLOUR_BG, TRUE);

	_SET_SEG(_mode.a, 0);
	_SET_SEG(_mode.b, 1);
	_SET_SEG(_mode.c, 2);
	_SET_SEG(_mode.d, 3);

	SetDrawScreen(DX_SCREEN_BACK);

	return _handle;
}
