#include "hd44780_win.h"

bool isRunning = false;

static uint8_t addr;

static uint8_t ddram[20 * 4];
static uint8_t cgram[8 * 8];

static uint8_t gfx[32];

/*マルチタスク関連*/

static HANDLE mutex = CreateMutex(NULL, FALSE, NULL);

static HANDLE mainLoop;

static DM113Z* lcd;

DM113Z_PAN::segment_t s_pan;

uint8_t seg[3];


static void update_new(uint8_t);
static void update_rewrite(uint8_t);

static void set_vol(uint8_t);
static void set_exp(uint8_t);

static void set_pan(uint8_t);
static void set_rev(uint8_t);
static void set_cho(uint8_t);
static void set_var(uint8_t);
static void set_key(uint8_t, uint8_t);
static void set_arrow(uint8_t);
static void set_mode(uint8_t);
static void set_gfx(uint8_t, uint8_t);

static void set_mic(uint8_t);
static void set_line(uint8_t);
static void set_ch(uint8_t);
static void set_bank_pgm(uint8_t);

unsigned int wrap(void* x)
{
	int ret = 1;

	lcd->init();

	isRunning = true;

	while (ret) {
		ret = lcd->callback();
	}

	isRunning = false;

	_endthreadex(0);

	return 0;
}

int lcd_is_running(void)
{
	return isRunning;
}

void lcd_init(void)
{
	lcd = new DM113Z;
	mainLoop = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall*)(void*))wrap, 0, 0, NULL);
	while (lcd->isReady == 0);

	memset(ddram, 0x20, 80);

	return;
}

void lcd_locate(uint8_t y, uint8_t x) {
	addr = y * 20 + x;
}

void lcd_putc(uint8_t c) {
	ddram[addr] = c;

	/*描画対象を探して書き換える*/
	update_new(addr);

	addr = (addr + 1) % 80;
}

void lcd_setcg(uint8_t c, uint8_t n, const uint8_t* font) {


	for (int i = 0; i < n; i++) {
		memcpy(&cgram[(i + c) * 8], (uint8_t*)&font[8 * i], 8);
		lcd->set_cgram(i + c, (uint8_t*)&font[8 * i]);
	}

	/*描画対象を探して書き換える*/
	for (int i = 0; i < n; i++) {
		update_rewrite(i + c);
	}
}


void lcd_clear(void)
{
	lcd->clear();
	memset(ddram, 0x20, 80);
	memset(cgram, 0x00, 64);
	addr = 0;
}


/*書き換え*/
static void update_new(uint8_t addr) {
	uint8_t x = addr % 20;
	uint8_t y = addr / 20;

	if (y < 2) {
		if (x < 17) {
			/*キャラクタエリア*/
			lcd->set_char_txt(x, y, ddram[addr]);
		}
		else {
			if (y == 0) {
				if (x == segment_map::vol::x[0]) {
					/*VOL・EXP*/
					set_vol(ddram[addr]);
					set_exp(ddram[addr]);
				}
				else {
					/*PART*/
					lcd->set_char_part(x - 17, ddram[addr]);
				}
			}
			else {
				/*MIDI*/
				lcd->set_char_midi(x - 17, ddram[addr]);
			}
		}
	}
	else {
		/*クソ面倒なグラフィックエリア*/
		set_gfx(addr, ddram[addr]);
		if (x == 3) {
			if (y == 2) {
				/*CH,BANK_PGM,MIC,LINE,KEY,ARROW,MODE*/
				set_ch(ddram[addr]);
				set_bank_pgm(ddram[addr]);
				set_mic(ddram[addr]);
				set_line(ddram[addr]);
				set_key(addr, ddram[addr]);
				set_arrow(ddram[addr]);
				set_mode(ddram[addr]);
			}
			else {
				/*PAN,REV,CHO,VAR,KEY*/
				set_pan(ddram[addr]);
				set_rev(ddram[addr]);
				set_cho(ddram[addr]);
				set_var(ddram[addr]);
				set_key(addr, ddram[addr]);
			}
		}
	}
}

static void update_rewrite(uint8_t c) {
	for (int i = 0; i < 80; i++) {
		if (ddram[i] == c) {
			update_new(i);
		}
	}
}

static void set_vol(uint8_t c) {
	uint8_t* font = (c < 8) ? &cgram[8 * c] : (uint8_t*)&fontdata[c];

	DM113Z_BAR_RECT::segment_t seg;

	seg.d8 = 0;

	for (int i = 0; i < 8; i++) {
		if (font[segment_map::vol::l[i]] & (uint8_t)segment_map::vol::b[i]) {
			seg.d8 |= (1 << (7 - i));
		}
	}

	lcd->set_param(DM113Z::PARAM_VOL, &seg.d8);
}

