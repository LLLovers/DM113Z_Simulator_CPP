#pragma once

#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "config.h"
#include "DXLib.h"


#include "DM113Z_SEG.h"
#include "DM113Z_SIGN.h"

class DM113Z_KEY
{
public:
	DM113Z_KEY(int);
	~DM113Z_KEY(void);

	void init(void);

	void set(int, uint8_t);

	int draw(void);

	int get_width(void) { return _width; }
	int get_height(void) { return _height; }
private:
	int _gap;
	int _seg_handle;
	int _handle;

	int _width;
	int _height;

	DM113Z_SEG** _seg;
	DM113Z_SIGN* _sign;
};

