#ifndef UTFT_Buttons_h
#define UTFT_Buttons_h

#include "Arduino.h"

#include "Kd_TFT.h"
#include "XPT2046_Calibrated.h"

#define MAX_BUTTONS	20	// Maximum number of buttons available at one time

// Define presets for button status
#define BUTTON_DISABLED			0x0001
#define BUTTON_SYMBOL			0x0002
#define BUTTON_SYMBOL_REP_3X	0x0004
#define BUTTON_BITMAP			0x0008	
#define BUTTON_NO_BORDER		0x0010
#define BUTTON_UNUSED			0x8000

typedef struct
{
  uint16_t			 pos_x, pos_y, width, height;
  uint16_t			 flags;
  String				 label;
  bitmapdatatype data;
} button_type;

class KdTFT_Buttons
{
	public:
		KdTFT_Buttons(KdTFT *ptrUTFT, XPT2046_Calibrated *ptrURTouch);

		int		addButton(uint16_t x, uint16_t y, uint16_t width, uint16_t height,  String label, uint16_t flags=0);
		int		addButton(uint16_t x, uint16_t y, uint16_t width, uint16_t height, bitmapdatatype data, uint16_t flags=0);
		void	drawButtons();
		void	drawButton(int buttonID);
		void	enableButton(int buttonID, boolean redraw=false);
		void	disableButton(int buttonID, boolean redraw=false);
		void	relabelButton(int buttonID, String label, boolean redraw=false);
		boolean	buttonEnabled(int buttonID);
		void	deleteButton(int buttonID);
		void	deleteAllButtons();
		int		checkButtons();
		void	setTextFont(uint8_t* font);
		void	setSymbolFont(uint8_t* font);
		void	setButtonColors(word atxt, word iatxt, word brd, word brdhi, word back);
    bool  between(uint16_t x, uint16_t a, uint16_t b);

	protected:
		KdTFT		*_KdTFT;
		XPT2046_Calibrated *KdTouch;
		button_type	buttons[MAX_BUTTONS];
		word		_color_text, _color_text_inactive, _color_background, _color_border, _color_hilite;
		uint8_t	*_font_text, *_font_symbol;
};

#endif
