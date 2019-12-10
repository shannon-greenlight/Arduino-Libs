/*
  Spankulator_Display.h - Spankulator_Display library for Wiring - description
  Copyright (c) 2006 John Doe.  All right reserved.
*/

// ensure this library description is only included once
#ifndef Spankulator_Display_h
#define Spankulator_Display_h

// include types & constants of Wiring core API
#include "Arduino.h"
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>
#include "Spankulator_Bitmaps.h"

enum blink_speed
{
	BLINK_OFF,
	BLINK_FAST,
	BLINK_MEDIUM,
	BLINK_SLOW
};

// library interface description
class Spankulator_Display
{
    // user-accessible "public" interface
public:
    Spankulator_Display();
    boolean interrupted;
    int lag;
    boolean rtl;
    boolean centered;
    void begin(void);
    void blinkRate(blink_speed);
    void print_at(String, int, int);
    void scroll_text(String, int);
    void bounce_text(String, int);
    void set_bmp(const uint8_t*);
    void demo(void);
    
    Adafruit_8x8matrix matrix;  // move this to private when done developing


    // library-accessible "private" interface
private:

};

#endif
