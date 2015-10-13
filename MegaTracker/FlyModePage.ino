#include "FlyModePage.h"

const char _FM_STAB[] PROGMEM = "Stabilize";
const char _FM_ACRO[] PROGMEM = "Acro";
const char _FM_ALTH[] PROGMEM = "Alt Hold";
const char _FM_AUTO[] PROGMEM = "Auto";
const char _FM_GUID[] PROGMEM = "Guided";
const char _FM_LOIT[] PROGMEM = "Loiter";
const char _FM_RETL[] PROGMEM = "Ret Home";
const char _FM_CIRC[] PROGMEM = "Circle";
const char _FM_POSI[] PROGMEM = "Pos Hold";
const char _FM_OFLO[] PROGMEM = "OF Loiter";
const char _FM_MANU[] PROGMEM = "Manual";
const char _FM_FBWA[] PROGMEM = "FBW A";
const char _FM_FBWB[] PROGMEM = "FBW B";
const char _FM_FBWC[] PROGMEM = "FBW C";
const char _FM_LAND[] PROGMEM = "Land";

const prog_char* plane_modes_str[] = {_FM_AUTO, _FM_CIRC, _FM_FBWA, _FM_FBWB, /*_FM_FBWC, _FM_GUID,*/ _FM_LOIT, _FM_MANU, _FM_RETL, _FM_STAB};
const uint8_t plane_modes_code[] PROGMEM = {10, 1, 5, 6, /*7, 15,*/ 12, 0, 11, 2};

const prog_char* copter_modes_str[] = {_FM_ACRO, _FM_ALTH, _FM_AUTO, _FM_CIRC, _FM_GUID, _FM_LAND, _FM_LOIT, _FM_POSI, _FM_RETL, _FM_STAB};
const uint8_t copter_modes_code[] PROGMEM = {1, 2, 3, 7, 4, 9, 5, 8, 6, 0};

FlyModePage::FlyModePage()
    : GUIPage(PSTR("Flight Modes"))
{
    //selected_index = cur_mode_index = -1;
    cur_mode_index = -1;
    mav_type = 0xff;
    fly_mode = 0xffff;
}

void FlyModePage::on_paint(Graphics* g)
{
    uint8_t x, y;

    if (aircraft.mav_type != mav_type)
    {
        mav_type = aircraft.mav_type;
        invalid = true;
    }

    if (invalid)
    {
        GUIPage::on_paint(g);
        invalid = false;
        const prog_char** modes = 0;
        const prog_char* name = 0;

        if (aircraft.mav_type == 1)  //apm
        {
            modes = plane_modes_str;
            item_num = sizeof(plane_modes_code);
            name = PSTR("(ArduPlane)");
        }
        else
        {
            modes = copter_modes_str;
            item_num = sizeof(copter_modes_code);
            name =  PSTR("(ArduCopter)");
        }


        g->draw_string_p(78, 2, name);
        uint8_t pivot = (item_num+1)/2;

        for (uint8_t i = 0; i < pivot; i++)
        {
            g->draw_string_p(9, 40 + i * 10, modes[i]);
            delay(1);
        }
        for (uint8_t i = 0; i < item_num - pivot; i++)
        {
            g->draw_string_p(81, 40 + i * 10, modes[pivot + i]);
            delay(1);
        }

        //if (selected_index != -1)
        //    draw_mark(selected_index, PSTR(">"));
        get_cursor_pos(curr_item, &x, &y);
        gui.get_cursor()->set(x, y, CURSOR_TYPE_NAVIGATE, false);
    }

    if (invalid || aircraft.fly_mode != fly_mode/* cur_mode_index != curr_item*/)
    {
        //if (cur_mode_index != -1 && cur_mode_index != selected_index)
        //    draw_mark(cur_mode_index, PSTR(" "));

        fly_mode = aircraft.fly_mode;
        const prog_uint8_t* codes = mav_type == 1? plane_modes_code: copter_modes_code;

        get_cursor_pos(cur_mode_index, &x, &y);
        g->draw_string_p(x, y, PSTR(" "));

        for (cur_mode_index = 0; cur_mode_index < item_num ; cur_mode_index++)
        {
            if (fly_mode == pgm_read_byte_near(codes + cur_mode_index))
            {
                get_cursor_pos(cur_mode_index, &x, &y);
                g->draw_string_p(x, y, PSTR("\x81"));
                break;
            }
        }
    }
}

void FlyModePage::on_key_press(uint8_t key_code)
{
    uint8_t x, y;

    if (key_code == KEY_S2)
        gui.go_back();
    else if (key_code == KEY_S1)
    {
        if (mav_type == 1)
            mavcomm.set_fly_mode(pgm_read_byte_near(&plane_modes_code[curr_item]));
        else
            mavcomm.set_fly_mode(pgm_read_byte_near(&copter_modes_code[curr_item]));
    }
    else if (key_code == KEY_UP || key_code == KEY_DOWN)
    {
        //if (selected_index != cur_mode_index)
        //    draw_mark(selected_index, PSTR(" "));
        bool erase = curr_item != cur_mode_index;

        curr_item += key_code == KEY_DOWN? 1: -1;
        if (curr_item == cur_mode_index)
            curr_item += key_code == KEY_DOWN? 1: -1;

        if (curr_item >= item_num)
            curr_item = 0;
        else if (curr_item < 0)
            curr_item = item_num - 1;
        //draw_mark(selected_index, PSTR(">"));
        get_cursor_pos(curr_item, &x, &y);
        gui.get_cursor()->move_to(x, y, erase);
    }
    else if (key_code == KEY_LEFT || key_code == KEY_RIGHT)
    {
        uint8_t pivot = (item_num + 1)/2;
        //if (selected_index != cur_mode_index)
        //    draw_mark(selected_index, PSTR(" "));
        bool erase = curr_item != cur_mode_index;

        if (key_code == KEY_LEFT)
        {
            do
            {
                if (curr_item == 0)
                    curr_item = item_num - 1;
                else if (curr_item < pivot)
                    curr_item = (curr_item - 1) + pivot;
                else
                    curr_item -= pivot;
            }
            while (curr_item == cur_mode_index);
        }
        else
        {
            do
            {
                if (curr_item < pivot)
                    curr_item += pivot;
                else if (curr_item == item_num-1)
                    curr_item = 0;
                else
                    curr_item = curr_item + 1 - pivot;
                if (curr_item >= item_num)
                    curr_item = pivot-1;
            }
            while (curr_item == cur_mode_index);
        }
        //draw_mark(selected_index, PSTR(">"));

        get_cursor_pos(curr_item, &x, &y);
        gui.get_cursor()->move_to(x, y, erase);
    }
}

void FlyModePage::get_cursor_pos(uint8_t idx, uint8_t* x, uint8_t* y)
{
    //Graphics* g = gui.get_graphics();
    int8_t pivot = (item_num + 1)/2;
    if (idx >= pivot)	//right
    {
        *x = 81 - 6;
        *y = 40 + (idx - pivot) * 10;
    }
    else
    {
        *x = 3;
        *y = 40 + idx * 10;
    }
}

