#include <Adafruit_SPIDevice.h>
#include <LittleFS.h>
#include <FS.h>
#include "Kd_TFT.h"

/*
** Kedei SPI 3.5 inch LCD test.
**
** Uses a v6.2 of the Kedei lcd originally intended for the Raspberry Pi but used here connected to
** an rPi Pico. It could easily be adapted to just about any micro with a SPI
**
** Connections rPi Pico / Pico W:
**
** Pico     TFT
** ---------------
** 15       T_CS
** 16       MISO
** 17       L_CS
** GND      GND
** 18       CLK
** 19       MOSI
** 20       IRQ
** VSYS     5V
*/

#define LCD_CS    17
#define Touch_CS  15
#define Touch_IRQ 20

Adafruit_SPIDevice devLCD = Adafruit_SPIDevice(LCD_CS, 32000000);

KdTFT gLCD(LCD_CS, Touch_CS);

int max_x, max_y;
extern unsigned short tux[0x1000];
 
void setup() {
  if (!Serial) {
    delay(1000);
  }

  Serial.begin(115200);
  Serial.println("Kedei SPI 3.5 inch LCD test");

  randomSeed(analogRead(0));
  
  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting file system");
  }
  else {
    Serial.println("File System mounted successfully");
    // LittleFS.format();
  }

  gLCD.begin(&devLCD);
  gLCD.setFont(SmallFont);

  max_x = gLCD.getDisplayXSize();
  max_y = gLCD.getDisplayYSize();

  gLCD.setColor(VGA_WHITE);

// Draw a 64x64 icon in double size.
  gLCD.fillScr(VGA_WHITE);
  gLCD.drawBitmap (10, 10, 64, 64, tux, 1);
  delay(500);
  gLCD.drawBitmap (80, 10, 64, 64, tux, 2);
  delay(500);
  gLCD.drawBitmap (190, 10, 64, 64, tux, 3);

  delay(2000);
  gLCD.fillScr(VGA_WHITE);

  for (int i = 0; i < 360; i += 20) {
    gLCD.drawBitmap (100, 100, 64, 64, tux, i, 32, 32);
  }

  delay(2000);
}

