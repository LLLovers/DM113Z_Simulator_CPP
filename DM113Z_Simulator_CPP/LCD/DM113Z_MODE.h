#pragma once

#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "config.h"
#include "DXLib.h"

class DM113Z_MODE
{
public:
	union mode_t {
		struct {
			uint8_t padding : 4;
			uint8_t a : 1;
			uint8_t b : 1;
			uint8_t c : 1;
			uint8_t d : 1;
		};
		uint8_t d8;
	};

	DM113Z_MODE(int);
	~DM113Z_MODE(void);

	void init(void);

	void set(union mode_t*);

	int draw(void);

	int get_width(void) { return _width; }
	int get_height(void) { return _height; }

private:
	union mode_t _mode;

	const char* _path = "arrow/";

	int _handle;

	int _handle_on;
	int _handle_off;

	int _gap;
	int _width;
	int _height;
	int _img_height;
};

