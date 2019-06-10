/*
  Selector.h - Selector library - implementation
  Copyright (c) 2019 Shannon Vance.  All right reserved.
*/

// include core Wiring API
#include "Arduino.h"

// include this library's description file
#include "Selector.h"

// include description files for other libraries used (if any)
#include "HardwareSerial.h"

#include <RotaryEncoder.h>
#include <LED_Driver_5916.h>

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

Selector::Selector(int n)
{
  // initialize this instance's variables
  num_fxns = n;
  value = 0;

  d = LED_Driver_5916();
  e = RotaryEncoder(num_fxns);

}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

void Selector::set(int val)
{
	value = val;
	d.set_LED(val);
	e.setEncoderValue(val);

}

int Selector::get(void)
{
	value = e.getEncoderValue();
	return value;
}

void inc() {
	e.incEncoder();
}

void dec() {
	e.decEncoder();
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library


