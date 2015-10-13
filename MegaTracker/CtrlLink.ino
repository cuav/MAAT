#include "CtrlLink.h"
#include <ctrl_cmd.h>

Stream* CtrlLink::face = 0;

void CtrlLink::begin(FastSerial* port, int32_t baud)
{
    CtrlLink::face = port;
    port->begin(baud, 16, 128);
}

void CtrlLink::end()
{
    if (face)
        ((FastSerial*)face)->end();
}

void CtrlLink::send_message(uint8_t cmd, void* data, uint8_t data_len)
{
    if (ready || cmd == DEV_QUERY)
        ControlLink::send_message(cmd, data, data_len);
}

void CtrlLink::update()
{
    while (face->available())
    {
        uint8_t ch = face->read();
//      Serial.print(ch);
        if (parse_message(ch, &msg))
        {
            debug("got ctrl msg %d\r\n", msg.cmd);
            int8_t ack = 0;
            // if (msg.cmd != 128)
            //   Serial.printf("Got mssage %i\n", msg.cmd);

            switch (msg.cmd)
            {
            case SERVO_GET_SETTING:
            {
                debug("Get servo setting\r\n");
                ServoSetSettingMsgData data;
                MyServo* servo = 0;
                if (msg.data[0] == 0)
                    servo = &pan_servo;
                else if (msg.data[0] == 1)
                    servo = &tlt_servo;
                if (servo)
                {
                    data.idx = msg.data[0];
                    data.min = servo->get_min();
                    data.max = servo->get_max();
                    data.cen = servo->get_cen();
                    data.acc = servo->get_acc();
                    //data.rev = servo->rev;
                    data.dzone = servo->get_dzone();
                    data.nty = servo->get_half_pi();
                    send_message(SERVO_SET_SETTING, (uint8_t*)&data, sizeof(data));
                }
                else
                    ack = -1;
            }
            break;
            case SERVO_SET_SETTING:
            {
                debug("Set servo setting\r\n");
                ServoSetSettingMsgData* data = (ServoSetSettingMsgData*)(msg.data);
                MyServo* servo = 0;
                if (data->idx == 0)
                    servo = &pan_servo;
                else if (data->idx == 1)
                    servo = &tlt_servo;
                if (servo)
                {
                    servo->set_min(data->min);
                    servo->set_max(data->max);
                    servo->set_cen(data->cen);
                    servo->set_acc(data->acc);
                    //servo->rev = data->rev;
                    servo->set_half_pi(data->nty);
                    if (data->save)
                        servo->save();
                    ack = 1;
                }
                else
                    ack = -1;
            }
            break;
            case COMPASS_GET_DECLINATION:
            {
                debug("Get declination\r\n");
                float dec = mag.get_declination();
                send_message(COMPASS_SET_DECLINATION, (uint8_t*)&dec, sizeof(dec));
            }
            break;
            case COMPASS_SET_DECLINATION:
            {
                debug("Set declination\r\n");
                mag.set_declination(*(float*)(msg.data));
                ack = 1;
            }
            break;
            case TRACKER_STOP:
                debug("Stop tracker\r\n");
                tracker.stop();
                ack = 1;
                break;
            case TRACKER_START:
                debug("Start tracker\r\n");
                tracker.start();
                ack = 1;
                break;
            case CTRL_CALIBRATE:
                break;
            case SERVO_SET_ANGLE:
            {
                ServoSetAngleMsgData* data = (ServoSetAngleMsgData*)msg.data;
                if (data->idx == 0)
                {
                    //Serial.printf("Set pan servo %f\n", data->angle);
                    pan_servo.set_angle(data->angle);
                    ack = 1;
                }
                else if (data->idx == 1)
                {
                    // Serial.printf("Set tilt servo %f\n", data->angle);
                    tlt_servo.set_angle(data->angle);
                    ack = 1;
                }
                else
                    ack = -1;
            }
            break;
            case SERVO_SET_PWM:
            {
                ServoSetPwmMsgData* data = (ServoSetPwmMsgData*)msg.data;
                if (data->idx == 0)
                {
                    //  Serial.printf("Set pan servo pwm %d\n", data->pwm);
                    pan_servo.set_pwm(data->pwm);
                    ack=1;
                }
                else if (data->idx == 1)
                {
                    //   Serial.printf("Set tilt servo pwm %d\n", data->pwm);
                    tlt_servo.set_pwm(data->pwm);
                    ack = 1;
                }
                else
                    ack = -1;
            }
            break;
            case KEY_PRESS:
                //  Serial.printf("key press %x\n", msg.data[0]);
                debug("key press\r\n");
                //debug(msg.data[0]);
                gui.key_press(msg.data[0]);
                ack = 1;
                break;
            case DEV_READY:
                //gui.start(&osd);
                ready = true;
                break;
            case CTRL_SET_ANGLES:
                tracker.stop();
                break;
            default:
                slcd.message_received(&msg);
                break;
            }
            /*
                        if (ack == 1)
                        {
                            debug("ACK\r\n");
                            send_message(CMD_ACK, &msg.seq, 1);
                        }
                        else if (ack == -1)
                        {
                            debug("NACK\r\n");
                            send_message(CMD_NACK, &msg.seq, 1);
                        }*/
        }
    }
}

