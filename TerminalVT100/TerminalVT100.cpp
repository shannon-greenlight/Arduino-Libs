/*
  TerminalVT100.h - TerminalVT100 library for Wiring - implementation
  Copyright (c) 2006 John Doe.  All right reserved.
*/

// include core Wiring API
#include <Arduino.h>

// include this library's description file
#include "TerminalVT100.h"

// include description files for other libraries used (if any)
//#include "HardwareSerial.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

TerminalVT100::TerminalVT100()
{
  // initialize this instance's variables

  Serial.begin(9600);
  // Use Putty to communicate (VT100 terminal)
  Serial.print("\eSP F");  // tell to use 7-bit control codes
  Serial.print("\e[?25l"); // hide cursor
  Serial.print("\e[?12l"); // disable cursor highlighting
  clrScreen();
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries
void TerminalVT100::setCursor(String row, String col)
{
  Serial.print("\e["+row+";"+col+"H");
  Serial.print("\e[2K");
}

void TerminalVT100::setRow(String row)
{
  Serial.print("\e["+row+";1H");
  Serial.print("\e[2K");
}

void TerminalVT100::clrScreen()
{
  Serial.print("\e[2J"); // clear screen
}

void TerminalVT100::clrBelowCursor()
{
  Serial.print("\e[J"); // clear screen
}

void TerminalVT100::clrBelow(String row)
{
  setRow(row);
  Serial.print("\e[J"); // clear screen
}

void TerminalVT100::print(String s)
{
  Serial.print(s);
}

void TerminalVT100::println(String s)
{
  Serial.println(s);
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

