#include <Adafruit_SPIDevice.h>
#include "Kd_TFT.h"
#include "XPT2046_Calibrated.h"
#include "Kd_Buttons.h"

/*
** Kedei SPI 3.5 inch LCD Butttons test.
**
** Uses a v6.2 of the Kedei lcd originally intended for the Raspberry Pi but used here connected to
** an rPi Pico. It could easily be adapted to just about any micro with a SPI
**
*/

#define LCD_CS    17
#define Touch_CS  15
#define Touch_IRQ 20

XPT2046_Calibrated ts = XPT2046_Calibrated(Touch_CS, Touch_IRQ);

Adafruit_SPIDevice devLCD = Adafruit_SPIDevice(LCD_CS, 32000000);

KdTFT gLCD(LCD_CS, Touch_CS);

KdTFT_Buttons  TFT_Buttons(&gLCD, &ts);

int max_x, max_y;
int but1, but2, but3, but4, but5, pressed_button;

// touchscreen values used for calibration (derived from Kd_Touch_Calibration.ino)

// source points used for calibration
static uint8_t  const SCREEN_ROTATION = 1;   //LANDSCAPE
static uint16_t const SCREEN_WIDTH    = 480;
static uint16_t const SCREEN_HEIGHT   = 320;

// touchscreen points used for calibration

enum class PointID { NONE = -1, A, B, C, COUNT };

static TS_Point _screenPoint[] = {
  TS_Point( 13,  11), // point A
  TS_Point(312, 113), // point B
  TS_Point(167, 214)  // point C
};

static TS_Point _touchPoint[] = {
  TS_Point(3756, 3408), // point A
  TS_Point(1539, 2614), // point B
  TS_Point(2450, 1726), // point C
};

static TS_Calibration cal(
    _screenPoint[(int)PointID::A], _touchPoint[(int)PointID::A],
    _screenPoint[(int)PointID::B], _touchPoint[(int)PointID::B],
    _screenPoint[(int)PointID::C], _touchPoint[(int)PointID::C],
    SCREEN_WIDTH,
    SCREEN_HEIGHT
);
 
// Declare which bitmaps we will be using
extern unsigned short cat[];
extern unsigned short dog[];
extern unsigned short bird[];
extern unsigned short monkey[];

void setup() {
  while (!Serial) { delay(10); }
  Serial.begin(115200);
  Serial.println("Kedei SPI 3.5 inch LCD test");

  randomSeed(analogRead(0));
  
  //Start touch first 
  if (!ts.begin()) {
    Serial.println("Couldn't start touchscreen controller");
    while (1);
  }

  ts.setRotation(SCREEN_ROTATION);    //Set Touch screen to LANDSCAPE
  ts.calibrate(cal);

  gLCD.begin(&devLCD);
  gLCD.setFont(SmallFont);

  max_x = gLCD.getDisplayXSize();
  max_y = gLCD.getDisplayYSize();

  gLCD.fillScr(VGA_BLACK);
  gLCD.setColor(VGA_WHITE);

  long times = 100;

  while(times-- > 0) {
    // See if there's any  touch data for us
    if (ts.tirqTouched() && ts.touched()) {
      times = 100;
      
      // Retrieve a point  
      TS_Point p = ts.getPoint();
      
      gLCD.drawPixel(p.x, p.y);

      Serial.printf("X = %d, Y = %d, pressure = %d\r\n", p.x, p.y, p.z);
      delay(10);
    }
  }

  gLCD.fillScr(VGA_BLACK);

	TFT_Buttons.deleteAllButtons();

  but1 = TFT_Buttons.addButton(10,  10, 80,  60, cat);
  but2 = TFT_Buttons.addButton(120, 10, 80,  60, dog);
  but3 = TFT_Buttons.addButton(10,  80, 80,  60, bird);
  but4 = TFT_Buttons.addButton(120, 80, 80,  60, monkey, BUTTON_NO_BORDER);
  but5 = TFT_Buttons.addButton(10, 150, 190, 30, "Disable Dog");
  TFT_Buttons.drawButtons();

  gLCD.print("You pressed:", 10, 200);
  gLCD.setColor(VGA_BLACK);
  gLCD.setBackColor(VGA_WHITE);
  gLCD.print("None    ", 10, 220);
}

void loop()
{
  if (ts.tirqTouched() && ts.touched()) {
    pressed_button = TFT_Buttons.checkButtons();
    Serial.println(pressed_button);

    if (pressed_button==but5)
      if (TFT_Buttons.buttonEnabled(but2))
      {
        TFT_Buttons.disableButton(but2);
        TFT_Buttons.relabelButton(but5, "Enable Dog", true);
        TFT_Buttons.drawButton(but2);
      }
      else
      {
        TFT_Buttons.enableButton(but2);
        TFT_Buttons.relabelButton(but5, "Disable Dog", true);
        TFT_Buttons.drawButton(but2);
      }

    if (pressed_button==but1)
      gLCD.print("Cat     ", 10, 220);
    if (pressed_button==but2)
      gLCD.print("Dog     ", 10, 220);
    if (pressed_button==but3)
      gLCD.print("Bird    ", 10, 220);
    if (pressed_button==but4)
      gLCD.print("Monkey  ", 10, 220);
    if (pressed_button==-1)
      gLCD.print("None    ", 10, 220);
  }
 }
