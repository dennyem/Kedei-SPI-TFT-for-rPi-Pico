#include "Kd_TFT.h"
#include <LittleFS.h>
#include <FS.h>

KdTFT::KdTFT(int8_t cspin, int8_t T_cspin) {
  L_CS  = cspin;
  T_CS  = T_cspin;
}

void KdTFT::begin(Adafruit_SPIDevice *lcd) {
  lcd_dev = lcd;

  pinMode(L_CS, OUTPUT);
  pinMode(T_CS, OUTPUT);

  lcd_dev->begin();

  InitLCD(LANDSCAPE);
}

int KdTFT::spi_TxRxLCD(uint8_t *data, int len)
{
  int ret = 0;
  
  digitalWrite(T_CS, HIGH);
 	ret = lcd_dev->write(data, len);
  digitalWrite(T_CS, LOW);        //Must pull touch cs low else lcd won't work
  return ret;
}

void KdTFT::LCD_Write_COM(char VL)  
{   
  uint8_t b1[3];
  b1[0] = 0x11;
	b1[1] = 0x00;
	b1[2] = VL;
	spi_TxRxLCD(b1, sizeof(b1));
}

void KdTFT::LCD_Write_DATA(char VL)
{
  uint8_t b1[3];
  b1[0] = 0x15;
  b1[1] = 0x00;
  b1[2] = VL;
	spi_TxRxLCD(b1, sizeof(b1));
}

// void KdTFT::LCD_Write_COM_DATA(char com1, int dat1)
// {
//      LCD_Write_COM(com1);
//      LCD_Write_DATA(dat1 >> 8, dat1);
// }

void KdTFT::LCD_Reset(void)
{
	uint8_t buff[4];

  /* Reset LCD */
	buff[0] = 0x00;
	buff[1] = 0x01;
	buff[2] = 0x00;
  buff[3] = 0x00;
	spi_TxRxLCD(buff, sizeof(buff));
	buff[1] = 0x00;
	spi_TxRxLCD(buff, sizeof(buff));
	buff[1] = 0x01;
	spi_TxRxLCD(buff, sizeof(buff));
}

void KdTFT::InitLCD(byte orientation)
{
  LCD_Reset();

  LCD_Write_COM(0x00);  //kedei 6.2
  delayMicroseconds(10);
  LCD_Write_COM(0xFF); LCD_Write_COM(0xFF);
  delayMicroseconds(10);
  LCD_Write_COM(0xFF); LCD_Write_COM(0xFF); LCD_Write_COM(0xFF); LCD_Write_COM(0xFF);
  delayMicroseconds(15);
  LCD_Write_COM(0x11);
  delayMicroseconds(150);

  LCD_Write_COM(0xB0); LCD_Write_DATA(0x00);
  LCD_Write_COM(0xB3); LCD_Write_DATA(0x02); LCD_Write_DATA(0x00); LCD_Write_DATA(0x00); LCD_Write_DATA(0x00);
  LCD_Write_COM(0xB9); LCD_Write_DATA(0x01); LCD_Write_DATA(0x00); LCD_Write_DATA(0x0F); LCD_Write_DATA(0x0F);
  LCD_Write_COM(0xC0); LCD_Write_DATA(0x13); LCD_Write_DATA(0x3B); LCD_Write_DATA(0x00); LCD_Write_DATA(0x02);
                       LCD_Write_DATA(0x00); LCD_Write_DATA(0x01); LCD_Write_DATA(0x00); LCD_Write_DATA(0x43);
  LCD_Write_COM(0xC1); LCD_Write_DATA(0x08); LCD_Write_DATA(0x0F); LCD_Write_DATA(0x08); LCD_Write_DATA(0x08);
  LCD_Write_COM(0xC4); LCD_Write_DATA(0x11); LCD_Write_DATA(0x07); LCD_Write_DATA(0x03); LCD_Write_DATA(0x04);
  LCD_Write_COM(0xC6); LCD_Write_DATA(0x00);
  LCD_Write_COM(0xC8); LCD_Write_DATA(0x03); LCD_Write_DATA(0x03); LCD_Write_DATA(0x13); LCD_Write_DATA(0x5C);
                       LCD_Write_DATA(0x03); LCD_Write_DATA(0x07); LCD_Write_DATA(0x14); LCD_Write_DATA(0x08);
                       LCD_Write_DATA(0x00); LCD_Write_DATA(0x21); LCD_Write_DATA(0x08); LCD_Write_DATA(0x14);
                       LCD_Write_DATA(0x07); LCD_Write_DATA(0x53); LCD_Write_DATA(0x0C); LCD_Write_DATA(0x13);
                       LCD_Write_DATA(0x03); LCD_Write_DATA(0x03); LCD_Write_DATA(0x21); LCD_Write_DATA(0x00);
  LCD_Write_COM(0x35); LCD_Write_DATA(0x00);
  LCD_Write_COM(0x36); LCD_Write_DATA(0x60); //0x60
  LCD_Write_COM(0x3A); LCD_Write_DATA(0x55);
  LCD_Write_COM(0x44); LCD_Write_DATA(0x00); LCD_Write_DATA(0x01);
  LCD_Write_COM(0xD0); LCD_Write_DATA(0x07); LCD_Write_DATA(0x07); LCD_Write_DATA(0x1D); LCD_Write_DATA(0x03);
  LCD_Write_COM(0xD1); LCD_Write_DATA(0x03); LCD_Write_DATA(0x30); LCD_Write_DATA(0x10);
  LCD_Write_COM(0xD2); LCD_Write_DATA(0x03); LCD_Write_DATA(0x14); LCD_Write_DATA(0x04);
  LCD_Write_COM(0x29);

  delayMicroseconds(30);

  LCD_Write_COM(0x2A); LCD_Write_DATA(0x00); LCD_Write_DATA(0x00); LCD_Write_DATA(0x01); LCD_Write_DATA(0x3F);
  LCD_Write_COM(0x2B); LCD_Write_DATA(0x00); LCD_Write_DATA(0x00); LCD_Write_DATA(0x01); LCD_Write_DATA(0xE0);
  LCD_Write_COM(0xB4); LCD_Write_DATA(0x00);

  LCD_Write_COM(0x2C);

  delay(10);

	orient = orientation;

  lcd_setrotation(0);

	setColor(0, 0, 0);
	setBackColor(0, 0, 0);
	cfont.font = 0;
	_transparent = false;
}

