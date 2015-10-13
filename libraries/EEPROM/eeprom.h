#ifndef _EEPROM_H
#define _EEPROM_H

#include <FastSerial.h>
#include <avr/eeprom.h>
#include <debug.h>

#ifndef OFFSET
#define OFFSET(type, field) ((uint32_t)&(((type*)0)->field))
#endif

class EEPROM
{
public:
    EEPROM(uint8_t ver)
    {
        rom_addr = (uint8_t*)3;
        VER = ver;
    }

    void* alloc(size_t n)
    {
        uint8_t* p = rom_addr;
        rom_addr += n;
        return p;
    }

    void read(void* ram, void* rom, size_t n)
    {
        eeprom_read_block(ram, rom, n);
    }
    
    void write(void* ram, void* rom, size_t n)
    {
        eeprom_write_block(ram, rom, n);
    }
    
    
    void init()
    {
        uint8_t buf[2];
        eeprom_read_block(buf, (void*)0, 2);
        if (buf[0] != MARK || buf[1] != VER)
        {
            debug("eeprom invalid");
            valid = false;
            buf[0] = MARK;
            buf[1] = VER;
            eeprom_write_block(buf, (void*)0, 2);
        }
        else
        {
            valid = true;
        }
    }

    bool is_valid()
    {
        return valid;
    }

private:
    uint8_t* rom_addr;
    uint8_t version;
    bool valid;

    static const uint8_t MARK = 'H';
    uint8_t VER;
};


extern EEPROM eeprom;
#endif
