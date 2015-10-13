#ifndef CTRL_MSG_H
#define CTRL_MSG_H

#include <stdint.h>

#define MAX_DATA_LENGTH		64
#define MAX_MSG_LENGTH		66
#define MIN_MSG_LENGTH		2

struct ControlLinkMessage
{
    uint8_t len;
    uint8_t seq;
    uint8_t cmd;
    uint8_t data[MAX_DATA_LENGTH];
};


typedef void (*SendCharFunc)(uint8_t ch);

class ControlLinkMessageHandler
{
	virtual void message_received(ControlLinkMessage* msg) = 0;
};

class ControlLink
{
public:
    ControlLink(SendCharFunc func):msg_state(0), checksum(0), dlen(0), timeout(0), send_char(func) {}
    bool parse_message(uint8_t ch, ControlLinkMessage* msg);
    virtual void send_message(uint8_t cmd, void* data = 0, uint8_t data_len = 0);

    void timeout_tick();
private:
    volatile uint8_t msg_state;
    uint8_t checksum;
    uint8_t dlen;
    volatile  uint8_t timeout;
    SendCharFunc send_char;
    uint8_t out_seq;
};

/*
bool parse_ctrl_link_message(uint8_t ch);
void on_recv_ctrl_link_message();
void send_ctrl_link_message(Message* m);
void ctrl_link_msg_timeout();



extern Message msg;
extern Message outmsg;
*/

#endif