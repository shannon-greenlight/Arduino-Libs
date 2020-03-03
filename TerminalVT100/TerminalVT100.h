/*
  TerminalVT100 -  library 
  Copyright (c) 2019 Shannon Vance.  All right reserved.
*/

// ensure this library description is only included once
#ifndef TerminalVT100_h
#define TerminalVT100_h

// include types & constants of Wiring core API
#include "Arduino.h"

#define FXN_ROW "6"
#define PULSE_LEN_ROW "7"
#define DECAY_ROW "8"
#define RANDOM_ROW "9"
#define SEQUENCE_ROW "8"
#define SEQ_INDEX_ROW "9"
#define USER_VAR_ROW 10
#define TRIGGER_ROW "20"
#define DEBUG_ROW "21"
#define INPUT_ROW "21"
#define MSG_ROW "21"

// library interface description
class TerminalVT100
{
  // user-accessible "public" interface
  public:
    TerminalVT100(void);
	void begin(void);
	void setCursor(String, String);
	void setRow(String);
	void clrScreen(void);
	void clrBelowCursor(void);
	void clrDown(String);
	void print(String);
	void println(String);
	void printChars(int, String);
	void printTitle(int, String);
	void blinkOn(void);
	void blinkOff(void);
	void clrToEOL(void);
	void printVal(String, String, String);
	
  // library-accessible "private" interface
  private:
};

#endif

