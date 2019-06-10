/*
  Selector.h - Selector library 
  Copyright (c) 2019 Shannon Vance.  All right reserved.
*/

// ensure this library description is only included once
#ifndef Selector_h
#define Selector_h

// include types & constants of Wiring core API
#include "Arduino.h"

#include <RotaryEncoder.h>
#include <LED_Driver_5916.h>

// library interface description
class Selector
{
  // user-accessible "public" interface
  public:
    Selector(int);
    void set(int);
    int get(void);
	void inc(void);
	void dec(void);
	int num_fxns;
	RotaryEncoder e;

  // library-accessible "private" interface
  private:
    int value;
	LED_Driver_5916 d;
};

#endif