uint8_t lcd_rotations[4] = {
/*MY MX MV ML BGR SS*/
	0b01100000,	//   0  Top left = 0.0
	0b01001000,	//  90
	0b00100000,	// 180
	0b00010100	// 270
};

void KdTFT::lcd_setrotation(uint8_t m)
{
	LCD_Write_COM(0x36);
  LCD_Write_DATA(lcd_rotations[m]);

	if(!(m & 1))
    orient = PORTRAIT;
  else
    orient = LANDSCAPE;
}


void KdTFT::setXY(word x1, word y1, word x2, word y2)
{
	if (orient == LANDSCAPE)
	{
		swap3(word, x1, y1);
		swap3(word, x2, y2)
		y1=disp_y_size-y1;
		y2=disp_y_size-y2;
		swap3(word, y1, y2)
	}

  LCD_Write_COM(0x2a); 
    LCD_Write_DATA(x1>>8);
    LCD_Write_DATA(x1);
    LCD_Write_DATA(x2>>8);
    LCD_Write_DATA(x2);
  LCD_Write_COM(0x2b); 
    LCD_Write_DATA(y1>>8);
    LCD_Write_DATA(y1);
    LCD_Write_DATA(y2>>8);
    LCD_Write_DATA(y2);
  LCD_Write_COM(0x2c);     
}

void KdTFT::clrXY()
{
	if (orient==PORTRAIT)
		setXY(0, 0, disp_x_size, disp_y_size);
	else
		setXY(0, 0, disp_y_size, disp_x_size);
}

void KdTFT::drawRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap3(int, x1, x2);
	}
	if (y1>y2)
	{
		swap3(int, y1, y2);
	}

	drawHLine(x1, y1, x2-x1);
	drawHLine(x1, y2, x2-x1);
	drawVLine(x1, y1, y2-y1);
	drawVLine(x2, y1, y2-y1);
}

void KdTFT::drawRoundRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap3(int, x1, x2);
	}
	if (y1>y2)
	{
		swap3(int, y1, y2);
	}
	if ((x2-x1)>4 && (y2-y1)>4)
	{
		drawPixel(x1+1,y1+1);
		drawPixel(x2-1,y1+1);
		drawPixel(x1+1,y2-1);
		drawPixel(x2-1,y2-1);
		drawHLine(x1+2, y1, x2-x1-4);
		drawHLine(x1+2, y2, x2-x1-4);
		drawVLine(x1, y1+2, y2-y1-4);
		drawVLine(x2, y1+2, y2-y1-4);
	}
}

