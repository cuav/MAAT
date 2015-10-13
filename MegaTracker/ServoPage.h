#ifndef SERVO_PAGE_H
#define SERVO_PAGE_H

class ServoPage: public GUIPage
{
public:
    ServoPage();
    virtual void on_paint(Graphics* g);
    virtual void on_key_press(uint8_t key_code);
//    virtual void invalidate();
    virtual void on_load();
    virtual void on_exit();

private:
    bool draw_item_value(Graphics* g, uint8_t item, float value, uint8_t left, uint8_t top);
    bool draw_item_value(Graphics* g, uint8_t item, int16_t value, uint8_t left, uint8_t top);

    void adjust_value(uint8_t key_code);
    //int8_t mode;
    //int8_t selected_item;
//    int8_t focus_item;
    uint16_t invalid_item;
    Label l_tmin, l_tmax, l_tcen, l_t90,
          /*l_pauto, l_pmin, l_pmax, l_pcen, l_pacc,*/l_sp, l_si, l_sd, l_ap, l_ai, l_ad;
    //CheckBox cb_pauto;
//    uint32_t update_time;
};

#endif
