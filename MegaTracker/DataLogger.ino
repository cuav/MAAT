#include "defines.h"
#include "DataLogger.h"

bool DataLogger::begin()
{
    bool ret = false;

    pinMode(53, OUTPUT);
    pinMode(SD_SELECT, OUTPUT);
//    pinMode(SD_DETECT, INPUT_PULLUP);

    debug("init datalogger\r\n");

    /*if (digitalRead(SD_DETECT))
    {
        slcd.draw_string_p(30, 65, PSTR("SD Card Missing..."));
        debug("sd card not detected\r\n");
    }*/
    if (has_card())
    {
//        if (file)
        end();

        if (sd_inited || SD.begin(SD_SELECT))
        {
            sd_inited = true;
            debug("init sd done\r\n");
            int fid = 1;
            int nid = 1;
            file = SD.open("mtfid.dat", FILE_WRITE);
            if (file)
            {
                debug("open mtfid.dat ok\r\n");
                if ((fid = file.read()) == -1)
                {
                    debug("read mtfild error\r\n");
                    fid = 1;
                }
                if ((nid = fid + 1) >= 256)
                    nid = 1;
                debug("next id is %i\r\n", nid);
                file.seek(0);
                file.write(nid);
                file.close();
            }
            else
            {
                debug("open mtfid.dat error\r\n");
            }


            char fname[12];
            sprintf(fname, "mt%04i.log", fid);
            debug("open sd file ");
            debug(fname);

            file = SD.open(fname, FILE_WRITE);
            if (file)
            {
                debug(" ok\r\n");
            }
            else
            {
                debug(" fail\r\n");
            }
        }
    }

    return file;
}


void DataLogger::message_received(mavlink_message_t* msg)
{
    if (file)
    {
        debug("log msg %i\r\n", msg->len + 8);
        uint64_t time = systime * 1000;
        if (file.write((uint8_t*)&time, sizeof(time)) != sizeof(time))
            debug("write time error\r\n");
        if (file.write((uint8_t*)msg, msg->len + 8) != msg->len + 8)
            debug("write data error\r\n");
    }
}
