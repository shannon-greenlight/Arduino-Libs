/*
  Spankulator_Display.h - Spankulator_Display library for Wiring - implementation
  Copyright (c) 2006 John Doe.  All right reserved.
*/

// include core Wiring API
#include "Arduino.h"

// include this library's description file
#include <Adafruit_LEDBackpack.h>
#include "Spankulator_Display.h"

// include description files for other libraries used (if any)
#include "HardwareSerial.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

Spankulator_Display::Spankulator_Display()
{
    // do whatever is required to initialize the library
    // Serial.begin(9600);
    // Serial.println("Spankulator Display");
    interrupted = false;
    lag = 60;
    rtl = true;
    centered = false;

    // initialize this instance's variables
    matrix = Adafruit_8x8matrix();
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

void Spankulator_Display::begin()
{
    matrix.begin(0x70); // pass in the address
    matrix.setTextSize(1);
    matrix.setTextWrap(false); // we dont want text to wrap so it scrolls nicely
    matrix.setTextColor(LED_ON);
}

void Spankulator_Display::blinkRate(blink_speed b)
{
    matrix.blinkRate(b);
}

void Spankulator_Display::print_at(String s, int x, int y = 0)
{
    matrix.clear();
    matrix.setCursor(x, y);
    matrix.print(s);
    matrix.writeDisplay();
}

void Spankulator_Display::scroll_text(String s, int num_repeat)
{
    boolean forever = num_repeat == -1;
    int cur = (s.length() * -6);
    int start = centered ? cur : -8;
    while (!interrupted && num_repeat != 0)
    {
        if (rtl)
        {
            for (int16_t x = -start; x >= cur; x--)
            {
                //Serial.println(x);
                if (interrupted)
                    break;
                print_at(s, x);
                delay(lag);
            }
        }
        else
        {
            for (int16_t x = start; x < -cur; x++)
            {
                //Serial.println(x);
                if (interrupted)
                    break;
                print_at(s, x);
                delay(lag);
            }
        }
        if (!forever)
            num_repeat--;
    }
}

void Spankulator_Display::bounce_text(String s, int num_repeat)
{
    boolean forever = num_repeat == -1;
    boolean was_centered = centered;
    boolean was_rtl = rtl;
    centered = true;
    while (!interrupted && num_repeat != 0)
    {
        rtl = true;
        scroll_text("Up", 1);
        delay(50);
        rtl = false;
        scroll_text("Up", 1);
        delay(50);
        if (!forever)
            num_repeat--;
    }
    centered = was_centered;
    rtl = was_rtl;
}

void Spankulator_Display::set_bmp(const uint8_t bitmap[])
{
    matrix.clear();
    matrix.drawBitmap(0, 0, bitmap, 8, 8, LED_ON);
    // matrix.setBrightness(brightness);
    // matrix.blinkRate(blinkRate);
    matrix.writeDisplay();
}

void Spankulator_Display::demo() {
	blinkRate(BLINK_OFF);
	set_bmp(smile_bmp);
	delay(1000);

	matrix.clear(); // clear display
	matrix.drawPixel(0, 0, LED_ON);
	matrix.writeDisplay(); // write the changes we just made to the display
	delay(500);

	matrix.clear();
	matrix.drawLine(0, 0, 7, 7, LED_ON);
	matrix.writeDisplay(); // write the changes we just made to the display
	delay(500);

	matrix.clear();
	matrix.drawRect(0, 0, 8, 8, LED_ON);
	matrix.fillRect(2, 2, 4, 4, LED_ON);
	matrix.writeDisplay(); // write the changes we just made to the display
	delay(500);

	matrix.clear();
	matrix.drawCircle(3, 3, 3, LED_ON);
	matrix.writeDisplay(); // write the changes we just made to the display
	delay(500);
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library
