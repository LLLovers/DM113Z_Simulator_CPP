#pragma once

#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "config.h"
#include "DXLib.h"


class DM113Z_GFX
{
public:
	DM113Z_GFX(int, int, int, int, int);
	~DM113Z_GFX(void);

	void init(void);

	void clear(void);

	void set(uint8_t*);

	int draw(void);

	int get_height(void) { return _height; }
	int get_width(void) { return _width; }

private:
	int _gfx_handle;

	uint8_t* _gfx;

	int _width;
	int _height;

	int _pixel_count;
	int _pixel_width;
	int _pixel_height;
	int _pixel_gap;

	int _gfx_width;
	int _gfx_height;
};

