#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

class DataLogger: public MAVLinkMessageHandler
{
public:
    DataLogger()
    {
        sd_inited = false;
    }

    virtual void message_received(mavlink_message_t* msg);
    //virtual void link_status_changed(MAVLINK_STATUS status) {}

//      bool init();
    bool begin();
    
    void end()
    {
        file.close();
    }
    void flush()
    {
        if(file)file.flush();
    }
    
    bool is_logging()
    {
        return (file);
    }
    
    bool has_card(){return !digitalRead(SD_DETECT);}
//private:
//      uint8_t get_next_id();
private:
    File file;
    bool sd_inited;
//      uint8_t nid;
};

#endif
