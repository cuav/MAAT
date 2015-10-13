#include <FastSerial.h>
#include <StringBuilder.h>
#include <graphics.h>
#include <label.h>
#include <checkbox.h>
#include <cursor.h>
#include <serial_lcd.h>
#include <ctrl_cmd.h>
#include <ctrl_link.h>
#include <AP_Compass_HMC5843.h>
#include <Compass.h>
#include <I2C.h>
#include <Matrix3.h>
#include <Vector3.h>
#include <rotations.h>
#include <safemath.h>
#include <_math.h>
#include <_SD.h>
#include <debug.h>
//#include <File.h>

#include <AP_InertialSensor_MPU6000_I2C.h>
#include <AP_IMU_INS.h>
#include <AHRS_DCM.h>
#include <GCS_MAVLink.h>
#include <AP_GPS.h>
//#include <Mavlink_compat.h>
#include <eeprom.h>
#include <avr/io.h>

#include "defines.h"
#include "MAVLink.h"
#include "AirCraft.h"
#include "MyServo.h"
#include "AntTracker.h"
#include "CtrlLink.h"
#include "OSDPage.h"
#include "MainMenuPage.h"
//#include "SetupPage.h"
#include "GUI.h"
#include "key.h"
#include "datalogger.h"

FastSerialPort0(Serial);
FastSerialPort1(Serial1);
FastSerialPort2(Serial2);
FastSerialPort3(Serial3);

EEPROM eeprom(EEPROM_VERISION);

AirCraft aircraft;
AP_InertialSensor_MPU6000_I2C mpu6050(0x68);
AP_IMU_INS imu(&mpu6050);
AP_Compass_HMC5843 mag;
AHRS_DCM ahrs(&imu, &mag);

MyServo pan_servo(SERVO_PORT_T3A, 500, 2500, 1500);
MyServo tlt_servo(SERVO_PORT_T3B);
AntTracker tracker(&pan_servo, &tlt_servo);
CtrlLink ctrlcomm;
SerialLCD slcd(&ctrlcomm);

OSDPage osd;
MainMenuPage menu;

GUI gui(&slcd);
DataLogger logger;


MAVLinkMessageHandler* msg_handler[] =
{
    static_cast<MAVLinkMessageHandler*>(&aircraft),
    static_cast<MAVLinkMessageHandler*>(&osd),
    static_cast<MAVLinkMessageHandler*>(&logger),
    NULL,
};


MAVComm mavcomm(msg_handler);
Key key;
GPS* gps = 0;

#if GPS_PROTOCOL ==  GPS_PROTOCOL_NMEA
	AP_GPS_NMEA _gps(&SERIAL_GPS_PORT);	
#elif GPS_PROTOCOL ==  GPS_PROTOCOL_SIRF
	AP_GPS_SIRF _gps(&SERIAL_GPS_PORT);
#elif GPS_PROTOCOL ==  GPS_PROTOCOL_UBLOX
	AP_GPS_UBLOX _gps(&SERIAL_GPS_PORT);
#elif GPS_PROTOCOL ==  GPS_PROTOCOL_MTK
	AP_GPS_MTK _gps(&SERIAL_GPS_PORT);
#elif GPS_PROTOCOL ==  GPS_PROTOCOL_MTK16
	AP_GPS_MTK16 _gps(&SERIAL_GPS_PORT);
#elif GPS_PROTOCOL ==  GPS_PROTOCOL_AUTO
	AP_GPS_Auto _gps(&SERIAL_GPS_PORT, &gps);
#else
	AP_GPS_None _gps(&SERIAL_GPS_PORT);
#endif

//SDClass card;
//File file;


//bool timer_tick = false;
uint32_t systime = 0;

void mav_delay(uint32_t ms)
{
    uint32_t t = millis();
    while (millis() < t + ms)
    {
        mavcomm.update();
        delayMicroseconds(100);
    }
}


void flash_led_1(bool on)
{
    if (on)
        PORTE |= _BV(5);
    else
        PORTE &= ~_BV(5);
}

void flash_led_2(bool on)
{
    if (on)
        PORTE |= _BV(6);
    else
        PORTE &= ~_BV(6);
}

void reset_settings()
{
//	tracker.stop();
//	tlt_servo.stop();
//	pan_servo.stop();
	
	eeprom.init();
	
	imu.reset_calibration();
	imu.save();
	
	mag.reset_calibration();
	mag.set_declination(0.0f);
	mag.save();
	
	tlt_servo.reset();
	tlt_servo.save();
	
	tracker.reset();
	tracker.save();
	
	aircraft.use_ground_gps = false;
	aircraft.save();
}

