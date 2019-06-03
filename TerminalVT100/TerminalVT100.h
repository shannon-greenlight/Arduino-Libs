/*
  TerminalVT100 -  library 
  Copyright (c) 2019 Shannon Vance.  All right reserved.
*/

// ensure this library description is only included once
#ifndef TerminalVT100_h
#define TerminalVT100_h

// include types & constants of Wiring core API
#include "Arduino.h"

// library interface description
class TerminalVT100
{
  // user-accessible "public" interface
  public:
    TerminalVT100(void);
	void setCursor(String, String);
	void setRow(String);
	void clrScreen(void);
	void clrBelowCursor(void);
	void clrBelow(String);
	void print(String);
	void println(String);
	void blinkOn(void);
	void blinkOff(void);
	void clrToEOL(void);
	
  // library-accessible "private" interface
  private:
};

#endif

