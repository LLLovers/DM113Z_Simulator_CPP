#pragma once

#include <stdint.h>
#include "DXLib.h"
#include <process.h>
#include "DM113Z.h"

/*
	HD44780 --- DM113Z•ÏŠ·
*/

int lcd_is_running(void);

void lcd_init(void);

void lcd_locate(uint8_t, uint8_t);

void lcd_putc(uint8_t);

void lcd_setcg(uint8_t, uint8_t, const uint8_t*);

void lcd_clear(void);