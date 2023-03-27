#pragma once
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "config.h"
#include "DXLib.h"
#include "DM113Z_CHR.h"

/// <summary>
/// âÊñ ç∂â∫MIDIÉ`ÉÉÉlÉãï\é¶
/// </summary>

class DM113Z_MIDI
{
public:
	DM113Z_MIDI(int, int, int);
	~DM113Z_MIDI(void);
	void init(void);

	void clear(void);

	void set(int, uint8_t*);

	int draw(void);

	int get_width(void) { return _width; }
	int get_height(void) { return _height; }

private:

	const int _pixel_width;
	const int _pixel_height;
	const int _pixel_gap;

	int _chr_handle;
	int _midi_handle;

	int _width;
	int _height;

	DM113Z_CHR** _chr;
};

