#include "AntTracker.h"
#include "MyServo.h"

void AntTracker::update()
{
    if (run /*&& aircraft.got_home*/)
    {
        float pan_angle, tilt_angle;
        uint32_t now = micros();

        uint32_t diff_time = now - last_update;
        last_update = now;

        //float rotate_rate = 0.0f;

        //flash_led_2(true);

        Vector3f vb = ahrs.get_dcm_matrix().mul_transpose(aircraft.rel_coord);

        float sdis = vb.x * vb.x + vb.y * vb.y;

        if (sdis > 10.0f || mavcomm.get_status() == MAVLINK_STATUS_INACTIVE)
        {
            pan_angle = atan2(vb.y, vb.x);
            tilt_angle = atan2(-vb.z, sqrt(sdis));
        }
        else if (mavcomm.get_status() >= MAVLINK_STATUS_GOT_DATA)
        {
            debug(".");
            pan_angle = ToRad(aircraft.heading - ahrs.euler_angle[2]/10.0f);

            while (pan_angle < -PI)
                pan_angle += D_PI;
            while (pan_angle > PI)
                pan_angle -= D_PI;
        }
        else
        {
            pan_angle = 0;
        }
        
        //curve
        
        //float fa = fabs(pan_angle);
        //if (pan_angle > PI)
        //	pan_angle = PI + 0.3*(pan_angle - PI);
        if (pan_angle > PI/2)
        	pan_angle = 0.425 * PI + 0.5*(pan_angle - PI/2);
        else if (pan_angle > PI/4)
        	pan_angle = PI/4 + 0.7*(pan_angle - PI/4);
        else if (pan_angle < -PI/4)
        	pan_angle = -PI/4 + 0.7*(pan_angle + PI/4);
        else if (pan_angle < -PI/2)
        	pan_angle = -0.425 * PI + 0.5*(pan_angle + PI/2); 
      	
        
		//end curve
        float spd = constrain(pid_spd.update(pan_angle, diff_time), -D_PI, D_PI);

		if (fabs(spd) < 0.01f)
			spd = 0.0f;

        if (slow)
        {
            if (slow_timeout > systime || fabs(spd) < 0.05f)
                slow = false;
            else if (fabs(spd) > 0.25f)
                spd *= 0.2f;
        }


        float real_spd = ahrs.imu->get_gyro().z;
        //float gz = mpu6050.gz();
        //Serial.printf("%.2f,%.2f\r\n", gz, real_spd);
		float fpwm = pid_acc.update(spd - real_spd, diff_time);
		int16_t ipwm = 0;
		if (fpwm > 0)
			ipwm = ceil(fpwm);
		else
			ipwm = floor(fpwm);
			
        pan_pwm = constrain(pan_pwm + ipwm, MyServo::Minim, MyServo::Maxim);

        sp->set_pwm(pan_pwm);
        st->set_angle(tilt_angle);


        //Serial.printf("%d\t%.2f\t%.2f\t%.2f", aircraft.heading, ahrs.euler_angle[2]/10.0f, rotate_rate, real_rotate);
        
        
        static int count = 0;

        count++;
        if (count >= 15)
        {
            //    debug("pan\t%.2f\r\n", pan);
            //Serial.printf("pan\t%.2f\r\n", pan);
            //    count = 0;
            int aheading = aircraft.heading * 10;
            if (aheading > 1800)
            	aheading -= 3600;
            else if (aheading < -1800)
            	aheading += 3600;

			int diff = aheading - ahrs.euler_angle[2];
			if (diff > 1800)
				diff -= 3600;
			else if (diff < -1800)
				diff += 3600;	            	
            		
           /* Serial.printf("$1;1;;%d;%d;%d;%d;%d;%d;0\r\n",
                          aheading,
                          (int)(ahrs.euler_angle[2]),
                          (int)(spd * 100),
                          (int)(real_spd * 100),
                          pan_pwm,
                          diff);
            */              
            count = 0;
        }
		
        //flash_led_2(false);
    }
}



