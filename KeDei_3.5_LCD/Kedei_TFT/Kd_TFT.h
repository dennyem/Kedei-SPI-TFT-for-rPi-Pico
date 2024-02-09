#ifndef KdTFT_h
#define KdTFT_h

#include <arduino.h>
#include <Adafruit_SPIDevice.h>
#include "DefaultFonts.c"
#include "defines.h"

#define bitmapdatatype unsigned short*

#define swap3(type, i, j) {type t = i; i = j; j = t;}
#define swap2(a, b) { int16_t t = a; a = b; b = t; }

#define fontbyte(x) cfont.font[x]  

#define LCD_WIDTH  479    // 320 x 480 minus 1
#define LCD_HEIGHT 319

#define LEFT 0
#define RIGHT 9999
#define CENTER 9998

#define PORTRAIT 0
#define LANDSCAPE 1

//*********************************
// COLORS
//*********************************
// VGA color palette
#define VGA_BLACK		0x0000
#define VGA_WHITE		0xFFFF
#define VGA_RED			0xF800
#define VGA_GREEN		0x0400
#define VGA_BLUE		0x001F
#define VGA_SILVER	0xC618
#define VGA_GRAY		0x8410
#define VGA_MAROON	0x8000
#define VGA_YELLOW	0xFFE0
#define VGA_OLIVE		0x8400
#define VGA_LIME		0x07E0
#define VGA_AQUA		0x07FF
#define VGA_TEAL		0x0410
#define VGA_NAVY		0x0010
#define VGA_FUCHSIA	0xF81F
#define VGA_PURPLE	0x8010
#define VGA_TRANSPARENT	0xFFFFFFFF

struct _current_font
{
	fontdatatype* font;
	uint8_t x_size;
	uint8_t y_size;
	uint8_t offset;
	uint8_t numchars;
};

class KdTFT
{
  private:
    Adafruit_SPIDevice *lcd_dev;

    int  spi_TxRxLCD(uint8_t *data, int len);
	  void lcd_rst();
    int  disp_x_size = LCD_WIDTH;
    int  disp_y_size = LCD_HEIGHT;
		byte fch, fcl, bch, bcl;
		byte orient;
		bool _transparent;
		_current_font	cfont;

	public:
    int8_t T_CS;
    int8_t L_CS;
  
		KdTFT(int8_t cspin, int8_t T_cspin);
    void  begin(Adafruit_SPIDevice *lcd);
    void  LCD_Reset(void);
		void	InitLCD(byte orientation=LANDSCAPE);
		void	drawPixel(int x, int y);
		void	drawLine(int x1, int y1, int x2, int y2);
		void	fillScr(byte r, byte g, byte b);
		void	fillScr(word color);
		void	drawRect(int x1, int y1, int x2, int y2);
		void	drawRoundRect(int x1, int y1, int x2, int y2);
		void	fillRect(int x1, int y1, int x2, int y2);
		void	fillRoundRect(int x1, int y1, int x2, int y2);
		void	drawCircle(int x, int y, int radius);
		void	fillCircle(int x, int y, int radius);
		void	setColor(byte r, byte g, byte b);
		void	setColor(word color);
		word	getColor();
		void	setBackColor(byte r, byte g, byte b);
		void	setBackColor(word color);
		word	getBackColor();
		void	print(const char *st, int x, int y, int deg=0);
		void	print( const String st, int x, int y, int deg=0);
		void	printNumI(long num, int x, int y, int length=0, char filler=' ');
		void	printNumF(double num, byte dec, int x, int y, char divider='.', int length=0, char filler=' ');
		fontdatatype* getFont();
		uint8_t	getFontXsize();
		uint8_t	getFontYsize();
		void	drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int scale=1);
		void	drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int deg, int rox, int roy);
    void  displayBitmap(const char *filename, int xCoord, int yCoord);
		// void	lcdOff();
		// void	lcdOn();
		void	setContrast(char c);
		int		getDisplayXSize();
		int		getDisplayYSize();
		// void	setBrightness(byte br);
		// void	setDisplayPage(byte page);
		// void	setWritePage(byte page);
    void  lcd_setrotation(uint8_t m);

		// void LCD_Write_DATA(char VH,char VL);
		// void LCD_Write_COM_DATA(char com1,int dat1);

		void LCD_Write_COM(char VL);
		void LCD_Write_DATA(char VL);
    void point(int x, int y, unsigned long c);
		void setPixel(word color);
    void setPixels(word x, word y, word x1, word y1, uint16_t *p, uint32_t num);
		void drawHLine(int x, int y, int l);
		void drawVLine(int x, int y, int l);
		void setXY(word x1, word y1, word x2, word y2);
		void clrXY();
		void	setFont(fontdatatype* font);
		void rotateChar(byte c, int x, int y, int pos, int deg);
		void printChar(byte c, int x, int y);
		// void _set_direction_registers(byte mode);
		void _convert_float(char *buf, double num, int width, byte prec);
		
    /* Geometric functions */
    void	drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
		void	fillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3);
		void	drawArc(int x, int y, int r, int startAngle, int endAngle, int thickness = 1);
		void	drawPie(int x, int y, int r, int startAngle, int endAngle);

  // drawBitmap utility functions
    uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
    uint32_t bswap32(uint32_t a);
    uint16_t bswap16(uint16_t a);
};

#endif