using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace MegaTracker
{
    class ServoPage
    {
        public void paint(GLCD lcd)
        {
            /*
            lcd.DrawString(3, 2, "Servo Setup");
            lcd.DrawHLine(3, 12, 152);

            lcd.DrawString(9, 20, "Pan");
            //lcd.DrawHLine(9, 23, 30);
            lcd.DrawString(15, 30, "Cen");
            //lcd.DrawRectange(40, 31, 105, 5);
            draw_processbar(lcd, 40, 31, 105, 5, 2200, 800, 1500);
            lcd.DrawString(15, 40, "Max");
            //lcd.DrawRectange(40, 41, 105, 5);
            draw_processbar(lcd, 40, 41, 105, 5, 2200, 800, 2000);

            lcd.DrawString(9, 60, "Tilt");
            lcd.DrawString(15, 60 + 10, "Cen");
            //lcd.DrawRectange(40, 71, 105, 5);
            draw_processbar(lcd, 40, 71, 105, 5, 1800, 1200, 1500);

            lcd.DrawString(15, 60 + 10 * 2, "Max");
            //lcd.DrawRectange(40, 60 + 10 * 2 + 1, 105, 5);
            draw_processbar(lcd, 40, 81, 105, 5, 2200, 800, 2000);

            lcd.DrawString(15, 60 + 10 * 3, "Min");
            //lcd.DrawRectange(40, 60 + 10 * 3 + 1, 105, 5);
            draw_processbar(lcd, 40, 91, 105, 5, 2200, 800, 1000);
            lcd.DrawString(15, 60 + 10 * 4, "Rev");
            lcd.DrawRectange(40, 60 + 10 * 4 + 1, 105, 5);
            lcd.DrawString(15, 60 + 10 * 5, "90");
            //lcd.DrawRectange(40, 60 + 10 * 5 + 1, 105, 5);
            draw_processbar(lcd, 40, 111, 105, 5, 2200, 800, 2000);
             */

            lcd.DrawString(3, 2, "Setup");
            lcd.DrawHLine(3, 12, 152);

            lcd.DrawString(3, 20, "Tilt Servo");
            lcd.DrawString(15, 30, "Cen");
            lcd.DrawString(39, 30, "+150");
            lcd.DrawString(81, 30, "90");
            lcd.DrawChar(93, 30, 0x80);
            lcd.DrawString(105, 30, "+150");

            lcd.DrawString(15, 40, "Min");
            lcd.DrawString(39, 40, "-100");
            lcd.DrawString(81, 40, "Max");
            lcd.DrawString(105, 40, "-200");
            
            //lcd.DrawString(60, 40, "Rev");
            //lcd.DrawRectange(81, 41, 5, 5);

            lcd.DrawString(3, 55, "Compass");
            lcd.DrawString(15, 65, "Declination -175");
            lcd.DrawChar(15 + 6*16, 65, 0x80);
            lcd.DrawString(15 + 6*17, 65, "30'");
            lcd.DrawString(15, 75, "Calibrate");
            lcd.DrawRectange(87, 76, 5, 5);

            lcd.DrawString(3, 90, "Accel.");
            lcd.DrawString(15, 100, "Calibrate");
            lcd.DrawRectange(87, 101, 5, 5);
            //lcd.DrawString(60, 30, "Max 2200");
        }

        public void draw_processbar(GLCD lcd, int x, int y, int width, int height, int max, int min, int value)
        {
            lcd.DrawRectange(x, y, width, height);
            lcd.FillRectange(x, y, width * (value-min) / (max - min), height, Color.White);
        }
    }
}
