#include "DM113Z_ARROW.h"

DM113Z_ARROW::DM113Z_ARROW(void)
{
	_width = 0;
	_height = 0;
	_state = 0;
	_handle_on = 0;
	_handle_off = 0;
	_handle = 0;
}

DM113Z_ARROW::~DM113Z_ARROW(void)
{
	DeleteGraph(_handle_on);
	DeleteGraph(_handle_off);
	DeleteGraph(_handle);
}

void DM113Z_ARROW::init(void)
{
	int ret;
	char name[20];
	/*�e�N�X�`���̓ǂݍ���*/
	sprintf_s(name, "%s0_ON.png", _path);
	_handle_on = LoadGraph(name);

	sprintf_s(name, "%s0_OFF.png", _path);
	_handle_off = LoadGraph(name);

	/*�T�C�Y�̎擾*/
	ret = GetGraphSize(_handle_on, &_width, &_height);

	assert(ret != -1);

	/*�X�N���[���̍쐬*/
	ret = MakeScreen(_width, _height, TRUE);

	assert(ret != -1);

	_state = 0;
	_handle = ret;
}

void DM113Z_ARROW::set(int state)
{
	_state = state;
}

int DM113Z_ARROW::draw(void)
{
	SetDrawScreen(_handle);

	DrawBox(0, 0, _width, _height, _COLOUR_BG, TRUE);

	DrawGraph(0, 0, (_state) ? _handle_on : _handle_off, TRUE);

	SetDrawScreen(DX_SCREEN_BACK);

	return _handle;
}
