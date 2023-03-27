#include "DM113Z_PAN.h"

#define _SET_SEG(cond,idx)	DrawGraph(0,0,(cond)? _handle_on[idx] : _handle_off[idx],TRUE)

DM113Z_PAN::DM113Z_PAN(void)
{
	memset(_handle_on, 0x00, sizeof(_handle_on));
	memset(_handle_off, 0x00, sizeof(_handle_off));

	_pan.d8 = 0x00;
	_width = 0;
	_height = 0;
}

DM113Z_PAN::~DM113Z_PAN(void)
{
	for (int i = 0; i < 8; i++) {
		DeleteGraph(_handle_on[i]);
		DeleteGraph(_handle_off[i]);
	}

	DeleteGraph(_handle);
}

void DM113Z_PAN::init(void)
{
	int ret;
	char name[20];
	/*テクスチャの読み込み*/
	for (int i = 0; i < 7; i++) {

		sprintf_s(name, "%s%d_ON.png", _path, i - 3);
		_handle_on[i] = LoadGraph(name);
		sprintf_s(name, "%s%d_OFF.png", _path, i - 3);
		_handle_off[i] = LoadGraph(name);
		assert(_handle_on[i] != -1 && _handle_off[i] != -1);
	}
	sprintf_s(name, "%sborder_ON.png", _path);
	_handle_on[7] = LoadGraph(name);

	sprintf_s(name, "%sborder_OFF.png", _path);
	_handle_off[7] = LoadGraph(name);

	/*サイズの取得*/
	ret = GetGraphSize(_handle_on[0], &_width, &_height);
	assert(ret != -1);

	/*スクリーンの作成*/
	ret = MakeScreen(_width, _height, TRUE);
	assert(ret != -1);

	_pan.d8 = 0x00;
	_handle = ret;
}

void DM113Z_PAN::set(union segment_t* pan)
{
	memcpy(&_pan, pan, sizeof(_pan));
}

int DM113Z_PAN::draw(void)
{
	SetDrawScreen(_handle);

	DrawBox(0, 0, _width, _height, _COLOUR_BG, TRUE);

	_SET_SEG(_pan.a, 0);
	_SET_SEG(_pan.b, 1);
	_SET_SEG(_pan.c, 2);
	_SET_SEG(_pan.d, 3);
	_SET_SEG(_pan.e, 4);
	_SET_SEG(_pan.f, 5);
	_SET_SEG(_pan.g, 6);
	_SET_SEG(_pan.border, 7);

	SetDrawScreen(DX_SCREEN_BACK);

	return _handle;
}
