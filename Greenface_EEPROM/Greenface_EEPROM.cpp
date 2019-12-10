#include "Arduino.h"
#include <TerminalVT100.h>
#include <Adafruit_FRAM_I2C.h>

#include <Greenface_EEPROM.h>

Greenface_EEPROM::Greenface_EEPROM() {
    fram = Adafruit_FRAM_I2C();
    framAddr = 0;
}

uint8_t Greenface_EEPROM::read(int framAddr) {
    return fram.read8(framAddr);
}

void Greenface_EEPROM::write(int framAddr, uint8_t value) {
	//Serial.println("Writing byte: "+String(value) + " to: " + String(framAddr));
    fram.write8(framAddr,value);
}

boolean Greenface_EEPROM::update(int framAddr, uint8_t val) {
    uint8_t existing = read(framAddr);
    boolean different = existing != val;
    if(different) {
        write(framAddr, val);
    }
    return different;
}

int Greenface_EEPROM::read_int(int framAddr) {
    return (int)(read(framAddr++) | read(framAddr)<<8);
}

void Greenface_EEPROM::write_int(int framAddr, int val) {
    write(framAddr++,val & 0xff);
    write(framAddr, val >> 8);
}

boolean Greenface_EEPROM::update_int(int framAddr, int val) {
    int existing = read_int(framAddr);
    boolean different = existing != val;
    if(different) {
        write(framAddr, val);
    }
    return different;
}

void Greenface_EEPROM::get(int framAddr, uint8_t* ptr, uint16_t num_bytes) {
    for(uint16_t i=0; i<num_bytes;i++) {
        *ptr++ = read(framAddr+i);
		//Serial.print(i);
    }
}

void Greenface_EEPROM::put(int framAddr, uint8_t* ptr, uint16_t num_bytes) {
    for(uint16_t i=0; i<num_bytes;i++) {
        write(framAddr+i, *ptr++);
		//Serial.print(i);
    }
}

uint16_t Greenface_EEPROM::begin() {
	if (fram.begin())
	{ // you can stick the new i2c addr in here, e.g. begin(0x51);
		Serial.println("Found I2C FRAM");
	}
	else
	{
		Serial.println("I2C FRAM not identified ... check your connections?\r\n");
		Serial.println("Will continue in case this processor doesn't support repeated start\r\n");
	}
	// Read the first byte
	//uint8_t test = fram.read8(0x0);
	int test = read_int(0x0);
	Serial.print("Restarted ");
	Serial.print(test);
	Serial.println(" times");
	// Test write ++
	//fram.write8(0x0, test + 1);
	write_int(0x0, test + 1);
    return 0;
}

void Greenface_EEPROM::dump(int start=0, int len=64) {
	// dump the memory
	uint8_t value;
	for (uint16_t a = start; a < start+len; a++)
	{
		value = fram.read8(a);
		if ((a % 32) == 0)
		{
			Serial.print("\n\r 0x");
			Serial.print(a, HEX);
			Serial.print(": ");
		}
		Serial.print("0x");
		if (value < 0x1)
			Serial.print('0');
		Serial.print(value, HEX);
		Serial.print(" ");
	}
}

