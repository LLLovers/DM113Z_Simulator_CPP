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

static void update_new(uint8_t);
static void update_rewrite(uint8_t);

static void set_vol(uint8_t);
static void set_exp(uint8_t);


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

				}
				else {
					/*PART*/

				}
			}
			else {
				/*MIDI*/

			}
		}
	}
	else {
		/*クソ面倒なグラフィックエリア*/
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