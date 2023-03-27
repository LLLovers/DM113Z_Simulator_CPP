#include "DM113Z.h"

DM113Z::DM113Z()
{
	_width = 0;
	_height = 0;
	_font_handle = 0;
	_handle = 0;

	_cgram = new uint8_t[64];

	_text = new DM113Z_TXT(
		config::text::column, config::text::row,
		config::text::pixel_width, config::text::pixel_height, config::text::pixel_gap
	);

	_line = new DM113Z_ROUND(
		config::line::width, config::line::height, (char*)"LINE"
	);

	_mic = new DM113Z_ROUND(
		config::mic::width, config::mic::height, (char*)"MIC"
	);

	_ch = new DM113Z_CH(
		config::ch::pixel_width, config::ch::pixel_height, config::ch::pixel_gap
	);

	_bank_pgm = new DM113Z_BANK_PGM(
		config::bank_pgm::pixel_width, config::bank_pgm::pixel_height, config::bank_pgm::pixel_gap
	);

	_part = new DM113Z_PART(
		config::part::pixel_width, config::part::pixel_height, config::part::pixel_gap
	);

	_midi = new DM113Z_MIDI(
		config::midi::pixel_width, config::midi::pixel_height, config::midi::pixel_gap
	);

	_gfx = new DM113Z_GFX(
		config::gfx::width, config::gfx::height,
		config::gfx::pixel_width, config::gfx::pixel_height, config::gfx::pixel_gap
	);

	_vol = new DM113Z_BAR_RECT(
		config::vol::pixel_width, config::vol::pixel_height, config::vol::pixel_gap
	);

	_exp = new DM113Z_BAR_RECT(
		config::exp::pixel_width, config::exp::pixel_height, config::exp::pixel_gap
	);

	_pan = new DM113Z_PAN;

	_rev = new DM113Z_BAR_ARC;

	_cho = new DM113Z_BAR_ARC;

	_var = new DM113Z_BAR_ARC;

	_key = new DM113Z_KEY(1);

	_mode = new DM113Z_MODE(
		config::mode::gap
	);

	_aMIDI = new DM113Z_ARROW;
	_aBANK = new DM113Z_ARROW;
	_aPGM = new DM113Z_ARROW;
	_aVOL = new DM113Z_ARROW;
	_aEXP = new DM113Z_ARROW;
	_aPAN = new DM113Z_ARROW;
	_aREV = new DM113Z_ARROW;
	_aCHO = new DM113Z_ARROW;
	_aVAR = new DM113Z_ARROW;
	_aKEY = new DM113Z_ARROW;

	isReady = 0;
}

DM113Z::~DM113Z()
{
	delete _text;
	delete _line;
	delete _mic;
	delete _ch;
	delete _bank_pgm;
	delete _part;
	delete _midi;
	delete _gfx;
	delete _vol;
	delete _exp;
	delete _pan;
	delete _rev;
	delete _cho;
	delete _var;
	delete _key;
	delete _mode;

	delete _aMIDI;
	delete _aBANK;
	delete _aPGM;
	delete _aVOL;
	delete _aEXP;
	delete _aPAN;
	delete _aREV;
	delete _aCHO;
	delete _aVAR;
	delete _aKEY;

	DeleteGraph(_handle);
	DxLib_End();
	exit;
}

void DM113Z::init(void)
{
	ChangeWindowMode(1);
	SetWindowSizeChangeEnableFlag(FALSE);
	SetAlwaysRunFlag(TRUE);
	SetMainWindowText("DM113Z Simulator Programmed By C0ntRA1L/Vide");

	SetGraphMode(config::window::width, config::window::height, 32, 60);

	DxLib_Init();

	_font_handle = CreateFontToHandle("YMFont", 32, 4, DX_FONTTYPE_NORMAL);

	_text->init();
	_line->init(_font_handle, 32);
	_mic->init(_font_handle, 32);
	_ch->init(_font_handle, 32);
	_bank_pgm->init(_font_handle, 32);
	_part->init();
	_midi->init();
	_gfx->init();
	_vol->init();
	_exp->init();
	_pan->init();
	_rev->init();
	_cho->init();
	_var->init();
	_key->init();
	_mode->init();
	_aMIDI->init();
	_aBANK->init();
	_aPGM->init();
	_aVOL->init();
	_aEXP->init();
	_aPAN->init();
	_aREV->init();
	_aCHO->init();
	_aVAR->init();
	_aKEY->init();

	isReady = 1;

	memset(_cgram, 0x00, 64);
}

void DM113Z::clear(void)
{
	_text->clear();
	_line->set(0);
	_mic->set(0);
	_ch->set(0, 0);
	_ch->set(1, 0);
	_bank_pgm->set(0, 0);
	_bank_pgm->set(1, 0);
	_part->clear();
	_midi->clear();
	_gfx->clear();
	_vol->set(0x00);
	_exp->set(0x00);
	_pan->set(0x00);
	_rev->set(0x00);
	_cho->set(0x00);
	_var->set(0x00);
	_key->set(0, 0);
	_key->set(1, 0);
	_key->set(2, 0);
	_mode->set(0x00);

	_aMIDI->set(0);
	_aBANK->set(0);
	_aPGM->set(0);
	_aVOL->set(0);
	_aEXP->set(0);
	_aPAN->set(0);
	_aREV->set(0);
	_aCHO->set(0);
	_aVAR->set(0);
	_aKEY->set(0);

	memset(_cgram, 0x00, 64);
}