static void set_exp(uint8_t c) {
	uint8_t* font = (c < 8) ? &cgram[8 * c] : (uint8_t*)&fontdata[c];

	DM113Z_BAR_RECT::segment_t seg;

	seg.d8 = 0;

	for (int i = 0; i < 8; i++) {
		if (font[segment_map::exp::l[i]] & (uint8_t)segment_map::exp::b[i]) {
			seg.d8 |= (1 << (7 - i));
		}
	}

	lcd->set_param(DM113Z::PARAM_EXP, &seg.d8);
}

static void set_pan(uint8_t c) {
	uint8_t* font = (c < 8) ? &cgram[8 * c] : (uint8_t*)&fontdata[c];

	uint8_t s = s_pan.d8;

	s &= (0x01);

	for (int i = 0; i < 7; i++) {
		s |= (uint8_t)((font[segment_map::pan::l[i]] & (segment_map::pan::b[i])) ? 1 : 0) << (7 - i);
	}

	s_pan.d8 = s;

	lcd->set_param(DM113Z::PARAM_PAN, &s_pan.d8);
}

static void set_rev(uint8_t c) {
	uint8_t* font = (c < 8) ? &cgram[8 * c] : (uint8_t*)&fontdata[c];

	uint8_t s = 0;

	for (int i = 0; i < 8; i++) {
		s |= (uint8_t)((font[segment_map::rev::l[i]] & (segment_map::rev::b[i])) ? 1 : 0) << (7 - i);
	}

	lcd->set_param(DM113Z::PARAM_REV, &s);
}

static void set_cho(uint8_t c) {
	uint8_t* font = (c < 8) ? &cgram[8 * c] : (uint8_t*)&fontdata[c];

	uint8_t s = 0;

	for (int i = 0; i < 8; i++) {
		s |= (uint8_t)((font[segment_map::cho::l[i]] & (segment_map::cho::b[i])) ? 1 : 0) << (7 - i);
	}

	lcd->set_param(DM113Z::PARAM_CHO, &s);
}


static void set_var(uint8_t c) {
	uint8_t* font = (c < 8) ? &cgram[8 * c] : (uint8_t*)&fontdata[c];

	uint8_t s = 0;

	for (int i = 0; i < 8; i++) {
		s |= (uint8_t)((font[segment_map::var::l[i]] & (segment_map::var::b[i])) ? 1 : 0) << (7 - i);
	}

	lcd->set_param(DM113Z::PARAM_VAR, &s);
}

static void set_key(uint8_t addr, uint8_t c) {
	uint8_t x = addr % 20;
	uint8_t y = addr / 20;

	uint8_t* font = (c < 8) ? &cgram[8 * c] : (uint8_t*)&fontdata[c];

	/*符号*/
	if (x == segment_map::key::sign::x[0] && y == segment_map::key::sign::y[0]) {
		seg[0] = 0;
		seg[0] |= ((font[segment_map::key::sign::l[0]] & segment_map::key::sign::b[0]) ? 1 : 0);
		seg[0] |= ((font[segment_map::key::sign::l[1]] & segment_map::key::sign::b[1]) ? 2 : 0);
	}

	/*数字*/
	if (y == 2) {
		/*D1のa,b,c,d,e,g*/
		seg[1] &= (uint8_t)0x40;
		for (int i = 0; i < 7; i++) {
			if (y != segment_map::key::d1::y[i])continue;
			seg[1] |= (uint8_t)((font[segment_map::key::d1::l[i]] & (segment_map::key::d1::b[i])) ? 1 : 0) << (1 + i);
		}

		/*D0のa,b,c,d,e,f,g*/
		seg[2] = 0;
		for (int i = 0; i < 7; i++) {
			seg[2] |= (uint8_t)((font[segment_map::key::d0::l[i]] & (segment_map::key::d0::b[i])) ? 1 : 0) << (1 + i);
		}
	}
	else {
		/*D1のf*/
		seg[1] &= ~(uint8_t)(0x40);
		seg[1] |= ((font[segment_map::key::d1::l[5]] & segment_map::key::d1::b[5]) ? 0x40 : 0x00);
	}

	lcd->set_param(DM113Z::PARAM_KEY, seg);
}

static void set_arrow(uint8_t c) {
	uint8_t* font = (c < 8) ? &cgram[8 * c] : (uint8_t*)&fontdata[c];
	for (int i = 0; i < 10; i++) {
		lcd->set_segment((DM113Z::seg_e)i, (font[segment_map::arrow::l[i]] & segment_map::arrow::b[i]) ? 1 : 0);
	}
}

static void set_mode(uint8_t c) {
	uint8_t* font = (c < 8) ? &cgram[8 * c] : (uint8_t*)&fontdata[c];
	uint8_t mode = 0x00;
	for (int i = 0; i < 4; i++) {
		mode |= ((font[segment_map::mode::l[i]] & segment_map::mode::b[i]) ? 1 : 0) << (i + 4);
	}

	lcd->set_param(DM113Z::PARAM_MODE, &mode);
}