void setup()
{
//  Serial.begin(115200);
//  Serial2.begin(115200);
	DDRE |= _BV(6) | _BV(5);
//  PORTE |= _BV(6) | _BV(5);
    flash_led_1(true);
    flash_led_2(true);
    
	MyServo::init();
		
    debug("sys start\r\n");

    

    key.init();

    delay(1);
    eeprom.init();
    
    if (!eeprom.is_valid())
    	reset_settings();

//  mavcomm.load();
    mavcomm.begin(&SERIAL_AIR_PORT, SERIAL_AIR_BAUD, &SERIAL_GND_PORT, SERIAL_GND_BAUD);
    ctrlcomm.begin(&SERIAL_CTL_PORT, SERIAL_CTL_BAUD);

//  Serial.println("hello, this is a test\n");
    delay(1);
    I2c.begin();
    I2c.timeOut(5);
    // initially set a fast I2c speed, and drop it on first failures
    I2c.setSpeed(true); // set fast I2C


    debug("wait for ctrl box\r\n");
    for (int i = 0; i < 5 && !ctrlcomm.ready; i++)
    {
        ctrlcomm.send_message(DEV_QUERY, 0, 0);
        for (int j = 0; j < 200 && !ctrlcomm.ready; j++)
        {
            mavcomm.update();
            ctrlcomm.update();
            delay(5);
        }
    }
    
    flash_led_1(false);
    flash_led_2(false);

    //turn off
    uint8_t update_mode = 0;
    ctrlcomm.send_message(CTRL_UPDATE, &update_mode, sizeof(uint8_t));

    slcd.clear_screen();
    slcd.draw_logo();
    
    slcd.draw_string_p(LOGO_MSG_X, LOGO_MSG_Y, PSTR("Firmware "FIRMWARE_VERSION));
    delay(2000);
    
    slcd.draw_string_p(30, 65, PSTR("Initialize IMU..."));

    bool imu_ok = false, mag_ok = false;
    for (uint8_t i = 0; i < 3; i++)
    {
        debug("init 6050\r\n");
        if (imu_ok = mpu6050.init())
        {
            delay(100);
            debug("init 5883\n");
            if (mag_ok = mag.init())
            {
//              Serial.println("mag init done");
                mag.load();
                mag.update();
//                if (mag_ok = mag.read())
//                  break;
                break;
            }
        }
        delay(100);
    }

    if (!imu_ok || !mag_ok)
    {
        slcd.draw_string_p(30, 65, imu_ok? PSTR("Compass Error    "): PSTR("MPU6050 Error    "));
        while(1)
        {
            mavcomm.update();
        }
    }

    start_200hz_timer();

    debug("calibrate gyro\n");
    slcd.draw_string_p(30, 65, PSTR("Calibrate Gyro..."));
    imu.init(AP_IMU_INS::COLD_START, mav_delay, flash_led_1);

	debug("init gps\n");
	slcd.draw_string_p(30, 65, PSTR("Init GPS...      "));
	
	gps = &_gps;
	gps->init();

    //MyServo::init();

    //pan_servo.load();
    tlt_servo.load();


//  pan_servo.set_ninty(100);
    /*
        pan_servo.set_min(1000);
        pan_servo.set_max(2000);
        pan_servo.set_cen(1490);
        pan_servo.set_ninty(500);
        pan_servo.set_acc(200);
        pan_servo.set_dzone(35);
    */
#ifndef DISABLE_SERVO
    pan_servo.start();
    tlt_servo.start();
#endif

    tlt_servo.set_angle(0.0f);
    pan_servo.set_angle(0.0f);

    //Serial.println("init ahrs");

    ahrs.init();
    //mag.update();
    //mag.read();
    ahrs.reset(true, true);
    systime = millis();
//    pan_servo.load();
    gui.start(&osd);
    tracker.load();
	aircraft.load();
    //tracker.slow_mode(true);
    tracker.slow_mode(true, millis() + 5000);
//    tracker.start();

    slcd.draw_string_p(30, 65, PSTR("Init SD Card...   "));
    
    if (!logger.has_card())
    {
    	slcd.draw_string_p(108, 65, PSTR("No Card"));
    	delay(1000);
    }
    else if (!logger.begin())
    {
        slcd.draw_string_p(108, 65, PSTR("Error"));
        debug("init sd card error\r\n");
        delay(1000);
    }
    //init sd card

    debug("init done\r\n");
}

static uint32_t _micro = 0;
    static float rotate = 0;
    