void DM113Z::set_char_txt(int x, int y, uint8_t c)
{
	uint8_t* font = (c < 8) ? &_cgram[8 * c] : (uint8_t*)fontdata[c];
	_text->set(x, y, font);
}

void DM113Z::set_char_part(int x, uint8_t c)
{
	uint8_t* font = (c < 8) ? &_cgram[8 * c] : (uint8_t*)fontdata[c];
	_part->set(x, font);
}

void DM113Z::set_char_midi(int x, uint8_t c)
{
	uint8_t* font = (c < 8) ? &_cgram[8 * c] : (uint8_t*)fontdata[c];
	_midi->set(x, font);
}

void DM113Z::set_gfx(uint8_t* gfx)
{
	_gfx->set(gfx);
}

void DM113Z::set_segment(seg_e seg, uint8_t state)
{
	switch (seg) {
	case SEG_MIC:
		_mic->set(state);
		break;
	case SEG_LINE:
		_line->set(state);
		break;
	case SEG_CH_LEFT:
		_ch->set(0, state);
		break;
	case SEG_CH_RIGHT:
		_ch->set(1, state);
		break;
	case SEG_BANK_PGM_LEFT:
		_bank_pgm->set(0, state);
		break;
	case SEG_BANK_PGM_RIGHT:
		_bank_pgm->set(1, state);
		break;
	case SEG_ARROW_MIDI:
		_aMIDI->set(state);
		break;
	case SEG_ARROW_BANK:
		_aBANK->set(state);
		break;
	case SEG_ARROW_PGM:
		_aPGM->set(state);
		break;
	case SEG_ARROW_VOL:
		_aVOL->set(state);
		break;
	case SEG_ARROW_EXP:
		_aEXP->set(state);
		break;
	case SEG_ARROW_PAN:
		_aPAN->set(state);
		break;
	case SEG_ARROW_REV:
		_aREV->set(state);
		break;
	case SEG_ARROW_CHO:
		_aCHO->set(state);
		break;
	case SEG_ARROW_VAR:
		_aVAR->set(state);
		break;
	case SEG_ARROW_KEY:
		_aKEY->set(state);
		break;
	}
}

void DM113Z::set_param(param_e param, uint8_t* value)
{

	switch (param) {
	case PARAM_VOL:
		_vol->set((DM113Z_BAR_RECT::segment_t*)value);
		break;
	case PARAM_EXP:
		_exp->set((DM113Z_BAR_RECT::segment_t*)value);
		break;
	case PARAM_PAN:
		_pan->set((DM113Z_PAN::segment_t*)value);
		break;
	case PARAM_REV:
		_rev->set((DM113Z_BAR_ARC::segment_t*)value);
		break;
	case PARAM_CHO:
		_cho->set((DM113Z_BAR_ARC::segment_t*)value);
		break;
	case PARAM_VAR:
		_var->set((DM113Z_BAR_ARC::segment_t*)value);
		break;
	case PARAM_KEY:
		_key->set(0, value[0]);
		_key->set(1, value[1]);
		_key->set(2, value[2]);
		break;
	case PARAM_MODE:
		_mode->set((DM113Z_MODE::mode_t*)value);
		break;
	}
}

void DM113Z::set_cgram(uint8_t idx, uint8_t* font)
{
	memcpy(&_cgram[8 * idx], font, 8);
}

