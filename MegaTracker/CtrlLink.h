#ifndef _CTRL_LINK_H
#define _CTRL_LINK_H
#include <FastSerial.h>
#include <ctrl_link.h>

class CtrlLink: public ControlLink
{
public:
    CtrlLink():ControlLink(SendChar)
    {
        ready = false;
    }

    virtual void send_message(uint8_t cmd, void* data, uint8_t data_len);

    void update();
    void begin(FastSerial* port, int32_t buad);
    void end();
    bool ready;
protected:
    static void SendChar(uint8_t ch)
    {
        face->write(ch);
    }
    static Stream* face;
    ControlLinkMessage msg;
};
#endif

