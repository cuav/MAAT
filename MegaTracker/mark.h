#ifndef MARK_H
#define MARK_H

#include <avr/pgmspace.h>

class Mark
{
public:
    static const prog_char* Select;
    static const prog_char* Focus;
    static const prog_char* Blank;
};

#endif //MARK_H