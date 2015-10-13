#ifndef _DEFINES_H
#define _DEFINES_H

#define SERIAL_AIR_PORT   	Serial3
#define SERIAL_AIR_BAUD   	57600
//57600

#define SERIAL_GND_PORT   	Serial
#define SERIAL_GND_BAUD   	115200

#define SERIAL_CTL_PORT   	Serial1
#define SERIAL_CTL_BAUD   	115200

#define SERIAL_GPS_PORT	  	Serial2
#define SERIAL_GPS_BAUD	  	115200

#define SD_SELECT          	28
#define SD_DETECT          	33

#define EEPROM_VERISION		6
#define FIRMWARE_VERSION	"1.00"

// GPS type codes - use the names, not the numbers
#define GPS_PROTOCOL_NONE       -1
#define GPS_PROTOCOL_NMEA       0
#define GPS_PROTOCOL_SIRF       1
#define GPS_PROTOCOL_UBLOX      2
#define GPS_PROTOCOL_IMU        3
#define GPS_PROTOCOL_MTK        4
#define GPS_PROTOCOL_HIL        5
#define GPS_PROTOCOL_MTK16      6
#define GPS_PROTOCOL_AUTO       7

#define GPS_PROTOCOL  GPS_PROTOCOL_AUTO



#ifndef _BV
#define _BV(bit)			(1<<bit)
#endif

#ifndef sbi
#define sbi(x, bit)			(x |= (1<<bit))
#endif

#ifndef	cbi
#define cbi(x, bit) 		(x &= ~(1<<bit))
#endif

#ifndef sbl
#define sbl(x, bit) 		(x |= (1UL << bit))
#endif

#ifndef cbl
#define cbl(x, bit) 		(x &= ~(1UL << bit))
#endif

#define LOGO_MSG_X			30
#define LOGO_MSG_Y			65

#define ENABLE_SERVO

//#define ENABLE_MAVLINK_TRANSFER

#endif //_DEFINES_H

