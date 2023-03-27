#pragma once

#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "config.h"
#include "DXLib.h"
class DM113Z_CH
{
public:
	DM113Z_CH(int,int,int);
	~DM113Z_CH(void);

	void init(int, int);

	void set(int,uint8_t);

	int draw(void);

	int get_width(void) { return _width; }

	int get_height(void) { return _height; }
private:
	uint8_t *_state;
	int* _pos;

	int _font_handle;
	int _font_size;

	int _handle;

	int _width;
	int _height;

	int _pixel_width;
	int _pixel_height;
	int _pixel_gap;
};

