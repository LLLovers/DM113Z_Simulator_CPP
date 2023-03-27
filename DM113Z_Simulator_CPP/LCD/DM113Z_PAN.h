#pragma once

#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "config.h"
#include "DXLib.h"

class DM113Z_PAN
{
public:
	union segment_t {
		struct {
			uint8_t border : 1;
			uint8_t a : 1;
			uint8_t b : 1;
			uint8_t c : 1;
			uint8_t d : 1;
			uint8_t e : 1;
			uint8_t f : 1;
			uint8_t g : 1;
		};
		uint8_t d8;
	};

	DM113Z_PAN(void);
	~DM113Z_PAN(void);

	void init(void);

	void set(union segment_t*);

	int draw(void);

	int get_width(void) { return _width; }
	int get_height(void) { return _height; }

private:
	union segment_t _pan;

	const char* _path = "pan/";

	int _handle;

	int _handle_on[8];
	int _handle_off[8];

	int _width;
	int _height;
};

