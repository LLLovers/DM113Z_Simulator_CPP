#pragma once

#include <stdint.h>
#include <string.h>
#include <assert.h>


#include "config.h"
#include "DXLib.h"
#include "DM113Z_CHR.h"

/// <summary>
/// 画面左下パート表示
/// </summary>

class DM113Z_PART
{
public:
	DM113Z_PART(int, int, int);
	~DM113Z_PART(void);

	void init(void);

	void clear(void);

	void set(int, uint8_t*);

	int draw(void);

	int get_width(void) { return _part_width; }
	int get_height(void) { return _part_height; }

private:

	const int _pixel_width;
	const int _pixel_height;
	const int _pixel_gap;

	int _chr_handle;
	int _part_handle;

	int _part_width;
	int _part_height;

	DM113Z_CHR** _chr;
};

