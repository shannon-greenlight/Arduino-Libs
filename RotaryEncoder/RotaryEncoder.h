/*
  RotaryEncoder - RotaryEncoder library 
  Copyright (c) 2019 Shannon Vance.  All right reserved.
*/

// ensure this library description is only included once
#ifndef RotaryEncoder_h
#define RotaryEncoder_h

// states
#define REST 0
#define CW 1
#define CCW 2
#define UNK 3

// pins
#define encoderPin1 2
#define encoderPin2 3

// include types & constants of Wiring core API
#include "Arduino.h"
#include <TerminalVT100.h>

// library interface description
class RotaryEncoder
{
  // user-accessible "public" interface
  public:
    RotaryEncoder(int);
    void incEncoder(void);
    void decEncoder(void);
	int getEncoderValue(void);
	void setEncoderValue(int);
	void updateEncoder(void);
	void aChanInt(void);
	void bChanInt(void);
	TerminalVT100 t;
	bool debug = false;
	
  // library-accessible "private" interface
  private:
	int numFxns;	
	int state;
	int encoderValue;
	String channel; // A or B
};

#endif