void loop()
{
  int x1, x3, y1, y3, as, ae;
  int buf[478];
  int x, x2;
  int y, y2;
  int r;

  gLCD.fillScr(VGA_BLACK);
  gLCD.displayBitmap("Angel.bmp", 0, 0);
  delay(2000);
  gLCD.fillScr(VGA_BLACK);
  gLCD.displayBitmap("venom.bmp", 0, 30);
  delay(2000);
  gLCD.fillScr(VGA_BLACK);
  gLCD.displayBitmap("yabby.bmp", 0, 50);
  delay(2000);

// Clear the screen and draw the frame
  gLCD.fillScr(VGA_BLACK);
  gLCD.setColor(255, 0, 0);
  gLCD.fillRect(0, 0, 479, 13);
  gLCD.setColor(64, 64, 64);
  gLCD.fillRect(0, 306, 479, 319);
  gLCD.setColor(255, 255, 255);
  gLCD.setBackColor(255, 0, 0);
  gLCD.print("* Kedei rPi SPI Color TFT Display *", CENTER, 1);
  gLCD.setBackColor(64, 64, 64);
  gLCD.setColor(255,255,0);
  gLCD.print("<http://www.dennyandem.com/>", CENTER, 307);
  gLCD.setColor(0, 0, 255);
  gLCD.drawRect(0, 14, 479, 305);

// Draw crosshairs
  gLCD.setColor(0, 0, 255);
  gLCD.setBackColor(0, 0, 0);
  gLCD.drawLine(239, 15, 239, 304);
  gLCD.drawLine(1, 159, 478, 159);
  for (int i=9; i<470; i+=10)
    gLCD.drawLine(i, 157, i, 161);
  for (int i=19; i<220; i+=10)
    gLCD.drawLine(237, i, 241, i);

// Draw sin-, cos- and tan-lines  
  gLCD.setColor(0,255,255);
  gLCD.print("Sin", 5, 15);
  for (int i = 1; i < 478; i++) {
    gLCD.drawPixel(i, 159 + (sin(((i * 1.13) * 3.14) / 180) * 95));
  }
  
  gLCD.setColor(255,0,0);
  gLCD.print("Cos", 5, 27);
  for (int i = 1; i < 478; i++) {
    gLCD.drawPixel(i, 159 + (cos(((i * 1.13) * 3.14) / 180) * 95));
  }

  gLCD.setColor(255,255,0);
  gLCD.print("Tan", 5, 39);
  for (int i = 1; i < 478; i++) {
    gLCD.drawPixel(i, 159 + (tan(((i * 1.13) * 3.14) / 180)));
  }

  delay(2000);

  gLCD.setColor(0,0,0);
  gLCD.fillRect(1,15,478,304);
  gLCD.setColor(0, 0, 255);
  gLCD.setBackColor(0, 0, 0);
  gLCD.drawLine(239, 15, 239, 304);
  gLCD.drawLine(1, 159, 478, 159);

// Draw a moving sinewave
  x = 1;
  for (int i = 1; i < (478 * 15); i++) 
  {
    x++;
    if (x == 479)
      x = 1;
    if (i > 479)
    {
      if ((x == 239) || (buf[x-1] == 159))
        gLCD.setColor(0, 0, 255);
      else
        gLCD.setColor(0, 0, 0);
      gLCD.drawPixel(x, buf[x-1]);
    }
    gLCD.setColor(0,255,255);
    y = 159 + (sin(((i * 0.7) * 3.14) / 180) * (90 - (i / 100)));
    gLCD.drawPixel(x, y);
    buf[x-1]=y;
  }

  delay(2000);
  
  gLCD.setColor(0,0,0);
  gLCD.fillRect(1, 15, 478, 304);

// Draw some filled rectangles
  for (int i = 1; i < 6; i++)
  {
    switch (i)
    {
      case 1:
        gLCD.setColor(255,0,255);
        break;
      case 2:
        gLCD.setColor(255,0,0);
        break;
      case 3:
        gLCD.setColor(0,255,0);
        break;
      case 4:
        gLCD.setColor(0,0,255);
        break;
      case 5:
        gLCD.setColor(255,255,0);
        break;
    }
    gLCD.fillRect(150 + (i * 20), 70 + (i * 20), 210 + (i * 20), 130 + (i * 20));
  }

  delay(2000);
  
  gLCD.setColor(0,0,0);
  gLCD.fillRect(1,15,478,304);

// Draw some filled, rounded rectangles
  for (int i = 1; i < 6; i++)
  {
    switch (i)
    {
      case 1:
        gLCD.setColor(255,0,255);
        break;
      case 2:
        gLCD.setColor(255,0,0);
        break;
      case 3:
        gLCD.setColor(0,255,0);
        break;
      case 4:
        gLCD.setColor(0,0,255);
        break;
      case 5:
        gLCD.setColor(255,255,0);
        break;
    }
    gLCD.fillRoundRect(270 - (i * 20), 70 + (i * 20), 330 - (i * 20), 130 + (i * 20));
  }
  
  delay(2000);
  
  gLCD.setColor(0, 0, 0);
  gLCD.fillRect(1, 15, 478, 304);

// Draw some random triangles
  for (int i = 0; i < 50; i++)
  {
    gLCD.setColor(random(255), random(255), random(255));
    x1 = random(max_x - 20);
    y1 = random(max_y - 20);
    x2 = random(max_x - 20);
    y2 = random(max_y - 20);
    x3 = random(max_x - 20);
    y3 = random(max_y - 20);
    gLCD.drawTriangle(x1, y1, x2, y2, x3, y3);
  }

  delay(2000);
 
  gLCD.setColor(0,0,0);
  gLCD.fillRect(1, 15, 478, 304);

// Draw some random filled triangles
  for (int i = 0; i < 50; i++)
  {
    gLCD.setColor(random(255), random(255), random(255));
    x1 = random(max_x - 20);
    y1 = random(max_y - 20);
    x2 = random(max_x - 20);
    y2 = random(max_y - 20);
    x3 = random(max_x - 20);
    y3 = random(max_y - 20);
    gLCD.fillTriangle(x1, y1, x2, y2, x3, y3);
  }

  delay(2000);
  gLCD.setColor(0,0,0);
  gLCD.fillRect(1,15,478,304);

// Draw some random pies
  for (int i = 0; i < 50; i++)
  {
    gLCD.setColor(random(255), random(255), random(255));
    x1 = 30 + random(max_x - 60);
    y1 = 30 + random(max_y - 60);
    r = 10 + random(20);
    as = random(360);
    ae = random(360);
    gLCD.drawPie(x1, y1, r, as, ae);
  }

  delay(2000);
  gLCD.setColor(0,0,0);
  gLCD.fillRect(1,15,478,304);

// Draw some filled circles
  for (int i = 1; i < 6; i++)
  {
    switch (i)
    {
      case 1:
        gLCD.setColor(255,0,255);
        break;
      case 2:
        gLCD.setColor(255,0,0);
        break;
      case 3:
        gLCD.setColor(0,255,0);
        break;
      case 4:
        gLCD.setColor(0,0,255);
        break;
      case 5:
        gLCD.setColor(255,255,0);
        break;
    }
    gLCD.fillCircle(180 + (i * 20), 100 + (i * 20), 30);
  }
  
  delay(2000);
  
  gLCD.setColor(VGA_BLACK);
  gLCD.fillRect(1, 15, 478, 304);

// Draw some lines in a pattern
  gLCD.setColor (255,0,0);
  for (int i = 15; i < 304; i+=5)
  {
    gLCD.drawLine(1, i, (i * 1.6) - 10, 304);
  }
  gLCD.setColor (255,0,0);
  for (int i = 304; i > 15; i -= 5)
  {
    gLCD.drawLine(478, i, (i * 1.6) - 11, 15);
  }
  gLCD.setColor (0,255,255);
  for (int i = 304; i > 15; i -= 5)
  {
    gLCD.drawLine(1, i, 491-(i * 1.6), 15);
  }
  gLCD.setColor (0,255,255);
  for (int i = 15; i < 304; i += 5)
  {
    gLCD.drawLine(478, i, 490-(i * 1.6), 304);
  }
  
  delay(2000);
  
  gLCD.setColor(0,0,0);
  gLCD.fillRect(1,15,478,304);

// Draw some random circles
  for (int i = 0; i < 100; i++)
  {
    gLCD.setColor(random(255), random(255), random(255));
    x=32+random(416);
    y=45+random(226);
    r=random(30);
    gLCD.drawCircle(x, y, r);
  }

  delay(2000);
  
  gLCD.setColor(0,0,0);
  gLCD.fillRect(1, 15, 478, 304);

// Draw some random rectangles
  for (int i = 0; i < 100; i++)
  {
    gLCD.setColor(random(255), random(255), random(255));
    x = 2 + random(476);
    y = 16 + random(289);
    x2 = 2 + random(476);
    y2 = 16 + random(289);
    gLCD.drawRect(x, y, x2, y2);
  }

  delay(2000);
  
  gLCD.setColor(0,0,0);
  gLCD.fillRect(1, 15, 478, 304);

// Draw some random rounded rectangles
  for (int i = 0; i < 100; i++)
  {
    gLCD.setColor(random(255), random(255), random(255));
    x = 2 + random(476);
    y = 16 + random(289);
    x2 = 2 + random(476);
    y2 = 16 + random(289);
    gLCD.drawRoundRect(x, y, x2, y2);
  }

  delay(2000);
  
  gLCD.setColor(0,0,0);
  gLCD.fillRect(1,15,478,304);

  for (int i = 0; i < 100; i++)
  {
    gLCD.setColor(random(255), random(255), random(255));
    x = 2 + random(476);
    y = 16 + random(289);
    x2 = 2 + random(476);
    y2 = 16 + random(289);
    gLCD.drawLine(x, y, x2, y2);
  }

  delay(2000);
  
  gLCD.setColor(0,0,0);
  gLCD.fillRect(1,15,478,304);

  for (int i = 0; i < 10000; i++)
  {
    gLCD.setColor(random(255), random(255), random(255));
    gLCD.drawPixel(2 + random(476), 16 + random(289));
  }

  delay(2000);

  gLCD.fillScr(0, 0, 255);
  gLCD.setColor(255, 0, 0);
  gLCD.fillRoundRect(160, 70, 319, 169);
  
  gLCD.setColor(255, 255, 255);
  gLCD.setBackColor(255, 0, 0);
  gLCD.print("That's it!", CENTER, 93);
  gLCD.print("Restarting in a", CENTER, 119);
  gLCD.print("few seconds...", CENTER, 132);
  
  gLCD.setColor(0, 255, 0);
  gLCD.setBackColor(0, 0, 255);
  gLCD.print("Runtime: (msecs)", CENTER, 290);
  gLCD.printNumI(millis(), CENTER, 305);
  
  delay (10000);
}
