/*
 * DRV113Z.c
 *
 * Created: 2023/03/20 16:44:06
 *  Author: Owner
 */ 

#include "DRV113Z.h"

/*現在の表示モード*/
static enum drv113z_display_type_e type = DISPLAY_RIGHT;

/*ボリューム/エクスプレッション表示用に保持*/
static uint8_t volume = 100,expression = 127;

/*レベル表示用に保持*/
static int8_t level[34];

/*記号表示領域のCGRAM保持*/
static uint8_t symbol_char[64];

/*内部関数*/

/*バーグラフ用文字コード算出*/
static inline uint8_t _graph_code(int a,int b){return (a == 0 && b == 0)? ' ' : (0x80 + a * 9 + b - 1);}

/*パラメータ表示関連*/
static void _set_part(int);
static void _set_midi(int);
static void _set_vol(int);
static void _set_exp(int);
static void _set_pan(int);
static void _set_rev(int);
static void _set_cho(int);
static void _set_var(int);
static void _set_key(int);

static void _set_pan_pfm(int);
static void _set_rev_pfm(int);
static void _set_cho_pfm(int);
static void _set_var_pfm(int);

/*ドライバの初期化*/
void drv113z_init(void){
	int i,j;
	/*LCDの初期化*/
	lcd_init();
	memset(symbol_char,0x00,sizeof(symbol_char) / sizeof(symbol_char[0]));
	drv113z_display_type(DISPLAY_RIGHT);
}

/*ドライバ動作モードの変更*/
void drv113z_display_type(enum drv113z_display_type_e _type){
	int i,j;
	uint8_t* offset = &symbol_char[24];
	
	type = _type;
	
	/*ユーザー定義文字を並べる*/
	
	for(i = 0;i < 2;i++){
		lcd_locate(2 + i,0);
		for(j = 0;j < 4;j++){
			lcd_putc(i * 4 + j);
		}
	}
	
	lcd_setcg(0,8,symbol_char);
	
	if(type & 0x01){
		offset[4] &= ~(uint8_t)0x01;
		offset[0] &= ~(uint8_t)0x01;
	}
	else {

	}
	
	if(type & 0x02){
		offset[3] |= (uint8_t)0x01;
		offset[5] |= (uint8_t)0x01;
	} else{
		offset[3] &= ~(uint8_t)0x01;
		offset[5] &= ~(uint8_t)0x01;
	}
	
	lcd_setcg(3,1,offset);
}


/*矢印表示用テーブル*/
static const uint8_t arrow_index[] = {2,1,0,3,4,5,6,7,6,7};


/*各パラメータ上の矢印の表示/非表示*/
void drv113z_set_arrow(enum drv113z_arrow_e arrow,int value){
	uint8_t* offset = &symbol_char[24];
	
	if(value){
		offset[arrow_index[(int)arrow]] |= (uint8_t)(arrow >= ARROW_VAR)? 0x01 : 0x02;
	} else {
		offset[arrow_index[(int)arrow]] &= ~(uint8_t)((arrow >= ARROW_VAR)? 0x01 : 0x02);
	}
	
	lcd_setcg(3,1,offset);
}

/*音源モード表示用テーブル*/
static const uint8_t mode_index[] = {4,5};
	
/*音源モード表示*/
void drv113z_set_mode(int mode){
	uint8_t* offset = &symbol_char[24];
	
offset[mode_index[(uint8_t)mode & 0x01]] &= ~(uint8_t)(0x0C);
offset[mode_index[(uint8_t)mode & 0x01]] |= (uint8_t)((uint8_t)mode >> 1) ? 0x08 : 0x04;
offset[mode_index[((uint8_t)mode & 0x01) ^ 0x01]] &= ~(uint8_t)(0x0C);

lcd_setcg(3, 1, offset);
}

