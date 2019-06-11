/*
  RotaryEncoder.h - RotaryEncoder library for Wiring - implementation
  Copyright (c) 2006 John Doe.  All right reserved.
*/

// include core Wiring API
#include <Arduino.h>

// include this library's description file
#include "RotaryEncoder.h"

// include description files for other libraries used (if any)
//#include "HardwareSerial.h"
//#include <TerminalVT100.h>

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

RotaryEncoder::RotaryEncoder()
{
  // initialize this instance's variables
  //t = TerminalVT100();
  //numFxns = nfxns;
  state = REST;
  setEncoderValue(0);
  
  pinMode(encoderPin1, INPUT);	// MSB - B terminal
  pinMode(encoderPin2, INPUT);	// LSB - A terminal
  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

  //Serial.begin(9600);
  //Serial.println("Rotary Encoder: "+nfxns);
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries
void RotaryEncoder::incEncoder()
{
  encoderValue++;
  encoderValue %= numFxns;
}

void RotaryEncoder::decEncoder()
{
  encoderValue--;
  if(encoderValue<0) encoderValue=numFxns-1;
}

int RotaryEncoder::getEncoderValue()
{
  return encoderValue;
}

void RotaryEncoder::setEncoderValue(int val)
{
  encoderValue=val;
}

void RotaryEncoder::aChanInt() {
	channel = "A";
	updateEncoder();
}

void RotaryEncoder::bChanInt() {
	channel = "B";
	updateEncoder();
}


void RotaryEncoder::updateEncoder() {
  t.print("");
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit - B terminal
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit - A terminal
  //int state;
  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  //debugInt("Encoded: ",encoded);
  switch(state) {
    case REST:
	  if(debug)  {
        t.setRow("10");
        t.clrBelowCursor();
	  }
      switch(encoded) {
        case 0:
          state = (channel == "A") ? CW : CCW;  // we missed first event
          break;
        case 1:
          state = CCW;
          break;
        case 2:
          state = CW;
          break;
        case 3:
          state = REST; // we missed it all
          break;
      }
      break;
    case CW:
      switch(encoded) {
        case 0:
          state = UNK;  
          break;
        case 1:
          state = UNK;
          break;
        case 2:
          state = UNK;  // 
          break;
        case 3:
          state = REST; // expected
          incEncoder();
          break;
      }
      break;
    case CCW:
      switch(encoded) {
        case 0:
          state = UNK;  
          break;
        case 1:
          state = UNK;
          break;
        case 2:
          state = UNK;  // 
          break;
        case 3:
          state = REST; // expected
          decEncoder();
          break;
      }
      break;
    case UNK:
	  //t.setRow("25");
      //t.println("Unknown state: "+String(encoded));
      if(encoded==3) state = REST;
      break;
  }
  if(debug) {
	t.setRow(String(15+encoded));
	t.clrBelowCursor();
	t.println("Encoded: "+String(encoded) + " State: "+ String(state)+ " Channel: "+channel);
  }
}

// Private Methods ////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

