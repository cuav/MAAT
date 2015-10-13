#include "MAVLink.h"




//mavlink_system_t mavlink_system = {12,1,0,0};
/*
MAVComm::MAVComm(MAVLinkMessageHandler** handlers)
{
      set_rom = eeprom.alloc((sizeof(int) + sizeof(uint8_t))*2);
      this->handlers = handlers;
      air_port = 0;
      air_buad = 57600;
      ground_port = 3;
      ground_buad = 115200;
}*/


void MAVComm::begin(FastSerial* air, int32_t air_buad, FastSerial* ground, int32_t ground_buad)
{
    mavlink_comm_0_port = &SERIAL_AIR_PORT;
    mavlink_comm_1_port = NULL;

    //air_face = get_face(air_port);
    //ground_face = get_face(ground_port);
    mavlink_status = MAVLINK_STATUS_INACTIVE;
    last_beat = 0;

    air_face = air;
    air_face->begin(air_buad);

    ground_face = ground;
    ground_face->begin(ground_buad);
//    ground_face->begin(ground_buad);
}

void MAVComm::end()
{
	if (air_face)
		air_face->end();
	if (ground_face)
		ground_face->end();
}

/*
FastSerial* MAVComm::get_face(uint8_t port)
{
  switch (port)
  {
    case 0: return &Serial;
    case 1: return &Serial1;
    case 2: return &Serial2;
    case 3: return &Serial3;
  }
}
*/

void MAVComm::update()
{
    mavlink_message_t msg;
    mavlink_status_t status;
    char ch;


    while (air_face->available())
    {
        ch = air_face->read();

        //temporarily disable transfering for debug
#ifdef ENABLE_MAVLINK_TRANSFER
        debug("a");
        ground_face->write(ch);
#endif

        if (mavlink_parse_char(0, ch, &msg, &status))
            raise_msg_received(&msg);
        /*
        else if (status.packet_rx_drop_count > 0)
        {
          Serial.println("mav parse error");
        }*/

    }

    if (mavlink_status > MAVLINK_STATUS_WAIT_HEARTBEAT && systime - last_beat > 3000)
    {
        mavlink_status = MAVLINK_STATUS_WAIT_HEARTBEAT;
        raise_status_changed(mavlink_status);
    }
    else if (mavlink_status == MAVLINK_STATUS_REQUIRE_DATA)
    {

        debug("require data");
        for (uint8_t i = 0; i < 3; i++)
        {
            request();
            delay(50);
        }
        mavlink_status = MAVLINK_STATUS_WAIT_DATA;
        raise_status_changed(mavlink_status);
    }

    //temporarily disable transfering for debug

    while (ground_face->available())
    {
        ch = ground_face->read();
        air_face->write(ch);
    }

}

void MAVComm::request()
{
    const uint8_t MAVStreams[][2] =
    {
        //{MAV_DATA_STREAM_RAW_SENSORS, 0x02},
        {MAV_DATA_STREAM_EXTENDED_STATUS, 0x05},
        //{MAV_DATA_STREAM_RC_CHANNELS, 0x02},
        {MAV_DATA_STREAM_POSITION, 0x05},
        {MAV_DATA_STREAM_EXTRA1, 0x05}, //ATTITUDE
        {MAV_DATA_STREAM_EXTRA2, 0x05}, //HUD
        //{MAV_DATA_STREAM_RAW_CONTROLLER, 0x02},
    };
    //const uint8_t MAVRates[] = {0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05};

    for (int i=0; i < sizeof(MAVStreams)/2; i++)
    {
        mavlink_msg_request_data_stream_send(MAVLINK_COMM_0,
                                             apm_mav_system, apm_mav_component,
                                             MAVStreams[i][0], MAVStreams[i][1], 1);
    }
}

void MAVComm::message_received(mavlink_message_t* msg)
{
    //uint32_t now = millis();
    switch (msg->msgid)
    {
    case MAVLINK_MSG_ID_HEARTBEAT:
        apm_mav_system    = msg->sysid;
        apm_mav_component = msg->compid;
        if (mavlink_status < MAVLINK_STATUS_REQUIRE_DATA || systime - last_beat > 3000)
        {
            mavlink_status = MAVLINK_STATUS_REQUIRE_DATA;
            raise_status_changed(mavlink_status);
        }
//        Serial.printf("mav heart %d\n", systime);
        last_beat = systime;

        break;
//        case MAVLINK_MSG_ID_SYS_STATUS:
    default:
        if (mavlink_status == MAVLINK_STATUS_WAIT_DATA)
        {
            mavlink_status = MAVLINK_STATUS_GOT_DATA;
            raise_status_changed(mavlink_status);
        }
//        Serial.println("mav other");
        break;
    }
}

void MAVComm::set_fly_mode(uint16_t mode)
{
    debug("set fly mode %d %d %d\n", apm_mav_system, MAV_MODE_FLAG_CUSTOM_MODE_ENABLED, mode);
    mavlink_msg_set_mode_send(MAVLINK_COMM_0, apm_mav_system, MAV_MODE_FLAG_CUSTOM_MODE_ENABLED, mode);
}

void MAVComm::raise_msg_received(mavlink_message_t* msg)
{
    if (handlers)
        for (MAVLinkMessageHandler** h = handlers; *h; h++)
            (*h)->message_received(msg);

    message_received(msg);
}

void MAVComm::raise_status_changed(MAVLINK_STATUS status)
{
    if (handlers)
        for (MAVLinkMessageHandler** h = handlers; *h; h++)
            (*h)->link_status_changed(status);
}
