#ifndef STRING_BUFFER_H
#define STRING_BUFFER_H

#include "BetterStream.h"

#define MAX_BUF		64

class StringBuilder: public BetterStream
{
public:
	StringBuilder()
	{
		idx = 0;
	}
		
    virtual size_t write(uint8_t ch)
    {
        if (idx <MAX_BUF)
        {
            buf[idx++] = ch;
            return 1;
        }
        else
            return 0;
    };

    
    operator const char* ()
    {
    	buf[idx] = 0;
        return (const char*)buf;
    }
    
    uint8_t length() {return idx;}

    void clear()
    {
    	idx = 0;
    }

    virtual int available(void)
    {
        return 0;
    }
//  virtual int txspace(void);
    virtual int read(void)
    {
        return 0;
    }
    virtual int peek(void)
    {
        return 0;
    }
    virtual void flush(void) {}
private:
    uint8_t	buf[MAX_BUF];
    uint8_t idx;
};

#endif

