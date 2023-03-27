#pragma once

#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "DXLib.h"
#include "config.h"
/// <summary>
///　各キャラクタ
/// </summary>



class DM113Z_CHR
{
public:
	DM113Z_CHR();
	DM113Z_CHR(int, int, int);

	void init(void);

	void set(uint8_t*);

	void clear(void);

	int draw(void);

	int get_height(void) { return _chr_height; }
	int get_width(void) { return _chr_width; }

private:
	const int count_x = 5;
	const int count_y = 8;

	int _chr_handle;

	uint8_t* _chr;

	int _pixel_width;
	int _pixel_height;
	int _pixel_gap;

	int _chr_width;
	int _chr_height;
};

