#pragma once

#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "DXLib.h"
#include "config.h"


class DM113Z_SIGN
{
public:

	enum sign_e {
		horizontal = 1,
		vertical = 2,
	};

	DM113Z_SIGN(void);
	~DM113Z_SIGN(void);

	void init(void);

	void set(uint8_t);

	int draw(void);

	int get_width(void) { return _width; }
	int get_height(void) { return _height; }

private:

	int _width;
	int _height;

	int _handle_on[2];
	int _handle_off[2];

	uint8_t _state;

	int _handle;

	const char* _path = "seg/s";
};

