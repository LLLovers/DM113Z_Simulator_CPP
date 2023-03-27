/*
*	DM113Z Simulator(C++”Å)
*
*/

#ifdef _WIN32
#include <Windows.h>
#else
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#endif

#ifdef _WIN32
#define SLEEP(x) Sleep(x)
#else
#define SLEEP(x) _delay_ms(x)
#endif

#include "DRV113Z.h"

#define V(x) ((tbl[(k + x) % sizeof(tbl)]))

uint8_t arr[] = {
	0x1f,0x1f,0x1f,0x10,0x15,0x15,0x15,0x15,
	0x1f,0x1f,0x1f,0x1a,0x0b,0x0b,0x0a,0x0a,
	0x1f,0x1f,0x1f,0x06,0x1a,0x1a,0x1a,0x1a,
	0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
	0x15,0x15,0x15,0x1f,0x1f,0x10,0x1f,0x1f,
	0x0a,0x0a,0x0a,0x1f,0x1f,0x00,0x1f,0x1f,
	0x1a,0x1a,0x02,0x1f,0x1f,0x00,0x1f,0x1f,
	0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10
};

const uint8_t tbl[] = {
	8,9,11,12,13,14,15,15,15,15,15,14,13,12,11,9,8,6,4,3,2,1,0,0,0,0,0,1,2,3,4,6
};

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(void)
#endif
{
	int k = 0;
	int fp = 1, fve = 1;
	int p = 0;
	int ve = 0;
	int key = 0;
	int c = 0;
	int r = 0;
	drv113z_init();
	drv113z_set_inst((char*)"ItWorks!!", sizeof("ItWorks!!"), 123, 456);
	drv113z_set_arrow(ARROW_PAN, 1);
	drv113z_set_mode(0);
	drv113z_set_param(PARAM_PART, 13);
	drv113z_set_param(PARAM_MIDI, 13);
	drv113z_set_gfx(arr);

	while (lcd_is_running()) {
		for (int j = 0; j < 16; j++) {
			drv113z_set_bar_unsigned(j + 2, V(j));
		}

		if (k % 5 == 0) {
			drv113z_set_param(PARAM_PAN, p - 3);
			if (fp) {
				if (p < 6) {
					p += 1;
				}
				else {
					fp = 0;
				}
			}
			else {
				if (p > 0) {
					p -= 1;
				}
				else {
					fp = 1;
				}
			}
			drv113z_set_param(PARAM_VOL, ve);
			drv113z_set_param(PARAM_EXP, 8 - ve);
			if (fve) {
				if (ve < 8) {
					ve += 1;
				}
				else {
					fve = 0;
				}
			}
			else {
				if (ve > 0) {
					ve -= 1;
				}
				else {
					fve = 1;
				}
			}
		}

		if (k % 3 == 0) {
			drv113z_set_param(PARAM_CHO, c);
			drv113z_set_param(PARAM_KEY, key - 24);
			key = (key < 48) ? key + 1 : 0;
			c = (c <= 8) ? c + 1 : 0;
		}
		drv113z_set_param_pfm(PARAM_REV, r);
		r = (r <= 8) ? r + 1 : 0;
		k = (k < 1000) ? k + 1 : 0;
		SLEEP(40);
	}

	return 0;
}