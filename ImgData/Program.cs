using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace ImgData
{
    class Program
    {

        static void Main(string[] args)
        {
            Bitmap img = (Bitmap)Image.FromFile(args[0]);

            Console.WriteLine("{0}, {1},", ((img.Width + 2) / 3)*3, img.Height);

            int k = 0;
            int l = 0;
            byte data = 0;
            for (int i = 0; i < img.Height; i++)
            {
                for (int j = 0; j < ((img.Width + 2) / 3)*3; j++)
                {
                    data = (byte)(data >> 1);

                    if (j < img.Width)
                        if (Color2Gray(img.GetPixel(j, i)) < 128)
                            data |= 0x80;

                    if (++k == 8)
                    {
                        Console.Write("0x{0:x2}, ", data);
                        k = 0;
                        if (++l == 8)
                        {
                            Console.WriteLine();
                            l = 0;
                        }
                    }
                }
            }

            if (k != 0)
                Console.Write("0x{0:x2}", data >> (8 - k));
        }

        static byte Color2Gray(Color c)
        {
            return (byte)(c.R * 0.3 + c.G * 0.59 + c.B * 0.11);
        }
    }
}
