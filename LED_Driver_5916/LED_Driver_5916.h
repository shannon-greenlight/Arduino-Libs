/*
  LED_Driver_5916.h - LED_Driver_5916 library 
  Copyright (c) 2019 Shannon Vance.  All right reserved.
*/

// ensure this library description is only included once
#ifndef LED_Driver_5916_h
#define LED_Driver_5916_h

// include types & constants of Wiring core API
#include "Arduino.h"

// library interface description
class LED_Driver_5916
{
  // user-accessible "public" interface
  public:
    LED_Driver_5916(void);
    void LED_write(word);
    void set_LED(int);
	int oePin;
	int lePin;
	int dataPin;
	int clkPin;

  // library-accessible "private" interface
  private:
    void doSomethingSecret(void);
	void setPin(int,bool);
	void setOE(bool);
	void setLE(bool);
	void setData(bool);
	void pulseCLK(void);
};

#endif

