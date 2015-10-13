#include "ctrl_link.h"


const uint8_t MARK = '#';
//const char ACK_MARK = '$';
//const char NACK_MARK = '%';


void ControlLink::timeout_tick()
{
    if (timeout < 50)
        timeout++;
    if (timeout >= 50)	//1s
        msg_state = 0;
}

bool ControlLink::parse_message(uint8_t ch, ControlLinkMessage* msg)
{
    timeout = 0;
    switch (msg_state)
    {
    case 0:
        if (ch == MARK)
        {
            msg_state++;
            checksum = 0;
            dlen = 0;
        }
        break;
    case 1:
        if (ch < MIN_MSG_LENGTH)
        {
            msg_state = 0;
        }
        else if (ch <= MAX_MSG_LENGTH)
        {
            msg->len = ch-2;
            checksum += ch;
            msg_state++;
        }
        else
        {
            msg_state = 5;
            msg->len = ch;
        }
        break;
    case 2:
        msg->seq = ch;
        checksum += ch;
        msg_state++;
        break;
    case 3:
        msg->cmd = ch;
        checksum += ch;
        msg_state++;
        break;
    case 4:
        if (dlen < msg->len)
        {
            msg->data[dlen++] = ch;
            checksum += ch;
        }
        else
        {
            msg->len += 2;
            msg_state = 0;
            return checksum == ch;
        }
        break;
    case 5:
        if (dlen++ == msg->len)
        {
            msg_state = 0;
            return false;
        }
        break;
    }

    //('0' + msg_state);
    return false;
}
/*
bool parse_message(uint8_t ch)
{
    timeout = 0;
    switch (msg_state)
    {
    case 0:
        if (ch == '#')
        {
            msg_state++;
            checksum = 0;
            dlen = 0;
        }
        break;
    case 1:
        msg->cmd = ch;
        checksum+= ch;
        msg_state++;
        break;
    case 2:
        if (ch <= 64)
        {
            msg->len = ch;
            checksum+=ch;
            msg_state++;
        }
        else
            msg_state = 0;
        break;
    case 3:
        if (dlen < msg->len)
        {
            msg->data[dlen++] = ch;
            checksum += ch;
        }
        else
        {
            msg_state = 0;
            return checksum == ch;
        }
    }

    uart_putc('0' + msg_state);
    return false;
}
*/

void ControlLink::send_message(uint8_t cmd, void* data, uint8_t data_len)
{
    send_char(MARK);
    uint8_t checksum = cmd + data_len + 2 + out_seq;
    send_char(data_len+2);
    send_char(out_seq++);
    send_char(cmd);

    for (uint8_t* p = (uint8_t*)data; p < ((uint8_t*)data) + data_len; p++)
    {
        checksum += *p;
        send_char(*p);
    }
    send_char(checksum);
}

/*
void ControlLink::send_ack()
{
	  send_char(ACK_MARK);
}

void ControlLink::send_nack()
{
	  send_char(NACK_MARK);
}*/

/*

void send_message(Message* m)
{
    uart_putc('#');
    checksum = 0;
    for (uint8_t* p = (uint8_t*)m; p < ((uint8_t*)m) + m->len+2; p++)
    {
        checksum += *p;
        uart_putc(*p);
    }
    uart_putc(checksum);
}

void on_recv_message()
{
    switch (msg->cmd)
    {
    case LCD_SEND_CMD:
        for (uint8_t i = 0; i < msg->len; i++)
            GLCD.SendCmd(msg->data[i]);
        break;
    case LCD_SEND_DATA:
        for (uint8_t i = 0; i < msg->len; i++)
            GLCD.SendData(msg->data[i]);
        break;
    case LCD_READ_REG:
        break;
    case LCD_READ_DATA:
        break;
    case LCD_SET_COLOR:
        GLCD.SetColor(msg->data[0], msg->data[1]);
        break;
    case LCD_CLEAR_SCREEN:
        GLCD.ClearScreen();
        break;
    case LCD_DRAW_HLINE:
        GLCD.DrawHLine(msg->data[0], msg->data[1], msg->data[2]);
        break;
    case LCD_DRAW_VLINE:
        GLCD.DrawVLine(msg->data[0], msg->data[1], msg->data[2]);
        break;
    case LCD_DRAW_LINE:
        GLCD.DrawLine(msg->data[0], msg->data[1], msg->data[2], msg->data[3]);
        break;
    case LCD_DRAW_RECT:
        GLCD.DrawRect(msg->data[0], msg->data[1], msg->data[2], msg->data[3]);
        break;
    case LCD_FILL_RECT:
        GLCD.FillRect(msg->data[0], msg->data[1], msg->data[2], msg->data[3]);
        break;
    case LCD_DRAW_ROUND_RECT:
        GLCD.DrawRoundRect(msg->data[0], msg->data[1], msg->data[2], msg->data[3], msg->data[4]);
        break;
    case LCD_DRAW_CIRCLE:
        GLCD.DrawCircle(msg->data[0], msg->data[1], msg->data[2]);
        break;
    case LCD_FILL_CIRCLE:
        GLCD.FillCircle(msg->data[0], msg->data[1], msg->data[2]);
        break;
    case LCD_DRAW_STRING:
        GLCD.DrawString(msg->data[0], msg->data[1], (char*)&msg->data[2], msg->len - 2);
        break;
    case KEY_STATE:
        outmsg->cmd = KEY_STATE;
        outmsg->len = 1;
        outmsg->data[0] = Keyboard.GetKeyState();
        send_message(&outmsg->;
        break;
    }
}
*/