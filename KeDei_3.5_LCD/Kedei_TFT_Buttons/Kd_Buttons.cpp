#include "Kd_Buttons.h"

extern int max_x;
extern int max_y;

KdTFT_Buttons::KdTFT_Buttons(KdTFT *ptrUTFT, XPT2046_Calibrated *ptrURTouch)
{
	_KdTFT = ptrUTFT;
	KdTouch = ptrURTouch;
	_color_text				= VGA_WHITE;
	_color_text_inactive	= VGA_GRAY;
	_color_background		= VGA_BLUE;
	_color_border			= VGA_WHITE;
	_color_hilite			= VGA_RED;
	_font_text				= NULL;
	_font_symbol			= NULL;
	deleteAllButtons();
}

int KdTFT_Buttons::addButton(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String label, uint16_t flags)
{
	int btcnt = 0;
  
	while (((buttons[btcnt].flags & BUTTON_UNUSED) == 0) and (btcnt<MAX_BUTTONS))
		btcnt++;
  
	if (btcnt == MAX_BUTTONS)
		return -1;
	else
	{
		buttons[btcnt].pos_x  = x;
		buttons[btcnt].pos_y  = y;
		buttons[btcnt].width  = width;
		buttons[btcnt].height = height;
		buttons[btcnt].flags  = flags;
		buttons[btcnt].label  = label;
		buttons[btcnt].data   = NULL;
		return btcnt;
	}
}

int KdTFT_Buttons::addButton(uint16_t x, uint16_t y, uint16_t width, uint16_t height, bitmapdatatype data, uint16_t flags)
{
	int btcnt = 0;
  
	while (((buttons[btcnt].flags & BUTTON_UNUSED) == 0) and (btcnt < MAX_BUTTONS))
		btcnt++;
  
	if (btcnt == MAX_BUTTONS)
		return -1;
	else
	{
		buttons[btcnt].pos_x  = x;
		buttons[btcnt].pos_y  = y;
		buttons[btcnt].width  = width;
		buttons[btcnt].height = height;
		buttons[btcnt].flags  = flags | BUTTON_BITMAP;
		buttons[btcnt].label  = "";
		buttons[btcnt].data   = data;
		return btcnt;
	}
}


void KdTFT_Buttons::drawButtons()
{
	for (int i=0;i<MAX_BUTTONS;i++)
	{
		if ((buttons[i].flags & BUTTON_UNUSED) == 0)
			drawButton(i);
	}
}

void KdTFT_Buttons::drawButton(int buttonID)
{
	int		text_x, text_y;
	fontdatatype *_font_current = _KdTFT->getFont();
	word	_current_color = _KdTFT->getColor();
	word	_current_back  = _KdTFT->getBackColor();

	if (buttons[buttonID].flags & BUTTON_BITMAP)
	{
		_KdTFT->drawBitmap(buttons[buttonID].pos_x, buttons[buttonID].pos_y, buttons[buttonID].width, buttons[buttonID].height, buttons[buttonID].data);
		if (!(buttons[buttonID].flags & BUTTON_NO_BORDER))
		{
			if ((buttons[buttonID].flags & BUTTON_DISABLED))
				_KdTFT->setColor(_color_text_inactive);
			else
				_KdTFT->setColor(_color_border);
			_KdTFT->drawRect(buttons[buttonID].pos_x, buttons[buttonID].pos_y, buttons[buttonID].pos_x+buttons[buttonID].width, buttons[buttonID].pos_y+buttons[buttonID].height);
		}
	}
	else
	{
		_KdTFT->setColor(_color_background);
		_KdTFT->fillRoundRect(buttons[buttonID].pos_x, buttons[buttonID].pos_y, buttons[buttonID].pos_x+buttons[buttonID].width, buttons[buttonID].pos_y+buttons[buttonID].height);
		_KdTFT->setColor(_color_border);
		_KdTFT->drawRoundRect(buttons[buttonID].pos_x, buttons[buttonID].pos_y, buttons[buttonID].pos_x+buttons[buttonID].width, buttons[buttonID].pos_y+buttons[buttonID].height);
		if (buttons[buttonID].flags & BUTTON_DISABLED)
			_KdTFT->setColor(_color_text_inactive);
		else
			_KdTFT->setColor(_color_text);
		if (buttons[buttonID].flags & BUTTON_SYMBOL)
		{
			_KdTFT->setFont(_font_symbol);
			text_x = (buttons[buttonID].width/2) - (_KdTFT->getFontXsize()/2) + buttons[buttonID].pos_x;
			text_y = (buttons[buttonID].height/2) - (_KdTFT->getFontYsize()/2) + buttons[buttonID].pos_y;
		}
		else
		{
			_KdTFT->setFont(_font_text);
			text_x = ((buttons[buttonID].width/2) - ((buttons[buttonID].label.length() * _KdTFT->getFontXsize())/2)) + buttons[buttonID].pos_x;
			text_y = (buttons[buttonID].height/2) - (_KdTFT->getFontYsize()/2) + buttons[buttonID].pos_y;
		}
		_KdTFT->setBackColor(_color_background);
		_KdTFT->print(buttons[buttonID].label, text_x, text_y);
		if ((buttons[buttonID].flags & BUTTON_SYMBOL) and (buttons[buttonID].flags & BUTTON_SYMBOL_REP_3X))
		{
			_KdTFT->print(buttons[buttonID].label, text_x-_KdTFT->getFontXsize(), text_y);
			_KdTFT->print(buttons[buttonID].label, text_x+_KdTFT->getFontXsize(), text_y);
		}
	}
	_KdTFT->setFont(_font_current);
	_KdTFT->setColor(_current_color);
	_KdTFT->setBackColor(_current_back);
}

