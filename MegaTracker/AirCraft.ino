#include "AirCraft.h"
#include <eeprom.h>


AirCraft::AirCraft()
{
	
//  got_home = false;
//  lat = lon = alt = dir =
	eeprom_addr = eeprom.alloc(sizeof(bool));
    reset();
}

void AirCraft::load()
{
	eeprom.read(&use_ground_gps, eeprom_addr, sizeof(use_ground_gps));
}

void AirCraft::save()
{
	eeprom.write(&use_ground_gps, eeprom_addr, sizeof(use_ground_gps));
}

void AirCraft::reset()
{
    memset(&lat, 0, (uint8_t*)&rel_coord - (uint8_t*)&lat);
    rel_coord.x = 3.0f;
    rel_coord.y = rel_coord.z = 0;
}

void AirCraft::set_home()
{
	home_lat = lat;
	home_lon = lon;
	home_alt = alt;
	
    got_home |= 1;
    debug("got home %f %f %f\r\n", home_lat, home_lon, home_alt);
}

void AirCraft::message_received(mavlink_message_t* msg)
{
    switch(msg->msgid)
    {
    case MAVLINK_MSG_ID_HEARTBEAT:
        mav_type = mavlink_msg_heartbeat_get_type(msg);
        fly_mode = mavlink_msg_heartbeat_get_custom_mode(msg);
        sys_status = mavlink_msg_heartbeat_get_system_status(msg);
        if (sys_status == MAV_STATE_ACTIVE)
            sys_status = ((mavlink_msg_heartbeat_get_base_mode(msg) & (uint8_t)MAV_MODE_FLAG_SAFETY_ARMED))?
                         MAV_STATE_ACTIVE: MAV_STATE_STANDBY;
        break;
    case MAVLINK_MSG_ID_SYS_STATUS:
        volt = mavlink_msg_sys_status_get_voltage_battery(msg)/1000.0f;
        curr = mavlink_msg_sys_status_get_current_battery(msg)/100.0f;
        if (curr < 0.0f)
            curr = 0.0f;
    case MAVLINK_MSG_ID_GPS_RAW_INT:
        sat_fix = mavlink_msg_gps_raw_int_get_fix_type(msg);
        sat_count = mavlink_msg_gps_raw_int_get_satellites_visible(msg);
        if (sat_fix > 1)
        {
            lat = mavlink_msg_gps_raw_int_get_lat(msg)/10000000.0f;
            lon = mavlink_msg_gps_raw_int_get_lon(msg)/10000000.0f;
        }
        break;
    case MAVLINK_MSG_ID_VFR_HUD:
//        Serial.println("got heading");
        heading = mavlink_msg_vfr_hud_get_heading(msg);
        airspeed = mavlink_msg_vfr_hud_get_airspeed(msg);
        groundspeed = mavlink_msg_vfr_hud_get_groundspeed(msg);
        alt = mavlink_msg_vfr_hud_get_alt(msg);
        throttle = mavlink_msg_vfr_hud_get_throttle(msg);
        break;
    case MAVLINK_MSG_ID_ATTITUDE:
        //      Serial.println("got attitude");
        pitch =  ToDeg(mavlink_msg_attitude_get_pitch(msg));
        roll = ToDeg(mavlink_msg_attitude_get_roll(msg));
        yaw = ToDeg(mavlink_msg_attitude_get_yaw(msg));
        break;
    case MAVLINK_MSG_ID_MISSION_CURRENT:
        wp_no = mavlink_msg_mission_current_get_seq(msg);
        break;
    case MAVLINK_MSG_ID_NAV_CONTROLLER_OUTPUT:
        wp_dist = mavlink_msg_nav_controller_output_get_wp_dist(msg);
        break;
    }
}

void AirCraft::update_home()
{
    float dstlon, dstlat;
    long bearing;

	if(got_home == 0 && sat_fix > 1)
    {
        set_home();
    }
    
	if (use_ground_gps && gps->status() == GPS::GPS_OK)
	{
		home_lat = gps->latitude / 1e7f;
		home_lon = gps->longitude / 1e7f;
		home_alt = gps->altitude / 100.0f;
		
		if (mav_type != 1)
		{
			if((got_home & 0x2) == 0)
			{
				home_alt_lunch = home_alt;
				got_home |= 0x2;
			}
		
			home_alt -= home_alt_lunch;
		}
	}
	
    if(got_home && sat_fix > 1)
    {
        // shrinking factor for longitude going to poles direction
        float rads = fabs(home_lat) * 0.0174532925;
        double scaleLongDown = cos(rads);
        double scaleLongUp   = 1.0f/cos(rads);

        //DST to Home
        rel_coord.x = (lat - home_lat) * 111319.5;
        rel_coord.y = (lon - home_lon) * 111319.5 * scaleLongDown;
        rel_coord.z = home_alt - alt;


        dstlat = fabs(rel_coord.x);//fabs(home_lat - lat) * 111319.5;
        dstlon = fabs(rel_coord.y);//fabs(home_lon - lon) * 111319.5 * scaleLongDown;
        home_dist = (uint32_t)sqrt(dstlat*dstlat + dstlon*dstlon);


        //DIR to Home
        dstlon = (home_lon - lon); //OffSet_X
        dstlat = (home_lat - lat) * scaleLongUp; //OffSet Y
        bearing = 90 + (atan2(dstlat, -dstlon) * 57.295775); //absolut home direction
        if(bearing < 0) bearing += 360;//normalization
        dir = bearing;

        bearing = bearing - 180;//absolut return direction
        if(bearing < 0) bearing += 360;//normalization
        bearing = bearing - heading;//relative home direction
        if(bearing < 0) bearing += 360; //normalization
        home_dir = bearing;
//        home_dir = round((float)(bearing/360.0f) * 16.0f) + 1;//array of arrows =)
//        if(home_direction > 16) home_direction = 0;
    }

    if (start_time == 0 && throttle > 0)
    {
        start_time = millis();
        fly_time = 0;
    }
    else if (start_time > 0)
        fly_time = millis() - start_time;
}