void KdTFT::fillRect(int x1, int y1, int x2, int y2)
{
	if (x1 > x2) {
		swap3(int, x1, x2);
	}
	if (y1 > y2) {
		swap3(int, y1, y2);
	}

	setXY(x1, y1, x2, y2);

  if (orient==PORTRAIT)  {
    for (int i = 0; i < ((y2 - y1) / 2) + 1; i++)
    {
      drawHLine(x1, y1 + i, x2 - x1);
      drawHLine(x1, y2 - i, x2 - x1);
    }
  }
  else
  {
    for (int i = 0; i < ((x2 - x1) / 2) + 1; i++)
    {
      drawVLine(x1 + i, y1, y2 - y1);
      drawVLine(x2 - i, y1, y2 - y1);
    }
  }
}

void KdTFT::fillRoundRect(int x1, int y1, int x2, int y2)
{
	if (x1>x2)
	{
		swap3(int, x1, x2);
	}
	if (y1>y2)
	{
		swap3(int, y1, y2);
	}

	if ((x2-x1)>4 && (y2-y1)>4)
	{
		for (int i=0; i<((y2-y1)/2)+1; i++)
		{
			switch(i)
			{
			case 0:
				drawHLine(x1+2, y1+i, x2-x1-4);
				drawHLine(x1+2, y2-i, x2-x1-4);
				break;
			case 1:
				drawHLine(x1+1, y1+i, x2-x1-2);
				drawHLine(x1+1, y2-i, x2-x1-2);
				break;
			default:
				drawHLine(x1, y1+i, x2-x1);
				drawHLine(x1, y2-i, x2-x1);
			}
		}
	}
}

void KdTFT::point(int x, int y, unsigned long c) {
  setXY(x, y, x, y);
  setPixel(c);
}

void KdTFT::drawCircle(int xo, int yo, int radius)
{
  int e = 0;
  int x = radius;
  int y = 0;

  while (x >= y) {
    point(xo + x, yo + y, fch);  //eight 45 degree sectors
    point(xo - x, yo + y, fch);
    point(xo + x, yo - y, fch);
    point(xo - x, yo - y, fch);
    point(xo + y, yo + x, fch);
    point(xo - y, yo + x, fch);
    point(xo + y, yo - x, fch);
    point(xo - y, yo - x, fch);
    e = e + 1 + 2 * y;
    y = y + 1;
    if (2 * (e - x) + 1 > 0) {
      x = x - 1;
      e = e + 1 - 2 * x;
    }
  }
}

void KdTFT::fillCircle(int x, int y, int radius)
{
	for(int y1=-radius; y1<=0; y1++) 
		for(int x1=-radius; x1<=0; x1++)
			if(x1*x1+y1*y1 <= radius*radius) 
			{
				drawHLine(x+x1, y+y1, 2*(-x1));
				drawHLine(x+x1, y-y1, 2*(-x1));
				break;
			}
}

void KdTFT::fillScr(byte r, byte g, byte b)
{
	word color = ((r&248)<<8 | (g&252)<<3 | (b&248)>>3);
	fillScr(color);
}

void KdTFT::fillScr(word color)
{
	long i;
  uint8_t b1[3];

  b1[0] = 0x15;
  b1[1] = byte(color >> 8);
  b1[2] = byte(color & 0xFF);

	setXY(0, 0, disp_x_size, disp_y_size);
  for (i=0; i < ((disp_x_size + 1) * (disp_y_size + 1)); i++) {
    spi_TxRxLCD(b1, sizeof(b1));
  }

	clrXY();
}

void KdTFT::setColor(byte r, byte g, byte b)
{
	fch = ((r&248) | g >> 5);
	fcl = ((g&28) <<3 | b >> 3);
}

void KdTFT::setColor(word color)
{
	fch = byte(color >> 8);
	fcl = byte(color & 0xFF);
}

word KdTFT::getColor()
{
	return (fch << 8) | fcl;
}

void KdTFT::setBackColor(byte r, byte g, byte b)
{
	bch=((r&248)|g>>5);
	bcl=((g&28)<<3|b>>3);
	_transparent=false;
}

void KdTFT::setBackColor(word color)
{
	if (color==VGA_TRANSPARENT)
		_transparent=true;
	else
	{
		bch=byte(color>>8);
		bcl=byte(color & 0xFF);
		_transparent=false;
	}
}

word KdTFT::getBackColor()
{
	return (bch<<8) | bcl;
}

