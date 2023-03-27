#pragma once
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "config.h"
#include "DXLib.h"

class DM113Z_SEG
{
public:
	union segment_t{
		struct {
			uint8_t padding : 1;
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

	DM113Z_SEG(void);
	~DM113Z_SEG(void);

	void init(void);

	void set(union segment_t*);

	int draw(void);

	int get_width(void) { return _width; }
	int get_height(void) { return _height; }

private:
	const char* _path = "seg/";

	union segment_t _segment;

	int _handle;

	int _handle_on[7];
	int _handle_off[7];

	int _width;
	int _height;
};