void KdTFT_Buttons::enableButton(int buttonID, boolean redraw)
{
	if (!(buttons[buttonID].flags & BUTTON_UNUSED))
	{
		buttons[buttonID].flags = buttons[buttonID].flags & ~BUTTON_DISABLED;
		if (redraw)
			drawButton(buttonID);
	}
}

void KdTFT_Buttons::disableButton(int buttonID, boolean redraw)
{
	if (!(buttons[buttonID].flags & BUTTON_UNUSED))
	{
		buttons[buttonID].flags = buttons[buttonID].flags | BUTTON_DISABLED;
		if (redraw)
			drawButton(buttonID);
	}
}

void KdTFT_Buttons::relabelButton(int buttonID, String label, boolean redraw)
{
	if (!(buttons[buttonID].flags & BUTTON_UNUSED))
	{
		buttons[buttonID].label = label;
		if (redraw)
			drawButton(buttonID);
	}
}

boolean KdTFT_Buttons::buttonEnabled(int buttonID)
{
	return !(buttons[buttonID].flags & BUTTON_DISABLED);
}
    
void KdTFT_Buttons::deleteButton(int buttonID)
{
	if (!(buttons[buttonID].flags & BUTTON_UNUSED))
		buttons[buttonID].flags = BUTTON_UNUSED;
}

void KdTFT_Buttons::deleteAllButtons()
{
	for (int i = 0; i < MAX_BUTTONS; i++)
	{
		buttons[i].pos_x=0;
		buttons[i].pos_y=0;
		buttons[i].width=0;
		buttons[i].height=0;
		buttons[i].flags=BUTTON_UNUSED;
		buttons[i].label="";
	}
}

bool KdTFT_Buttons::between(uint16_t x, uint16_t a, uint16_t b)
{
  if(x >= a && x <= b)
    return true;
  else
    return false;
}

int KdTFT_Buttons::checkButtons()
{
  if (KdTouch->tirqTouched() && KdTouch->touched()) {
    int		result = -1;
    TS_Point p = KdTouch->getPoint();
  
    uint16_t	touch_x = p.x;
    uint16_t	touch_y = p.y;
    word	_current_color = _KdTFT->getColor();

    for (int i = 0; i < MAX_BUTTONS; i++) {
       if (((buttons[i].flags & BUTTON_UNUSED) == 0) and ((buttons[i].flags & BUTTON_DISABLED) == 0) and (result == -1)) {
        // if ((touch_x >= buttons[i].pos_x) and (touch_x <= (buttons[i].pos_x + buttons[i].width)) and (touch_y >= buttons[i].pos_y) and (touch_y <= (buttons[i].pos_y + buttons[i].height))) {
        uint16_t	b_x  = buttons[i].pos_x;
        uint16_t	b_y  = buttons[i].pos_y;
        uint16_t	b_xw = buttons[i].pos_x + buttons[i].width;
        uint16_t	b_yh = buttons[i].pos_y + buttons[i].height;
        // Serial.printf_P("num = %d, X = %d, Y = %d, L = %d, T = %d, W = %d, H = %d\r\n", i, touch_x, touch_y, b_x, b_y, b_xw, b_xh);
        Serial.printf_P("num = %d, X = %d, Y = %d, W = %d, H = %d\r\n", i, touch_x, touch_y,
          between(touch_x, b_x, b_xw), between(touch_y, b_y, b_yh));

        if(between(touch_x, buttons[i].pos_x, buttons[i].pos_x + buttons[i].width) &&
           between(touch_y, buttons[i].pos_y, buttons[i].pos_y + buttons[i].height)) {
          result = i;
        }
      }
    }

    if (result != -1)
    {
      if (!(buttons[result].flags & BUTTON_NO_BORDER))
      {
        _KdTFT->setColor(_color_hilite);
        if (buttons[result].flags & BUTTON_BITMAP)
          _KdTFT->drawRect(buttons[result].pos_x, buttons[result].pos_y, buttons[result].pos_x+buttons[result].width, buttons[result].pos_y+buttons[result].height);
        else
          _KdTFT->drawRoundRect(buttons[result].pos_x, buttons[result].pos_y, buttons[result].pos_x+buttons[result].width, buttons[result].pos_y+buttons[result].height);
      }
    }

    while (KdTouch->touched()) {};

    if (result != -1)
    {
      if (!(buttons[result].flags & BUTTON_NO_BORDER))
      {
        _KdTFT->setColor(_color_border);
        if (buttons[result].flags & BUTTON_BITMAP)
          _KdTFT->drawRect(buttons[result].pos_x, buttons[result].pos_y, buttons[result].pos_x+buttons[result].width, buttons[result].pos_y+buttons[result].height);
        else
          _KdTFT->drawRoundRect(buttons[result].pos_x, buttons[result].pos_y, buttons[result].pos_x+buttons[result].width, buttons[result].pos_y+buttons[result].height);
      }
    }
    _KdTFT->setColor(_current_color);
    return result;
  }
	else
		return -1;
}

void KdTFT_Buttons::setTextFont(uint8_t* font)
{
	_font_text = font;
}

void KdTFT_Buttons::setSymbolFont(uint8_t* font)
{
	_font_symbol = font;
}

void KdTFT_Buttons::setButtonColors(word atxt, word iatxt, word brd, word brdhi, word back)
{
	_color_text				= atxt;
	_color_text_inactive	= iatxt;
	_color_background		= back;
	_color_border			= brd;
	_color_hilite			= brdhi;
}