void KdTFT::setPixel(word color)
{
  uint8_t b1[3];
  b1[0] = 0x15;
  b1[1] = color >> 8;
  b1[2] = color & 0xFF;
  spi_TxRxLCD(b1, sizeof(b1));
}

void KdTFT::setPixels(word x, word y, word x1, word y1, uint16_t *p, uint32_t num)
{
  uint8_t b1[3];

  b1[0] = 0x15;

  setXY(x, y, x1, y1);
  for(int l = 0; l < num; l++) {
    b1[1] = p[l] >> 8;
    b1[2] = p[l] & 0xFF;
    spi_TxRxLCD(b1, sizeof(b1));
  }
	clrXY();
}

void KdTFT::drawPixel(int x, int y)
{
	setXY(x, y, x, y);
	setPixel((fch<<8)|fcl);
	clrXY();
}

void KdTFT::drawLine(int x1, int y1, int x2, int y2)
{
	uint8_t b1[3];

  b1[0] = 0x15;
	b1[1] = fch;
	b1[2] = fcl;

	if (y1==y2)
		drawHLine(x1, y1, x2-x1);
	else if (x1==x2)
		drawVLine(x1, y1, y2-y1);
	else
	{
		unsigned int	dx = (x2 > x1 ? x2 - x1 : x1 - x2);
		short			xstep =  x2 > x1 ? 1 : -1;
		unsigned int	dy = (y2 > y1 ? y2 - y1 : y1 - y2);
		short			ystep =  y2 > y1 ? 1 : -1;
		int				col = x1, row = y1;

		if (dx < dy)
		{
			int t = - (dy >> 1);
			while (true)
			{
				setXY (col, row, col, row);
			  spi_TxRxLCD(b1, sizeof(b1));

				if (row == y2)
					return;
				row += ystep;
				t += dx;
				if (t >= 0)
				{
					col += xstep;
					t   -= dy;
				}
			} 
		}
		else
		{
			int t = - (dx >> 1);
			while (true)
			{
				setXY (col, row, col, row);
    	  spi_TxRxLCD(b1, sizeof(b1));
				if (col == x2)
					return;
				col += xstep;
				t += dy;
				if (t >= 0)
				{
					row += ystep;
					t   -= dx;
				}
			} 
		}
	}
	clrXY();
}

void KdTFT::drawHLine(int x, int y, int l)
{
	uint8_t b1[3];

  b1[0] = 0x15;
	b1[1] = fch;
	b1[2] = fcl;

	if(l < 0)
	{
		l = -l;
		x -= l;
	}
	setXY(x, y, x+l, y);

  for (int i = 0; i < l + 1; i++) {
	  spi_TxRxLCD(b1, sizeof(b1));
  }

	clrXY();
}

void KdTFT::drawVLine(int x, int y, int l)
{
	uint8_t b1[3];

  b1[0] = 0x15;
	b1[1] = fch;
	b1[2] = fcl;

	if (l < 0)
	{
		l = -l;
		y -= l;
	}

	setXY(x, y, x, y+l);

  for (int i=0; i<l+1; i++) {
	  spi_TxRxLCD(b1, sizeof(b1));
  }

	clrXY();
}

void KdTFT::printChar(byte c, int x, int y)
{
	byte i,ch;
	word j;
	word temp; 

	if (!_transparent)
	{
		if (orient==PORTRAIT)
		{
			setXY(x,y,x+cfont.x_size-1,y+cfont.y_size-1);
	  
			temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
			for(j=0;j<((cfont.x_size/8)*cfont.y_size);j++)
			{
				ch=pgm_read_byte(&cfont.font[temp]);
				for(i=0;i<8;i++)
				{   
					if((ch&(1<<(7-i)))!=0)   
					{
						setPixel((fch<<8)|fcl);
					} 
					else
					{
						setPixel((bch<<8)|bcl);
					}   
				}
				temp++;
			}
		}
		else
		{
			temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;

			for(j=0;j<((cfont.x_size/8)*cfont.y_size);j+=(cfont.x_size/8))
			{
				setXY(x,y+(j/(cfont.x_size/8)),x+cfont.x_size-1,y+(j/(cfont.x_size/8)));
				for (int zz=(cfont.x_size/8)-1; zz>=0; zz--)
				{
					ch=pgm_read_byte(&cfont.font[temp+zz]);
					for(i=0;i<8;i++)
					{   
						if((ch&(1<<i))!=0)   
						{
							setPixel((fch<<8)|fcl);
						} 
						else
						{
							setPixel((bch<<8)|bcl);
						}   
					}
				}
				temp+=(cfont.x_size/8);
			}
		}
	}
	else
	{
		temp=((c - cfont.offset) * ((cfont.x_size / 8) * cfont.y_size)) + 4;
		for(j = 0; j < cfont.y_size; j++) 
		{
			for (int zz = 0; zz < (cfont.x_size / 8); zz++)
			{
				ch = pgm_read_byte(&cfont.font[temp + zz]); 
				for(i = 0; i < 8; i++)
				{   
				
					if((ch&(1 << (7 - i))) != 0)   
					{
						setXY(x + i + (zz * 8), y + j, x + i + (zz * 8) + 1, y + j + 1);
						setPixel((fch << 8) | fcl);
					} 
				}
			}
			temp += (cfont.x_size / 8);
		}
	}

	clrXY();
}

