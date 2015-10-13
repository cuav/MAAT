using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace MegaTracker
{
    class OSDPage
    {
        float course = 300f;
        float pitch = 0f;
        float roll = 0f;

        float lat = -35.020939f;
        float lon = 117.883415f;

        float aspd = 100f;
        float gspd = 90f;

        float dis = 5000;
        float alt = 200;

        float thr = 0.2f;

        float vol = 12.4f;
        float amp = 20.2f;


        public void paint(GLCD lcd)
        {
            draw_course(lcd);
            draw_horizon(lcd);

            lcd.DrawString(3, 2, "Spd");
            lcd.DrawString(24, 2, string.Format("{0:f0}", aspd));
            lcd.DrawString(3, 10, "Thr");
            lcd.DrawString(24, 10, string.Format("{0:p0}", thr));
            //lcd.DrawString(3, 10, "GS");
            //lcd.DrawString(24, 10, string.Format("{0:f0}", gspd));

            lcd.DrawString(54, 2, "Dis");
            lcd.DrawString(54, 10, "Alt");
            lcd.DrawString(75, 2, string.Format("{0:f0}", dis));
            lcd.DrawString(75, 10, string.Format("{0:f0}", alt));
            lcd.DrawString(3, 20/*127 - 8*/, DateTime.Now.ToString("mm:ss"));

            lcd.DrawString(111, 2, "Vol");
            lcd.DrawString(111, 10, "Amp");

            lcd.DrawString(132, 2, string.Format("{0:f1}", vol));
            lcd.DrawString(132, 10, string.Format("{0:f1}", amp));



            lcd.DrawString(3, 127 - 8 * 2, string.Format("{0:f6}", lat));
            lcd.DrawString(3, 127 - 8, string.Format("{0:f6}", lon));

            lcd.DrawChar(81, 127 - 8 * 2, 0xf0);
            lcd.DrawString(96, 127 - 8 * 2, "6");
            lcd.DrawString(96, 127 - 8, "2D");

            lcd.DrawString(120, 127 - 8 * 2, "Auto");
            lcd.DrawString(126, 127 - 8, "360>2");
            lcd.FillRectange(54, 57, 53, 9, Color.Blue);
            lcd.DrawString(57, 58, "Disarmed");
        }

        void draw_course(GLCD lcd)
        {
            //lcd.DrawVParallel(7, 90, 2, 18, );


            lcd.DrawVLine(79, 81, 2);
            int num = (360 - (int)course) % 45;
            num = (num >= 35 || num <= 10) ? 3 : 2;
            lcd.DrawVParallel(24 + (370 - (int)course) % 45, 85, 1, 45, num);
            lcd.DrawVParallel(24 + (360 - (int)course) % 5, 87, 2, 5, 23);

            //lcd.DrawLine(0, 15, 158, 15);
            string[] name = { "NW", "N", "NE", "E", "SE", "S", "SW", "W" };
            for (int i = 0; i < num; i++)
            {
                lcd.DrawString(24 + (370 - (int)course) % 45 + i * 45 - 3, 93, name[(((int)course + 35) / 45 + i) % 8]);
            }
            //lcd.DrawString(78, 90, "N");
            //lcd.DrawString(27, 90, "NW");
            //lcd.DrawString(120, 90, "NE");
        }

        void draw_horizon(GLCD lcd)
        {
            //pitch = 4;
            //roll = -38;
            //lcd.DrawVParallel(
            lcd.DrawHParallel(28, 40, 2, 5, 9);
            lcd.DrawHParallel(27, 40, 3, 10, 5);
            lcd.DrawHParallel(129, 40, 2, 5, 9);
            lcd.DrawHParallel(129, 40, 3, 10, 5);

            lcd.DrawString(72, 30, string.Format("{0:f0}", roll));
            lcd.DrawString(135, 58, string.Format("{0:f0}", pitch));

            // lcd.DrawLine(33, 58, 125, 58);

            int right = 92;
            int top = 40;

            /*if (roll > 90)
                roll -= 180;
            else*/ if (roll == 90)
                roll = 89.99f;
            else if (roll == -90)
                roll = -89.99f;
            /*else if (roll < -90)
                roll += 180;
            */
                   

            roll *= (float)(Math.PI / 180);

            int x1 = 0;
            int y1 = (int)(20 - 46 * Math.Tan(roll) - pitch / 2);

            if (y1 < 0)
            {
                y1 = 0;
                x1 = (int)(-(20 - pitch / 2) / Math.Tan(roll) + 46);
            }
            else if (y1 > 40)
            {
                y1 = 40;
                x1 = (int)((20 + pitch / 2) / Math.Tan(roll) + 46);
            }


            int x2 = 92;
            int y2 = (int)(20 - pitch / 2 + 46 * Math.Tan(roll));

            if (y2 < 0)
            {
                y2 = 0;
                x2 = (int)(-(20 - pitch / 2) / Math.Tan(roll) + 46);
            }
            else if (y2 > 40)
            {
                y2 = 40;
                x2 = (int)((20 + pitch / 2) / Math.Tan(roll) + 46);
            }
            lcd.FillRectange(33, 40, 96, 41, Color.Blue);
            lcd.DrawLine(x1 + 33, -y1 + 80, x2 + 33, -y2 + 80);
        }
    }
}