static void set_gfx(uint8_t addr, uint8_t c) {
	uint8_t* font = (c < 8) ? &cgram[8 * c] : (uint8_t*)&fontdata[c];

	/*

		gfx:7	6	5	4	3	2	1	0	7	6	5	4	3	2	1	0
		chr:4	3	2	1	0	4	3	2	1	0	4	3	2	1	0	4

	*/

	switch (addr) {
	case 40:
		for (int i = 0; i < 8; i++) {
			gfx[2 * i] &= ~((uint8_t)0xF8);
			gfx[2 * i] |= (uint8_t)((font[i] & (0x1F)) << 3);
		}
		break;
	case 41:
		for (int i = 0; i < 8; i++) {
			gfx[2 * i] &= ~((uint8_t)0x07);
			gfx[2 * i] |= (uint8_t)((font[i] & (0x1C)) >> 2);

			gfx[2 * i + 1] &= ~((uint8_t)0xC0);
			gfx[2 * i + 1] |= (uint8_t)((font[i] & (0x03)) << 6);
		}
		break;
	case 42:
		for (int i = 0; i < 8; i++) {
			gfx[2 * i + 1] &= ~((uint8_t)0x1E);
			gfx[2 * i + 1] |= (uint8_t)((font[i] & (0x1F)) << 1);
		}
		break;
	case 43:
		for (int i = 0; i < 8; i++) {
			gfx[2 * i + 1] &= ~((uint8_t)0x01);
			gfx[2 * i + 1] |= (uint8_t)((font[i] & (0x10)) >> 4);
		}
		break;
	case 60:
		for (int i = 8; i < 16; i++) {
			gfx[2 * i] &= ~((uint8_t)0xF8);
			gfx[2 * i] |= (uint8_t)((font[i - 8] & (0x1F)) << 3);
		}
		break;
	case 61:
		for (int i = 8; i < 16; i++) {
			gfx[2 * i] &= ~((uint8_t)0x07);
			gfx[2 * i] |= (uint8_t)((font[i - 8] & (0x1C)) >> 2);

			gfx[2 * i + 1] &= ~((uint8_t)0xC0);
			gfx[2 * i + 1] |= (uint8_t)((font[i - 8] & (0x03)) << 6);
		}
		break;
	case 62:
		for (int i = 8; i < 16; i++) {
			gfx[2 * i + 1] &= ~((uint8_t)0x1E);
			gfx[2 * i + 1] |= (uint8_t)((font[i - 8] & (0x1F)) << 1);
		}
		break;
	case 63:
		for (int i = 8; i < 16; i++) {
			gfx[2 * i + 1] &= ~((uint8_t)0x01);
			gfx[2 * i + 1] |= (uint8_t)((font[i - 8] & (0x10)) >> 4);
		}
		break;
	}

	lcd->set_gfx(gfx);
}

static void set_mic(uint8_t c) {
	uint8_t* font = (c < 8) ? &cgram[8 * c] : (uint8_t*)&fontdata[c];
	lcd->set_segment(DM113Z::SEG_MIC, (font[segment_map::mic::l] & segment_map::mic::b) ? 1 : 0);
}

static void set_line(uint8_t c) {
	uint8_t* font = (c < 8) ? &cgram[8 * c] : (uint8_t*)&fontdata[c];
	lcd->set_segment(DM113Z::SEG_LINE, (font[segment_map::line::l] & segment_map::line::b) ? 1 : 0);
}

static void set_ch(uint8_t c) {
	uint8_t* font = (c < 8) ? &cgram[8 * c] : (uint8_t*)&fontdata[c];

	lcd->set_segment(DM113Z::SEG_CH_LEFT, (font[segment_map::ch_left::l] & segment_map::ch_left::b) ? 1 : 0);
	lcd->set_segment(DM113Z::SEG_CH_RIGHT, (font[segment_map::ch_right::l] & segment_map::ch_right::b) ? 1 : 0);

	if (font[segment_map::ch_left::l] & segment_map::ch_left::b) {
		s_pan.border = 1;
	}
	else {
		s_pan.border = 0;
	}
}

static void set_bank_pgm(uint8_t c) {
	uint8_t* font = (c < 8) ? &cgram[8 * c] : (uint8_t*)&fontdata[c];
	lcd->set_segment(DM113Z::SEG_BANK_PGM_LEFT, (font[segment_map::bank_pgm_left::l] & segment_map::bank_pgm_left::b) ? 1 : 0);
	lcd->set_segment(DM113Z::SEG_BANK_PGM_RIGHT, (font[segment_map::bank_pgm_right::l] & segment_map::bank_pgm_right::b) ? 1 : 0);
}