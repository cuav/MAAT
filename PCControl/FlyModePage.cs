using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MegaTracker
{
    class FlyModePage
    {
        public void paint(GLCD lcd)
        {
            lcd.DrawString(3, 2, "Fly Mode");
            lcd.DrawHLine(3, 12, 152);

            
            lcd.DrawString(54, 2, "(ArduPlane)");
            lcd.DrawString(9, 40, "Manual");
            lcd.DrawString(9, 50, "Stabilize");
            lcd.DrawString(9, 60, "Auto");
            lcd.DrawString(9, 70, "Guided");
            lcd.DrawString(9, 80, "Loiter");

            lcd.DrawString(81, 40, "Retn Home");
            lcd.DrawString(81, 50, "Circle");
            lcd.DrawString(81, 60, "FBW A");
            lcd.DrawString(81, 70, "FBW B");
            
            
             
            /*
            lcd.DrawString(54, 2, "(ArduCopter)");
            lcd.DrawString(9, 40, "Stabilize");
            lcd.DrawString(9, 50, "Alth Hold");
            lcd.DrawString(9, 60, "Auto");
            lcd.DrawString(9, 70, "Guided");
            lcd.DrawString(9, 80, "Loiter");
            lcd.DrawString(81, 40, "Retn Home");
            lcd.DrawString(81, 50, "Circle");
            lcd.DrawString(81, 60, "Position");
            lcd.DrawString(81, 70, "Land");
            lcd.DrawString(81, 80, "Oflo");
             */
            /*
            lcd.DrawString(9, 20, "Stab");
            lcd.DrawString(9, 30, "Acro");
            lcd.DrawString(9, 40, "Alth");
            lcd.DrawString(9, 50, "Auto");
            lcd.DrawString(9, 60, "Guid");
            lcd.DrawString(9, 70, "Loit");
            lcd.DrawString(9, 80, "Retl");*/
        }
    }
}