void loop()
{
    static uint32_t fastloop_time = 0;
    static uint8_t count = 0;
    
    
    //uint32_t now = millis();
//    mag._update(now);
    mavcomm.update();
    ctrlcomm.update();
    
    gps->update();

    systime = millis();
    
//    static float pwm[] = {0.0f, -90.0f, 90.0f};

//    static int pwm_idx = 0;
//    static uint32_t pwm_change = 0;

    if (systime >= fastloop_time)
    {
        fastloop_time = systime + 10;
        gui.update();
        ahrs.update();
        tracker.update();
        
        uint32_t now = micros();
        if (_micro != 0)
        {
        	rotate += imu.get_gyro().z* (now - _micro) * 1.0e-6;
        }
        _micro = now;
        /*
       	key.update();

        if (key.is_pressed())
        {
            debug("reset key is pressed\r\n");
            logger.begin();
            aircraft.reset();
            aircraft.update_home();
            tracker.slow_mode(true, systime + 5000);
            flash_led_1(false);
            flash_led_2(false);
        }
        */

        if (++count == 10)
        {
            tenloop();
            count = 0;
        }
    }
}

void fiftyloop()
{

}

void tenloop()
{
    static int count = 0;
    static int flash_count = 0;

    aircraft.update_home();
//    count++;
    if (mag.read())
    {
        mag.calculate_heading(ahrs.get_dcm_matrix());
        mag.null_offsets();
//          Serial.printf("MAG %d %d %d\n", mag.mag_x, mag.mag_y, mag.mag_z);
        //float* off = mag.get_offsets();
        //Serial.printf("MAG OFF %.2f %.2f %.2f\n", off[0], off[1], off[2]);
        if (count++ > 300)  //10 min
        {
            debug("save mag offset\r\n");
            mag.save_offsets();
            debug("flush log\r\n");
            logger.flush();
            count = 0;
        }
    }
    else
    {
    	Serial.printf("mag read error\r\n");
    }
    
    /*Serial.printf("ROT %.2f GYRO %.4f GYRO_I %.4f ROLL %.2f PITCH %.2f HEAD %.2f MAG %.2f\n", 
        	ToDeg(rotate), imu.get_gyro().z, 
        	ahrs._omega_I.z,
        	ahrs.euler_angle[0]/10.0,
        	ahrs.euler_angle[1]/10.0,
        	ahrs.euler_angle[2]/10.0, ToDeg(mag.heading));
	*/
	
    euler_output();

//    if (aircraft.got_home)
//   {
    //     Serial.printf("%f\r\n", aircraft.home_dir);
    bool good_link = mavcomm.get_status() >= MAVLINK_STATUS_GOT_DATA;

    flash_led_2(good_link);

    if (good_link && aircraft.sat_fix > 1)
        flash_led_1(true);
    else if (aircraft.got_home)
    {
        //debug("lost gps\r\n");
        if (flash_count == 0)
            flash_led_1(true);
        else if (flash_count == 2)
            flash_led_1(false);

        if (++flash_count >= 4)
            flash_count = 0;
    }
    else
    	flash_led_1(false);
}



void euler_output()
{

//  Serial.printf("%d %d %d\n", mag.mag_x, mag.mag_y, mag.mag_z);
//    Serial.printf("!ANG:%f,%f,%f,%f,%f\n", ahrs.euler_angle[0]/10.0, ahrs.euler_angle[1]/10.0, ahrs.euler_angle[2]/10.0, ToDeg(mag.heading), tracker.pan);
//    Serial.printf("!MAG:%i,%i,%i\n", mag.mag_x, mag.mag_y, mag.mag_z);
//  float g[3];
//    mpu6050.get_gyros(g);
//   Serial.printf("GYRO: %f, %f, %f\n",g[0], g[1], g[2]);
}


void start_200hz_timer()
{
    //timer 1, 200Hz, CTC,
    TCNT1 = 0;
    TCCR1A = 0;
    TCCR1B = _BV(WGM12) | _BV(CS11) | _BV(CS10); // clk/64,
    OCR1A = 1250;
    //ICR1 = 625;
    TIMSK1 = _BV(OCIE1A);
//    sei();
}

ISR(TIMER1_COMPA_vect)
{
    static uint8_t count = 0;
    static bool busy = false;

    if (!busy)
    {
        //flash_led_1(true);
        busy = true;
        sei();
        mpu6050.read();
        if (count++ == 4)
        {
            // Serial.println("update mag");
            mag.update();
            count = 0;
        }
        busy = false;
        //flash_led_1(false);
    }
    //timer_tick = true;
}