void KdTFT::rotateChar(byte c, int x, int y, int pos, int deg)
{
	byte i,j,ch;
	word temp; 
	int newx,newy;
	double radian;
	radian=deg*0.0175;  

	//	cbi(P_CS, B_CS);

	temp=((c-cfont.offset)*((cfont.x_size/8)*cfont.y_size))+4;
	for(j=0;j<cfont.y_size;j++) 
	{
		for (int zz=0; zz<(cfont.x_size/8); zz++)
		{
			ch=pgm_read_byte(&cfont.font[temp+zz]); 
			for(i=0;i<8;i++)
			{   
				newx=x+(((i+(zz*8)+(pos*cfont.x_size))*cos(radian))-((j)*sin(radian)));
				newy=y+(((j)*cos(radian))+((i+(zz*8)+(pos*cfont.x_size))*sin(radian)));

				setXY(newx,newy,newx+1,newy+1);
				
				if((ch&(1<<(7-i)))!=0)   
				{
					setPixel((fch<<8)|fcl);
				} 
				else  
				{
					if (!_transparent)
						setPixel((bch<<8)|bcl);
				}   
			}
		}
		temp+=(cfont.x_size/8);
	}
	//	sbi(P_CS, B_CS);
	clrXY();
}

void KdTFT::print(const char *st, int x, int y, int deg)
{
	int stl, i;

	stl = strlen(st);

	if (orient==PORTRAIT)
	{
	if (x==RIGHT)
		x=(disp_x_size+1)-(stl*cfont.x_size);
	if (x==CENTER)
		x=((disp_x_size+1)-(stl*cfont.x_size))/2;
	}
	else
	{
	if (x==RIGHT)
		x=(disp_y_size+1)-(stl*cfont.x_size);
	if (x==CENTER)
		x=((disp_y_size+1)-(stl*cfont.x_size))/2;
	}

	for (i=0; i<stl; i++)
		if (deg==0)
			printChar(*st++, x + (i*(cfont.x_size)), y);
		else
			rotateChar(*st++, x, y, i, deg);
}

void KdTFT::print(const String st, int x, int y, int deg)
{
	char buf[st.length()+1];

	st.toCharArray(buf, st.length()+1);
	print(buf, x, y, deg);
}

void KdTFT::printNumI(long num, int x, int y, int length, char filler)
{
	char buf[25];
	char st[27];
	boolean neg=false;
	int c=0, f=0;
  
	if (num==0)
	{
		if (length!=0)
		{
			for (c=0; c<(length-1); c++)
				st[c]=filler;
			st[c]=48;
			st[c+1]=0;
		}
		else
		{
			st[0]=48;
			st[1]=0;
		}
	}
	else
	{
		if (num<0)
		{
			neg=true;
			num=-num;
		}
	  
		while (num>0)
		{
			buf[c]=48+(num % 10);
			c++;
			num=(num-(num % 10))/10;
		}
		buf[c]=0;
	  
		if (neg)
		{
			st[0]=45;
		}
	  
		if (length>(c+neg))
		{
			for (int i=0; i<(length-c-neg); i++)
			{
				st[i+neg]=filler;
				f++;
			}
		}

		for (int i=0; i<c; i++)
		{
			st[i+neg+f]=buf[c-i-1];
		}
		st[c+neg+f]=0;

	}

	print(st,x,y);
}