int DM113Z::callback(void)
{
	int x = config::window::margin_x;
	int y = config::window::margin_y;

	int floor = 0;
	int ax = 0;
	int ay = 0;

	if (ProcessMessage() != -1) {
		SetDrawScreen(DX_SCREEN_BACK);
		DrawBox(0, 0, config::window::width, config::window::height, _COLOUR_BG, TRUE);

		_handle = _text->draw();
		DrawGraph(x, y, _handle, TRUE);
		y += _text->get_height() + config::text::margin_y;

		_handle = _ch->draw();
		DrawGraph(x, y, _handle, TRUE);
		y += _ch->get_height() + config::ch::margin_y;

		_handle = _bank_pgm->draw();
		DrawGraph(x, y + config::offset::bank_pgm::y, _handle, TRUE);

		_handle = _mic->draw();
		DrawGraph(x, y, _handle, TRUE);
		y += _mic->get_height() + config::mic::margin_y;

		_handle = _line->draw();
		DrawGraph(x, y, _handle, TRUE);
		y += _line->get_height() + config::line::margin_y;

		floor = y + _part->get_height();	/*ˆê”Ô‰º‚Ìƒ‰ƒCƒ“*/
		ay = floor - _gfx->get_height() - _aMIDI->get_height() - config::arrow::margin_y;

		_handle = _part->draw();
		DrawGraph(x, y, _handle, TRUE);
		x += _part->get_width() + config::part::margin_x;

		_handle = _midi->draw();
		y = floor - _midi->get_height();
		DrawGraph(x, y, _handle, TRUE);

		ax = x + (_midi->get_width() / 2) - (_aMIDI->get_width() / 2) - (config::midi::pixel_width / 2);
		_handle = _aMIDI->draw();
		DrawGraph(ax, ay, _handle, TRUE);
		x += _midi->get_width() + config::midi::margin_x;

		y = floor - _gfx->get_height();
		_handle = _gfx->draw();
		DrawGraph(x, y, _handle, TRUE);

		ax = x + (_gfx->get_width() / 4) - (_aBANK->get_width() / 2) - (config::gfx::pixel_width / 2);
		_handle = _aBANK->draw();
		DrawGraph(ax, ay, _handle, TRUE);

		ax = x + (_gfx->get_width() / 4 * 3) - (_aPGM->get_width() / 2) - (config::gfx::pixel_width / 2);
		_handle = _aPGM->draw();
		DrawGraph(ax, ay, _handle, TRUE);

		x += _gfx->get_width() + config::gfx::margin_x;

		_handle = _vol->draw();
		y = floor - _vol->get_height();
		DrawGraph(x, y, _handle, TRUE);
		ax = x + (_vol->get_width() / 2) - (_aVOL->get_width() / 2);
		_handle = _aVOL->draw();
		DrawGraph(ax, ay, _handle, TRUE);
		x += _vol->get_width() + config::vol::margin_x;

		_handle = _exp->draw();
		y = floor - _exp->get_height();
		DrawGraph(x, y, _handle, TRUE);
		ax = x + (_exp->get_width() / 2) - (_aEXP->get_width() / 2);
		_handle = _aEXP->draw();
		DrawGraph(ax, ay, _handle, TRUE);
		x += _exp->get_width() + config::exp::margin_x;

		_handle = _pan->draw();
		y = floor - _pan->get_height();
		DrawGraph(x, y + config::offset::pan::y, _handle, TRUE);
		ax = x + (_pan->get_width() / 2) - (_aPAN->get_width() / 2);
		_handle = _aPAN->draw();
		DrawGraph(ax, ay, _handle, TRUE);
		x += _pan->get_width() + config::pan::margin_x;

		_handle = _rev->draw();
		y = floor - _rev->get_height();
		DrawGraph(x, y + config::offset::arc::y, _handle, TRUE);
		ax = x + (_rev->get_width() / 2) - (_aREV->get_width() / 2);
		_handle = _aREV->draw();
		DrawGraph(ax, ay, _handle, TRUE);
		x += _rev->get_width() + config::rev::margin_x;

		_handle = _cho->draw();
		DrawGraph(x, y + config::offset::arc::y, _handle, TRUE);
		ax = x + (_cho->get_width() / 2) - (_aCHO->get_width() / 2);
		_handle = _aCHO->draw();
		DrawGraph(ax, ay, _handle, TRUE);
		x += _cho->get_width() + config::cho::margin_x;

		_handle = _var->draw();
		DrawGraph(x, y + config::offset::arc::y, _handle, TRUE);
		ax = x + (_var->get_width() / 2) - (_aVAR->get_width() / 2);
		_handle = _aVAR->draw();
		DrawGraph(ax, ay, _handle, TRUE);
		x += _var->get_width() + config::var::margin_x;

		_handle = _key->draw();
		y = floor - _key->get_height();
		DrawGraph(x, y, _handle, TRUE);
		ax = x + (_key->get_width() / 2) - (_aKEY->get_width() / 2);
		_handle = _aKEY->draw();
		DrawGraph(ax, ay, _handle, TRUE);
		x += _key->get_width() + config::key::margin_x;

		_handle = _mode->draw();
		y = floor - _mode->get_height();
		DrawGraph(x, y, _handle, TRUE);
		x += _mode->get_width() + config::mode::margin_x;

		ScreenFlip();
		return 1;
	}
	else {
		return 0;
	}

}

void DM113Z::_set_resolution(void)
{
	int x = config::window::margin_x;
	int y = config::window::margin_x;

	y += _text->get_height() + config::text::margin_y;
	y += _ch->get_height() + config::ch::margin_y;
	y += _mic->get_height() + config::mic::margin_y;
	y += _line->get_height() + config::line::margin_y;
	y += _part->get_height() + config::window::margin_y;	/*‚±‚±‚Åy‚ªŒˆ’è*/

	x += _part->get_width() + config::part::margin_x;
	x += _midi->get_width() + config::midi::margin_x;
	x += _gfx->get_width() + config::gfx::margin_x;
	x += _vol->get_width() + config::vol::margin_x;
	x += _exp->get_width() + config::exp::margin_x;
	x += _pan->get_width() + config::pan::margin_x;
	x += _rev->get_width() + config::rev::margin_x;
	x += _cho->get_width() + config::cho::margin_x;
	x += _var->get_width() + config::var::margin_x;
	x += _key->get_width() + config::key::margin_x;
	x += _mode->get_width() + config::mode::margin_x + config::window::margin_x;	/*‚±‚±‚Åx‚ªŒˆ’è*/

	_width = x;
	_height = y;
}
