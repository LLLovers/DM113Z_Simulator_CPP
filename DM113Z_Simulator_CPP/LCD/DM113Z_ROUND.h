#pragma once

#include <assert.h>
#include <string.h>

#include "config.h"
#include "DxLib.h"


class DM113Z_ROUND
{
public:
	DM113Z_ROUND(int,int,char*);
	~DM113Z_ROUND(void);

	void init(int, int);

	void set(int);

	int draw(void);

	int get_width(void) { return _width; }

	int get_height(void) { return _height; }
private:

	int _state;

	int _font_handle;
	int _font_size;

	int _handle;

	char _str[5];

	int _str_width;

	int _width;
	int _height;

};