void KdTFT::printNumF(double num, byte dec, int x, int y, char divider, int length, char filler)
{
	char st[27];
	boolean neg=false;

	if (dec<1)
		dec=1;
	else if (dec>5)
		dec=5;

	if (num<0)
		neg = true;

	_convert_float(st, num, length, dec);

	if (divider != '.')
	{
		for (int i=0; i<sizeof(st); i++)
			if (st[i]=='.')
				st[i]=divider;
	}

	if (filler != ' ')
	{
		if (neg)
		{
			st[0]='-';
			for (int i=1; i<sizeof(st); i++)
				if ((st[i]==' ') || (st[i]=='-'))
					st[i]=filler;
		}
		else
		{
			for (int i=0; i<sizeof(st); i++)
				if (st[i]==' ')
					st[i]=filler;
		}
	}

	print(st, x, y);
}

void KdTFT::setFont(fontdatatype* font)
{
	cfont.font=font;
	cfont.x_size=fontbyte(0);
	cfont.y_size=fontbyte(1);
	cfont.offset=fontbyte(2);
	cfont.numchars=fontbyte(3);
}

fontdatatype* KdTFT::getFont()
{
	return cfont.font;
}

uint8_t KdTFT::getFontXsize()
{
	return cfont.x_size;
}

uint8_t KdTFT::getFontYsize()
{
	return cfont.y_size;
}

void KdTFT::drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int scale)
{
	unsigned int col;
	int tx, ty, tc, tsx, tsy;

	if (scale==1)
	{
		if (orient==PORTRAIT)
		{
			setXY(x, y, x+sx-1, y+sy-1);
			for (tc=0; tc<(sx*sy); tc++)
			{
				col=pgm_read_word(&data[tc]);
        setPixel(col);
			}
		}
		else
		{
			for (ty=0; ty<sy; ty++)
			{
				setXY(x, y+ty, x+sx-1, y+ty);
				for (tx=sx-1; tx>=0; tx--)
				{
					col=pgm_read_word(&data[(ty*sx)+tx]);
          setPixel(col);
				}
			}
		}
	}
	else
	{
		if (orient==PORTRAIT)
		{
			for (ty=0; ty<sy; ty++)
			{
				setXY(x, y+(ty*scale), x+((sx*scale)-1), y+(ty*scale)+scale);
				for (tsy=0; tsy<scale; tsy++)
					for (tx=0; tx<sx; tx++)
					{
						col=pgm_read_word(&data[(ty*sx)+tx]);
						for (tsx=0; tsx<scale; tsx++)
              setPixel(col);
					}
			}
		}
		else
		{
			for (ty=0; ty<sy; ty++)
			{
				for (tsy=0; tsy<scale; tsy++)
				{
					setXY(x, y+(ty*scale)+tsy, x+((sx*scale)-1), y+(ty*scale)+tsy);
					for (tx=sx-1; tx>=0; tx--)
					{
						col=pgm_read_word(&data[(ty*sx)+tx]);
						for (tsx=0; tsx<scale; tsx++)
              setPixel(col);
					}
				}
			}
		}
	}
	clrXY();
}

void KdTFT::drawBitmap(int x, int y, int sx, int sy, bitmapdatatype data, int deg, int rox, int roy)
{
	unsigned int col;
	int tx, ty, newx, newy;
	double radian;
	radian=deg*0.0175;  

	if (deg==0)
		drawBitmap(x, y, sx, sy, data);
	else
	{
		for (ty=0; ty<sy; ty++)
			for (tx=0; tx<sx; tx++)
			{
				col=pgm_read_word(&data[(ty*sx)+tx]);

				newx=x+rox+(((tx-rox)*cos(radian))-((ty-roy)*sin(radian)));
				newy=y+roy+(((ty-roy)*cos(radian))+((tx-rox)*sin(radian)));

				setXY(newx, newy, newx, newy);
        setPixel(col);
			}
	}
	clrXY();
}

