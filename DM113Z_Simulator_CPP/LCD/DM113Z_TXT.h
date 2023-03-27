#pragma once

#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "config.h"
#include "DXLib.h"
#include "DM113Z_CHR.h"

/// <summary>
/// 画面上部テキスト領域
/// </summary>

class DM113Z_TXT
{
public:
	DM113Z_TXT(int, int, int, int, int);
	~DM113Z_TXT(void);

	void init(void);

	void clear(void);

	void set(int, int, uint8_t*);

	int draw(void);

	int get_width(void) { return _txt_width; }
	int get_height(void) { return _txt_height; }

private:
	int _count_x;
	int _count_y;
	int _count;

	int _pixel_width;
	int _pixel_height;
	int _pixel_gap;

	int _chr_handle;
	int _txt_handle;

	int _txt_width;
	int _txt_height;

	DM113Z_CHR** _chr;
};

