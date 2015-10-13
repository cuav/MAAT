#ifndef AIR_CRAFT_H
#define AIR_CRAFT_H
#include <FastSerial.h>
#include <GCS_MAVLink.h>
//#include <EEPROM
//#include <Mavlink_compat.h>

class AirCraft: public MAVLinkMessageHandler
{
public:
    AirCraft();
    void load();
    void save();
    
    void message_received(mavlink_message_t* msg);
    void update_home();
    void reset();
    void set_home();


    float lat, lon, alt, dir;

    uint8_t got_home;
    
    
    float home_lat, home_lon, home_alt, home_alt_lunch;
    uint16_t home_dist;
    float home_dir;

    uint8_t sat_fix, sat_count;

    int8_t pitch, roll, yaw;
    uint16_t throttle;
    float heading, groundspeed, airspeed;

    float volt, curr;
    uint8_t sys_status;
    uint8_t  mav_type;
    uint16_t fly_mode;
    uint16_t wp_no;
    uint16_t wp_dist;
    uint32_t start_time;
    uint32_t fly_time;

    Vector3f rel_coord;
    
    bool use_ground_gps;
    void* eeprom_addr;
};

extern AirCraft aircraft;

#endif
