/*
 * DRV113Z.h
 *
 * Created: 2023/03/20 16:43:54
 *  Author: Owner
 */


#ifndef DRV113Z_H_
#define DRV113Z_H_

#include <string.h>

#include "LCD/hd44780_win.h"


enum drv113z_arrow_e {
	ARROW_MIDI = 0,
	ARROW_BANK,
	ARROW_PGM,
	ARROW_VOL,
	ARROW_EXP,
	ARROW_PAN,
	ARROW_REV,
	ARROW_CHO,
	ARROW_VAR,
	ARROW_KEY
};

enum drv113z_param_e {
	PARAM_PART = 0,
	PARAM_MIDI,
	PARAM_VOL,
	PARAM_EXP,
	PARAM_PAN,
	PARAM_REV,
	PARAM_CHO,
	PARAM_VAR,
	PARAM_KEY,
};

enum drv113z_display_type_e {
	DISPLAY_LEFT = 1,	/*左側にプログラム名等を表示(17~32チャネル用)*/
	DISPLAY_RIGHT = 2,	/*右側にプログラム名等を表示(1~16チャネル用)*/
	DISPLAY_NONE = 3	/*表示しない(32チャネル表示モード)*/
};

/*ドライバの初期化*/
void drv113z_init(void);

/*ドライバ動作モードの変更*/
void drv113z_display_type(enum drv113z_display_type_e);

/*各パラメータ上の矢印の表示/非表示*/
void drv113z_set_arrow(enum drv113z_arrow_e, int);

/*音源モード表示*/
void drv113z_set_mode(int);

/*各パラメータ表示*/
void drv113z_set_param(enum drv113z_param_e, int);

/*プログラム名/プログラムナンバー/バンクを表示*/
void drv113z_set_inst(char*, int, int, int);

/*バーグラフの値を設定(0~15)*/
void drv113z_set_bar_unsigned(int, int);

/*バーグラフの値を設定(-5~4)*/
void drv113z_set_bar_signed(int, int);

/*グラフィック表示*/
void drv113z_set_gfx(uint8_t*);

/*パフォーマンス用表示(デモ曲で使われていたもの)*/
void drv113z_set_param_pfm(enum drv113z_param_e, int);
#endif /* DRV113Z_H_ */