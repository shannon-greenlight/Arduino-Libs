// ensure this library description is only included once
#ifndef EEPROM_Int_h
#define EEPROM_Int_h

// include types & constants of Wiring core API
#include "Arduino.h"
#include <TerminalVT100.h>
#include <Greenface_EEPROM.h>

// library interface description
class EEPROM_Int: public Greenface_EEPROM
{
    // user-accessible "public" interface
public:
    EEPROM_Int(int * _ptr, int _size, int _offset);
    //static Greenface_EEPROM gfram;
    int * ptr;
    int indx,size,offset;
    uint16_t length() { return size; }
    void read(); // xfer from eeprom to array
    int get(int index=-1);
    void put(int val, int index=-1);
    //boolean update(int framAddr, uint8_t val);
    //void write(int framAddr, uint8_t val);
    //void dump(int start, int len);

    // library-accessible "private" interface
private:
};
#endif
