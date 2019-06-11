/*
  LED_Driver_5916.h - LED_Driver_5916 library for Wiring - implementation
  Copyright (c) 2006 John Doe.  All right reserved.
*/

// include core Wiring API
#include <Arduino.h>

// include this library's description file
#include "LED_Driver_5916.h"

// include description files for other libraries used (if any)
#include "HardwareSerial.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances

LED_Driver_5916::LED_Driver_5916()
{
  // initialize this instance's variables
  oePin = 5;
  lePin = 8;
  dataPin = 9;
  clkPin = 7;
  
  // do whatever is required to initialize the library
  pinMode(oePin, OUTPUT);
  pinMode(lePin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clkPin, OUTPUT);
  setOE(false);
  setLE(false);
  //Serial.begin(9600);
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in Wiring sketches, this library, and other libraries

void LED_Driver_5916::set_LED(int led) {
	word w = 32768 >> led;
	LED_write(w);
}

void LED_Driver_5916::LED_write(word leds)
{
	
	int i;
	setOE(false);
	setLE(false);
	for(i=0;i<16;i++) {
		byte b = leds & 0x0001;
		setData(b);
		delay(1);
		pulseCLK();
		delay(1);
		leds = leds>>1;
		//Serial.println(b);
	}
	// latch shifted data into output register
	setLE(true);
	delay(2);
	setLE(false);
	setOE(true);	// enable outputs
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

void LED_Driver_5916::setPin(int pin, bool val) {
	if(val) {
		digitalWrite(pin, HIGH);
	} else {
		digitalWrite(pin, LOW);
	}
}

void LED_Driver_5916::setOE(bool oe) {
	setPin(oePin,!oe);	// oe is active low
}

void LED_Driver_5916::setLE(bool le) {
	setPin(lePin,le);
}

void LED_Driver_5916::setData(bool dat) {
	setPin(dataPin,dat);
}

void LED_Driver_5916::pulseCLK(void) {
	setPin(clkPin,true);
	//delay(1);
	setPin(clkPin,false);
}

