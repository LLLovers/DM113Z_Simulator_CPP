#include "DM113Z_SEG.h"

#define _SET_SEG(cond,idx)	DrawGraph(0,0,(cond)? _handle_on[idx] : _handle_off[idx],TRUE)

DM113Z_SEG::DM113Z_SEG(void)
{
	memset(_handle_on, 0x00, sizeof(_handle_on));
	memset(_handle_off, 0x00, sizeof(_handle_off));

	_segment.d8 = 0x00;
	_width = 0;
	_height = 0;
}

DM113Z_SEG::~DM113Z_SEG(void)
{
	for (int i = 0; i < 7; i++) {
		DeleteGraph(_handle_on[i]);
		DeleteGraph(_handle_off[i]);
	}

	DeleteGraph(_handle);
}

void DM113Z_SEG::init(void)
{
	int ret;
	/*テクスチャの読み込み*/
	for (int i = 0; i < 7; i++) {
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

	/*スクリーンの作成*/
	ret = MakeScreen(_width, _height, TRUE);
	assert(ret != -1);

	_segment.d8 = 0x00;
	_handle = ret;
}

void DM113Z_SEG::set(union segment_t* seg)
{
	memcpy(&_segment, seg, sizeof(_segment));
}

int DM113Z_SEG::draw(void)
{
	SetDrawScreen(_handle);

	DrawBox(0, 0, _width, _height, _COLOUR_BG, TRUE);

	_SET_SEG(_segment.a, 0);
	_SET_SEG(_segment.b, 1);
	_SET_SEG(_segment.c, 2);
	_SET_SEG(_segment.d, 3);
	_SET_SEG(_segment.e, 4);
	_SET_SEG(_segment.f, 5);
	_SET_SEG(_segment.g, 6);

	SetDrawScreen(DX_SCREEN_BACK);

	return _handle;
}
