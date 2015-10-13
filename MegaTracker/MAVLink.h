#ifndef MAVCOMM_H
#define MAVCOMM_H
#include <FastSerial.h>
#include <GCS_MAVLink.h>
//#include <Mavlink_compat.h>


enum MAVLINK_STATUS
{
    MAVLINK_STATUS_INACTIVE,
    MAVLINK_STATUS_WAIT_HEARTBEAT,
    MAVLINK_STATUS_REQUIRE_DATA,
    MAVLINK_STATUS_WAIT_DATA,
    MAVLINK_STATUS_GOT_DATA,
    MAVLINK_STATUS_UPDATE_DATA
};

class MAVLinkMessageHandler
{
public:
    virtual void message_received(mavlink_message_t* msg) {}
    virtual void link_status_changed(MAVLINK_STATUS status) {}
};



class MAVComm
{
public:
    MAVComm(MAVLinkMessageHandler** _handlers)
        :handlers(_handlers)
    {
        set_rom = eeprom.alloc(sizeof(int)*2);
    }

    void begin(FastSerial* air, int32_t air_buad, FastSerial* ground, int32_t ground_buad);
    
    void end();

    void load()
    {
        if (eeprom.is_valid())    eeprom.read(&air_buad, set_rom, sizeof(int32_t)*2);
        else save();
    }
    void save()
    {
        eeprom.write(&air_buad, set_rom, sizeof(int32_t)*2);
    }

    //void set_air_port(int port);
    //void set_ground_port(int port);

    void set_air_buadrate(int buad)
    {
        air_buad = buad;
    }
    void set_ground_buadrate(int buad)
    {
        ground_buad = buad;
    }

    void update();
    void send(mavlink_message_t* msg);
    void request(void);
    void set_fly_mode(uint16_t mode);
    MAVLINK_STATUS get_status()
    {
        return mavlink_status;
    }
private:
    void message_received(mavlink_message_t* msg);

    void raise_msg_received(mavlink_message_t* msg);
    void raise_status_changed(MAVLINK_STATUS status);
    //FastSerial* get_face(uint8_t port);
//    uint32_t last_heart_beat;
//    bool req_mav_rate;
    //uint32_t last_ground_response;
    MAVLINK_STATUS mavlink_status;
    uint32_t last_beat;

    uint8_t apm_mav_system;
    uint8_t apm_mav_component;

    MAVLinkMessageHandler** handlers;
    FastSerial* air_face;
    FastSerial* ground_face;

    int32_t air_buad;
    int32_t ground_buad;

    void* set_rom;
};

#endif