/*** Draws a bitmap contained in a LittleFS File <filename> ***/
void KdTFT::displayBitmap(const char *filename, int xCoord, int yCoord)
{
  File filePtr;
  BITMAPFILEHEADER fileHeader;
  BITMAPINFOHEADER infoHeader;
  COLORMAP        *clrTable = 0;  // Used for 8 bit dibs only at this stage
  unsigned char   *bitmapImage;
  uint16_t        *lineBuffer;    // 565 Color data by line
  int imageIdx;                   // image index counter
  int bufSize;
  int currentLine;
  int numLines; 
  uint8_t b, g, r, w;
  
  if ((filePtr = LittleFS.open(filename, "r")) == false) {
    Serial.printf("File %s not found\r\n", filename);
    return;
  }

  //read the bitmap file header
  filePtr.read((uint8_t *)&fileHeader, sizeof(BITMAPFILEHEADER));

  //verify that this is a .BMP file by checking bitmap id
  if (fileHeader.bfType != 0x4D42)  {
    Serial.printf("File %s is not a valid bmp\r\n", filename);
    filePtr.close();
    return;
  }

  //read the bitmap info header
  filePtr.read((uint8_t *)&infoHeader, sizeof(BITMAPINFOHEADER)); 
  
  fileHeader.bfSize         = bswap32(fileHeader.bfSize);     // Swap endianness for each element
  fileHeader.bfOffBits      = bswap32(fileHeader.bfOffBits);

  infoHeader.biSize         = bswap32(infoHeader.biSize);
  infoHeader.biBitCount     = bswap16(infoHeader.biBitCount);
  infoHeader.biPlanes       = bswap16(infoHeader.biPlanes);
  infoHeader.biCompression  = bswap32(infoHeader.biCompression);
  infoHeader.biSizeImage    = bswap32(infoHeader.biSizeImage);
  infoHeader.biWidth        = bswap32(infoHeader.biWidth);
  infoHeader.biHeight       = bswap32(infoHeader.biHeight);
  infoHeader.biClrUsed      = bswap32(infoHeader.biClrUsed);
  infoHeader.biClrImportant = bswap32(infoHeader.biClrImportant);

  // Serial.printf("Width: 0x%X (%d), Height: 0x%X (%d)\r\n", infoHeader.biWidth, infoHeader.biWidth, infoHeader.biHeight, infoHeader.biHeight);
  // Serial.printf("biBitCount: 0x%X (%d)\r\n", infoHeader.biBitCount, infoHeader.biBitCount);
  // Serial.printf("Compression: 0x%X\r\n", infoHeader.biCompression);
  // Serial.printf("biClrUsed: 0x%X (%d)\r\n", infoHeader.biClrUsed, infoHeader.biClrUsed);

  if(infoHeader.biBitCount == 8 && infoHeader.biClrUsed > 0) {                   // Get the colur pallette for 8 bit dib's
    clrTable = (COLORMAP *)malloc((sizeof(COLORMAP) * infoHeader.biClrUsed));    // create an array of 4 byte colors
    filePtr.read((uint8_t *)clrTable, sizeof(COLORMAP) * infoHeader.biClrUsed); 
  }

  //move file pointer to the beginning of bitmap data
  filePtr.seek(fileHeader.bfOffBits);

  bufSize = infoHeader.biSizeImage / infoHeader.biHeight;  // enough Ram for one line (could be 1, 3 or 4 bytes per pixel)

  bitmapImage = (unsigned char*)malloc(bufSize);
  lineBuffer  = (uint16_t *)malloc(infoHeader.biWidth * 2);

  currentLine = infoHeader.biHeight;          //DIB's are bottom up coordinate system

  for(numLines = 0; numLines < infoHeader.biHeight; numLines++) {
    filePtr.read(bitmapImage, bufSize);       // read in bufSize chunks of bitmap image data

    imageIdx = 0;

    for(int x = 0; x < infoHeader.biWidth; x++) {
      switch(infoHeader.biBitCount) {
      case 32:  w = bitmapImage[imageIdx++];  // Alpha bits are ignored at this time
                b = bitmapImage[imageIdx++];
                g = bitmapImage[imageIdx++];
                r = bitmapImage[imageIdx++];
                break;
      case 24:  b = bitmapImage[imageIdx++];
                g = bitmapImage[imageIdx++];
                r = bitmapImage[imageIdx++];
                break;
      case 8:   b = clrTable[bitmapImage[imageIdx]].b;
                g = clrTable[bitmapImage[imageIdx]].g;
                r = clrTable[bitmapImage[imageIdx++]].r;
                break;
      }
      lineBuffer[x] = color565(r, g, b);
    }

    setPixels(xCoord, currentLine + yCoord, infoHeader.biWidth, 1, lineBuffer, infoHeader.biWidth);
    currentLine--;
  }

  // Free up dynamically allocated ram
  if(clrTable != 0)   //clrTable is only used for 8 bit DIBs
    free(clrTable);

  free(bitmapImage);
  free(lineBuffer);
  filePtr.close();
}

// void KdTFT::lcdOff()
// {
// }

// void KdTFT::lcdOn()
// {
// }

// void KdTFT::setBrightness(byte br)
// {
// }

// void KdTFT::setContrast(char c)
// {
// }

