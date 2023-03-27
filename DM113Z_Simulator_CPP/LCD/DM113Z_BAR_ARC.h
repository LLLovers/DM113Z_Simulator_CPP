#pragma once

#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "DXLib.h"
#include "config.h"

class DM113Z_BAR_ARC
{
public:
	union segment_t {
		struct {
			uint8_t a : 1;
			uint8_t b : 1;
			uint8_t c : 1;
			uint8_t d : 1;
			uint8_t e : 1;
			uint8_t f : 1;
			uint8_t g : 1;
			uint8_t h : 1;
		};
		uint8_t d8;
	};

	DM113Z_BAR_ARC(void);
	~DM113Z_BAR_ARC(void);

	void init(void);

	void set(union segment_t*);

	int draw(void);

	int get_height(void) { return _height; }
	int get_width(void) { return _width; }

private:
	const char* _path = "arc/";

	const int _count = 8;

	int _handle_on[8];
	int _handle_off[8];

	int _handle;

	union segment_t _seg;

	int _width;
	int _height;
};