/*各パラメータ表示*/
void drv113z_set_param(enum drv113z_param_e param, int value) {
	void (*set_func)(int);

	switch (param) {
	case PARAM_PART:
		set_func = _set_part;
		break;
	case PARAM_MIDI:
		set_func = _set_midi;
		break;
	case PARAM_VOL:
		set_func = _set_vol;
		break;
	case PARAM_EXP:
		set_func = _set_exp;
		break;
	case PARAM_PAN:
		set_func = _set_pan;
		break;
	case PARAM_REV:
		set_func = _set_rev;
		break;
	case PARAM_CHO:
		set_func = _set_cho;
		break;
	case PARAM_VAR:
		set_func = _set_var;
		break;
	case PARAM_KEY:
		set_func = _set_key;
		break;
	default:
		return;
	}

	set_func(value);
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

/*プログラム名/プログラムナンバー/バンクを表示*/
void drv113z_set_inst(char* name, int length, int pgm, int bank) {
	lcd_locate(0, 9);
	for (int i = 0; i < MIN(length - 1, 8); i++) {
		lcd_putc(name[i]);
	}
	lcd_locate(1, 9);
	lcd_putc(0x11);
	lcd_putc('0' + bank / 100);
	lcd_putc('0' + ((bank >= 100) ? bank - (bank / 100) * 100 : bank) / 10);
	lcd_putc('0' + bank % 10);
	lcd_putc(0x10);
	lcd_putc('0' + pgm / 100);
	lcd_putc('0' + ((pgm >= 100) ? pgm - (pgm / 100) * 100 : pgm) / 10);
	lcd_putc('0' + pgm % 10);
}


/*バーグラフの値を設定(0~15)*/
void drv113z_set_bar_unsigned(int ch, int value) {
	int idx = ch / 2;
	level[ch] = value;

	lcd_locate(1, idx);
	lcd_putc(_graph_code(MIN(level[idx * 2], 8), MIN(level[idx * 2 + 1], 8)));

	lcd_locate(0, idx);
	lcd_putc(_graph_code(MAX(level[idx * 2] - 8, 0), MAX(level[idx * 2 + 1] - 8, 0)));
}

/*バーグラフの値を設定(-5~4)*/
void drv113z_set_bar_signed(int ch, int value) {

}

/*グラフィック表示*/
void drv113z_set_gfx(uint8_t* gfx) {
	uint8_t cnt_chr = 0;
	uint8_t cnt_line = 0;
	uint8_t cnt_bit = 4;
	for (int i = 0; i < 256; i++) {

		if (gfx[i / 8] & (1 << (7 - (i % 8)))){
			symbol_char[8 * (cnt_chr + 4 * (cnt_line / 8)) + (cnt_line % 8)] |= (1 << cnt_bit);
		}
		else {
			symbol_char[8 * (cnt_chr + 4 * (cnt_line / 8)) + (cnt_line % 8)] &= ~(1 << cnt_bit);
		}

		if (cnt_bit == 0 || cnt_chr == 3) {
			cnt_chr++;
			cnt_bit = 4;
			if (cnt_chr == 4) {
				cnt_chr = 0;
				cnt_line++;
			}
		}
		else {
			cnt_bit--;
		}
	}
	lcd_setcg(0,8,symbol_char);
}


/*パフォーマンス用表示(デモ曲で使われていたもの)*/
void drv113z_set_param_pfm(enum drv113z_param_e param,int value){
	void (*set_func)(int);
	
	switch(param){
		case PARAM_PAN:
		set_func = _set_pan_pfm;
		break;
		case PARAM_REV:
		set_func = _set_rev_pfm;
		break;
		case PARAM_CHO:
		set_func = _set_cho_pfm;
		break;
		case PARAM_VAR:
		set_func = _set_var_pfm;
		break;
		default:
		return;
	}
	
	set_func(value);
}

static void _set_part(int value){
	lcd_locate(0,17);
	lcd_putc('0' + (value / 10));
	lcd_putc('0' + (value % 10));
}

static void _set_midi(int value){
	lcd_locate(1,17);
	lcd_putc((value < 16)? 'A' : 'B');
	lcd_putc('0' + (((value > 15)? value - 16 : value) / 10));
	lcd_putc('0' + (((value > 15)? value - 16 : value) % 10));
}

static void _set_vol(int value){
	volume = value;
	lcd_locate(0,19);
	lcd_putc(_graph_code(volume,expression));
}

static void _set_exp(int value){
	expression = value;
	lcd_locate(0,19);
	lcd_putc(_graph_code(volume,expression));
}

static void _set_pan(int value){
	int i;
	uint8_t* offset = &symbol_char[56];
	value *= -1;
	value += 3;
	for(i = 0;i < 8;i++){
		offset[i] = (i == value + 1)? offset[i] | 0x01 : offset[i] & ~(0x01);
	}
	
	lcd_setcg(7,1,offset);
}

static void _set_rev(int value){
	uint8_t* offset = &symbol_char[56];
	
	offset[0] = (value > 7)? offset[0] | 0x08 : offset[0] & ~(0x08);
	offset[1] = (value > 6)? offset[1] | 0x08 : offset[1] & ~(0x08);
	offset[2] = (value > 5)? offset[2] | 0x08 : offset[2] & ~(0x08);
	offset[3] = (value > 4)? offset[3] | 0x08 : offset[3] & ~(0x08);
	offset[4] = (value > 3)? offset[4] | 0x08 : offset[4] & ~(0x08);
	offset[5] = (value > 2)? offset[5] | 0x08 : offset[5] & ~(0x08);
	offset[6] = (value > 1)? offset[6] | 0x08 : offset[6] & ~(0x08);
	offset[7] = (value > 0)? offset[7] | 0x08 : offset[7] & ~(0x08);
	
	lcd_setcg(7,1,offset);
}

static void _set_cho(int value){
	uint8_t* offset = &symbol_char[56];
	
	offset[0] = (value > 7)? offset[0] | 0x04 : offset[0] & ~(0x04);
	offset[1] = (value > 6)? offset[1] | 0x04 : offset[1] & ~(0x04);
	offset[2] = (value > 5)? offset[2] | 0x04 : offset[2] & ~(0x04);
	offset[3] = (value > 4)? offset[3] | 0x04 : offset[3] & ~(0x04);
	offset[4] = (value > 3)? offset[4] | 0x04 : offset[4] & ~(0x04);
	offset[5] = (value > 2)? offset[5] | 0x04 : offset[5] & ~(0x04);
	offset[6] = (value > 1)? offset[6] | 0x04 : offset[6] & ~(0x04);
	offset[7] = (value > 0)? offset[7] | 0x04 : offset[7] & ~(0x04);
	
	lcd_setcg(7,1,offset);	
}

static void _set_var(int value){
	uint8_t* offset = &symbol_char[56];
	
	offset[0] = (value > 7)? offset[0] | 0x02: offset[0] & ~(0x02);
	offset[1] = (value > 6)? offset[1] | 0x02 : offset[1] & ~(0x02);
	offset[2] = (value > 5)? offset[2] | 0x02 : offset[2] & ~(0x02);
	offset[3] = (value > 4)? offset[3] | 0x02 : offset[3] & ~(0x02);
	offset[4] = (value > 3)? offset[4] | 0x02 : offset[4] & ~(0x02);
	offset[5] = (value > 2)? offset[5] | 0x02 : offset[5] & ~(0x02);
	offset[6] = (value > 1)? offset[6] | 0x02 : offset[6] & ~(0x02);
	offset[7] = (value > 0)? offset[7] | 0x02 : offset[7] & ~(0x02);
	
	lcd_setcg(7,1,offset);	
}

static uint8_t seg_table[10][7] = {
	/* 0 */ {0,0,0,0,0,0,1},
	/* 1 */ {1,0,0,1,1,1,1},
	/* 2 */ {0,0,1,0,0,1,0},
	/* 3 */ {0,0,0,0,1,1,0},
	/* 4 */ {1,0,0,1,1,0,0},
	/* 5 */ {0,1,0,0,1,0,0},
	/* 6 */ {0,1,0,0,0,0,0},
	/* 7 */ {0,0,0,1,1,1,1},
	/* 8 */ {0,0,0,0,0,0,0},
	/* 9 */ {0,0,0,0,1,0,0},
};

static void _set_key(int value){
	uint8_t* offset0 = &symbol_char[24];
	uint8_t* offset1 = &symbol_char[56];
	
	int num = (value > 0)? value : -1 * value;
	
	uint8_t d0 = num % 10;
	uint8_t d1 = num / 10;
	
	uint8_t* seg;
	
	seg = seg_table[d0];
	
	offset0[0] &= ~(0x0C);
	
	if(value >=  0){
		/*+*/
		offset0[0] |= 0x0C;
	} else {
		/*-*/
		offset0[0] |= 0x04;
	}
	
	/*10の位*/
	if(d1 == 0){
		offset0[1] &= ~(0x04);
		offset0[7] &= ~(0x08);
		offset1[0] &= ~(0x01);
		offset0[1] &= ~(0x08);
	} else if(d1 == 1){
		offset0[1] |= (0x04);
		offset0[7] |= (0x08);
		offset1[0] &= ~(0x01);
		offset0[1] &= ~(0x08);		
	} else if(d1 == 2){
		offset0[1] |= (0x04);
		offset0[7] &= ~(0x08);
		offset1[0] &= ~(0x01);
		offset0[1] |= (0x08);		
	} else {
		/*表示不可*/
	}

	offset0[6] = (seg[0])? 
		offset0[6] & ~(0x04) : offset0[6] | 0x04;
	offset0[6] = (seg[1])? 
		offset0[6] & ~(0x08) : offset0[6] | 0x08;
	offset0[3] = (seg[2])? 
		offset0[3] & ~(0x08) : offset0[3] | 0x08;
	offset0[2] = (seg[3])? 
		offset0[2] & ~(0x08) : offset0[2] | 0x08;
	offset0[2] = (seg[4])? 
		offset0[2] & ~(0x04) : offset0[2] | 0x04;
	offset0[7] = (seg[5])? 
		offset0[7] & ~(0x04) : offset0[7] | 0x04;
	offset0[3] = (seg[6])? 
		offset0[3] & ~(0x04) : offset0[3] | 0x04;
	
	lcd_setcg(3,1,offset0);
	lcd_setcg(7,1,offset1);
}

static void _set_pan_pfm(int value){
	
}

static void _set_rev_pfm(int value){
	uint8_t* offset = &symbol_char[56];
	
	offset[0] = (value != 7)? offset[0] | 0x08 : offset[0] & ~(0x08);
	offset[1] = (value != 6)? offset[1] | 0x08 : offset[1] & ~(0x08);
	offset[2] = (value != 5)? offset[2] | 0x08 : offset[2] & ~(0x08);
	offset[3] = (value != 4)? offset[3] | 0x08 : offset[3] & ~(0x08);
	offset[4] = (value != 3)? offset[4] | 0x08 : offset[4] & ~(0x08);
	offset[5] = (value != 2)? offset[5] | 0x08 : offset[5] & ~(0x08);
	offset[6] = (value != 1)? offset[6] | 0x08 : offset[6] & ~(0x08);
	offset[7] = (value != 0)? offset[7] | 0x08 : offset[7] & ~(0x08);
	
	lcd_setcg(7,1,offset);
}

static void _set_cho_pfm(int value){
	uint8_t* offset = &symbol_char[56];
	
	offset[0] = (value != 7)? offset[0] | 0x04 : offset[0] & ~(0x04);
	offset[1] = (value != 6)? offset[1] | 0x04 : offset[1] & ~(0x04);
	offset[2] = (value != 5)? offset[2] | 0x04 : offset[2] & ~(0x04);
	offset[3] = (value != 4)? offset[3] | 0x04 : offset[3] & ~(0x04);
	offset[4] = (value != 3)? offset[4] | 0x04 : offset[4] & ~(0x04);
	offset[5] = (value != 2)? offset[5] | 0x04 : offset[5] & ~(0x04);
	offset[6] = (value != 1)? offset[6] | 0x04 : offset[6] & ~(0x04);
	offset[7] = (value != 0)? offset[7] | 0x04 : offset[7] & ~(0x04);
	
	lcd_setcg(7,1,offset);
}

static void _set_var_pfm(int value){
	uint8_t* offset = &symbol_char[56];
	
	offset[0] = (value != 7)? offset[0] | 0x02: offset[0] & ~(0x02);
	offset[1] = (value != 6)? offset[1] | 0x02 : offset[1] & ~(0x02);
	offset[2] = (value != 5)? offset[2] | 0x02 : offset[2] & ~(0x02);
	offset[3] = (value != 4)? offset[3] | 0x02 : offset[3] & ~(0x02);
	offset[4] = (value != 3)? offset[4] | 0x02 : offset[4] & ~(0x02);
	offset[5] = (value != 2)? offset[5] | 0x02 : offset[5] & ~(0x02);
	offset[6] = (value != 1)? offset[6] | 0x02 : offset[6] & ~(0x02);
	offset[7] = (value != 0)? offset[7] | 0x02 : offset[7] & ~(0x02);
	
	lcd_setcg(7,1,offset);
}