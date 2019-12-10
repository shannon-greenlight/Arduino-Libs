#include "Arduino.h"
#include <TerminalVT100.h>
#include <Adafruit_FRAM_I2C.h>

#include <Greenface_EEPROM.h>
#include <EEPROM_Int.h>

EEPROM_Int::EEPROM_Int(int * _ptr, int _size, int _offset) {
    ptr = _ptr;
    size = _size;
    offset = _offset;
    indx = 0;
}

void EEPROM_Int::read() {
    for(uint16_t i=0; i<size;i++) {
        ptr[i] = read_int(offset+(i*2));
    }
}

int EEPROM_Int::get(int index) {
    if(index==-1) index = indx;
    return ptr[index];
}

void EEPROM_Int::put(int val, int index) {
    if(index==-1) index = indx;
    ptr[index] = val;
    write_int(offset+(index*2), val);
}


