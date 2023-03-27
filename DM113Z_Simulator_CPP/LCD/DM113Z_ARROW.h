#pragma once

#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "DXLib.h"
#include "config.h"

class DM113Z_ARROW
{
public:

	DM113Z_ARROW(void);
	~DM113Z_ARROW(void);

	void init(void);

	void set(int);

	int draw(void);

	int get_width(void) { return _width; }
	int get_height(void) { return _height; }

private:

	int _width;
	int _height;

	int _handle_on;
	int _handle_off;

	int _state;

	int _handle;

	const char* _path = "arrow/";
};

