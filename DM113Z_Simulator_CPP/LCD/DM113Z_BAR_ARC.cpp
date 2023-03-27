#include "DM113Z_BAR_ARC.h"

DM113Z_BAR_ARC::DM113Z_BAR_ARC(void)
{
	_handle = 0;

	memset(_handle_on, 0x00, sizeof(_handle_on));
	memset(_handle_off, 0x00, sizeof(_handle_off));

	_width = 0;
	_height = 0;

	_seg.d8 = 0x00;
}

DM113Z_BAR_ARC::~DM113Z_BAR_ARC(void)
{
	for (int i = 0; i < 8; i++) {
		DeleteGraph(_handle_on[i]);
		DeleteGraph(_handle_off[i]);
	}

	DeleteGraph(_handle);
}

void DM113Z_BAR_ARC::init(void)
{
	int ret;

	/*テクスチャの読み込み*/
	for (int i = 0; i < 8; i++) {
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

	_seg.d8 = 0x00;
	_handle = ret;
}

void DM113Z_BAR_ARC::set(segment_t* seg)
{
	memcpy(&_seg, seg, sizeof(_seg));
}

int DM113Z_BAR_ARC::draw(void)
{
	int y = 0;

	SetDrawScreen(_handle);

	DrawBox(0, 0, _width, _height, _COLOUR_BG, TRUE);
#define _SET_SEG(cond,idx)	DrawGraph(0,0,(cond)? _handle_on[idx] : _handle_off[idx],TRUE)

	for (int y = 0; y < 8; y++) {
		_SET_SEG(_seg.d8 & (1 << (7 - y)), y);
	}

	SetDrawScreen(DX_SCREEN_BACK);

	return _handle;
}