int KdTFT::getDisplayXSize()
{
	if (orient==PORTRAIT)
		return disp_x_size+1;
	else
		return disp_y_size+1;
}

int KdTFT::getDisplayYSize()
{
	if (orient==PORTRAIT)
		return disp_y_size+1;
	else
		return disp_x_size+1;
}

void KdTFT::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	drawLine(x1, y1, x2, y2);
	drawLine(x2, y2, x3, y3);
	drawLine(x3, y3, x1, y1);
}

void KdTFT::fillTriangle(int32_t x1, int32_t y1, int32_t x2, int32_t y2, int32_t x3, int32_t y3)
{
	int32_t xs, xe;
	int16_t y, ly;

	if (y1 > y2)
	{
		swap2(y1, y2); 
		swap2(x1, x2);
	}
	if (y2 > y3)
	{
		swap2(y3, y2);
		swap2(x3, x2);
	}
	if (y1 > y2)
	{
		swap2(y1, y2);
		swap2(x1, x2);
	}
	
	if(y1 == y3)	// Single line triangles
	{
		xs = xe = x1;
		if(x2 < xs)			xs = x2;
		else if(x2 > xe)	xe = x2;
		if(x3 < xs)			xs = x3;
		else if(x3 > xe)	xe = x3;
		drawHLine(xs, y1, xe - xs);
		return;
	}
	
	// Upper part
	if (y2 == y3) ly = y2;
	else          ly = y2-1;
	
	for(y=y1; y<=ly; y++)
	{
		xs = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
		xe = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
		drawHLine(xs, y, xe-xs);
	}
	
	// Lower part
	for(; y<=y3; y++)
	{
		xs = x2 + (x3 - x2) * (y - y2) / (y3 - y2);
		xe = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
		drawHLine(xs, y, xe-xs);
	}
}

void KdTFT::drawArc(int x, int y, int r, int startAngle, int endAngle, int thickness)
{
	int rDelta = -(thickness/2);
	int px, py, cx, cy;

	startAngle -= 90;
	endAngle   -= 90;
	
	if (startAngle!=endAngle)
	{
		for (int i=0; i<thickness; i++)
		{
			px = x + cos((startAngle*3.14)/180) * (r+rDelta+i);
			py = y + sin((startAngle*3.14)/180) * (r+rDelta+i);
			for (int d=startAngle+1; d<endAngle+1; d++)
			{
				cx = x + cos((d*3.14)/180) * (r+rDelta+i);
				cy = y + sin((d*3.14)/180) * (r+rDelta+i);
				drawLine(px, py, cx, cy);
				px = cx;
				py = cy;
			}
		}
	}
	else
	{
		px = x + cos((startAngle*3.14)/180) * (r+rDelta);
		py = y + sin((startAngle*3.14)/180) * (r+rDelta);
		cx = x + cos((startAngle*3.14)/180) * (r-rDelta);
		cy = y + sin((startAngle*3.14)/180) * (r-rDelta);
		drawLine(px, py, cx, cy);
	}
}

void KdTFT::drawPie(int x, int y, int r, int startAngle, int endAngle)
{
	int px, py, cx, cy;

	startAngle -= 90;
	endAngle   -= 90;
	if (startAngle>endAngle)
		startAngle -= 360;
	
	px = x + cos((startAngle*3.14)/180) * r;
	py = y + sin((startAngle*3.14)/180) * r;
	drawLine(x, y, px, py);
	for (int d=startAngle+1; d<endAngle+1; d++)
	{
		cx = x + cos((d*3.14)/180) * r;
		cy = y + sin((d*3.14)/180) * r;
		drawLine(px, py, cx, cy);
		px = cx;
		py = cy;
	}
	drawLine(x, y, px, py);
}

uint16_t KdTFT::bswap16(uint16_t a)
{
  uint8_t tmp_a = (a & 0xff00) >> 8 ;
  uint8_t tmp_b = (a & 0x00ff);
  uint16_t b = (tmp_b | (tmp_a << 8));
  return b;
}

uint32_t KdTFT::bswap32(uint32_t a)
{
  uint8_t tmp_a = (a & 0xff000000) >> 24;
  uint8_t tmp_b = (a & 0x00ff0000) >> 16;
  uint8_t tmp_c = (a & 0x0000ff00) >> 8 ;
  uint8_t tmp_d = (a & 0x000000ff);
  uint32_t b = (tmp_d | (tmp_c << 8) | (tmp_b << 16) | (tmp_a << 24));
  return b;
}

uint16_t KdTFT::color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

