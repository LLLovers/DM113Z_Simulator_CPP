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
	DISPLAY_LEFT = 1,	/*�����Ƀv���O����������\��(17~32�`���l���p)*/
	DISPLAY_RIGHT = 2,	/*�E���Ƀv���O����������\��(1~16�`���l���p)*/
	DISPLAY_NONE = 3	/*�\�����Ȃ�(32�`���l���\�����[�h)*/
};

/*�h���C�o�̏�����*/
void drv113z_init(void);

/*�h���C�o���샂�[�h�̕ύX*/
void drv113z_display_type(enum drv113z_display_type_e);

/*�e�p�����[�^��̖��̕\��/��\��*/
void drv113z_set_arrow(enum drv113z_arrow_e, int);

/*�������[�h�\��*/
void drv113z_set_mode(int);

/*�e�p�����[�^�\��*/
void drv113z_set_param(enum drv113z_param_e, int);

/*�v���O������/�v���O�����i���o�[/�o���N��\��*/
void drv113z_set_inst(char*, int, int, int);

/*�o�[�O���t�̒l��ݒ�(0~15)*/
void drv113z_set_bar_unsigned(int, int);

/*�o�[�O���t�̒l��ݒ�(-5~4)*/
void drv113z_set_bar_signed(int, int);

/*�O���t�B�b�N�\��*/
void drv113z_set_gfx(uint8_t*);

/*�p�t�H�[�}���X�p�\��(�f���ȂŎg���Ă�������)*/
void drv113z_set_param_pfm(enum drv113z_param_e, int);
#endif /* DRV113Z_H_ */