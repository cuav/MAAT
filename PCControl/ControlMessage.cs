using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace MegaTracker
{

    public class ControlMessage
    {
        public byte len;
        public byte seq;
        public byte cmd;
        public byte[] data = new byte[MaxDataLength];

        public const int MaxDataLength = 64;
        public const int MAX_MSG_LENGTH = 66;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct SetServoParamMsgData
    {
        public byte idx;
        public short min;
        public short max;
        public short cen;
        public short nty;
        public short acc;
        public short dzone;
        public byte save;
    };

    struct GetServoParamMsgData
    {
        public byte idx;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct SetServoAngleMsgData
    {
        public byte idx;
        public float angle;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct SetServoPwmMsgData
    {
        public byte idx;
        public short pwm;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct DrawLineMsgData
    {
        public byte x1;
        public byte y1;
        public byte x2;
        public byte y2;
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct DrawHLineMsgData
    {
        public byte x;
        public byte y;
        public byte width;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct DrawVLineMsgData
    {
        public byte x;
        public byte y;
        public byte height;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct DrawRectMsgData
    {
        public byte x;
        public byte y;
        public byte width;
        public byte height;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct FillRectMsgData
    {
        public byte x;
        public byte y;
        public byte width;
        public byte height;
        public byte color;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public unsafe struct DrawStringMsgData
    {
        public byte x;
        public byte y;
        public fixed byte str[62];
        //[MarshalAs(UnmanagedType.ByValTStr, SizeConst = 62)]
        //public string str;
    };



    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    struct DrawHParallelMsgData
    {
        public byte x;
        public byte y;
        public byte width;
        public byte interval;
        public byte count;
    };

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    struct DrawVParallelMsgData
    {
        public byte x;
        public byte y;
        public byte height;
        public byte interval;
        public byte count;
    };

    public enum CommandType
    {
        LCD_SEND_CMD = 1,
        LCD_SEND_DATA,
        LCD_READ_REG,
        LCD_READ_DATA,
        LCD_SET_COLOR,
        LCD_CLEAR_SCREEN,
        LCD_DRAW_HLINE,
        LCD_DRAW_VLINE,
        LCD_DRAW_LINE,
        LCD_DRAW_RECT,
        LCD_FILL_RECT,
        LCD_DRAW_ROUND_RECT,
        LCD_DRAW_CIRCLE,
        LCD_FILL_CIRCLE,
        LCD_DRAW_STRING,
        LCD_DRAW_HPARALLEL,
        LCD_DRAW_VPARALLEL,

        SERVO_GET_SETTING = 32,
        SERVO_SET_SETTING,
        SERVO_SET_ANGLE,
        SERVO_SET_PWM,

        COMPASS_GET_DECLINATION,
        COMPASS_SET_DECLINATION,


        TRACKER_STOP,
        TRACKER_START,

        CTRL_CALIBRATE,
        CTRL_SET_SERVO,
        CTRL_SET_HOME_COORD,
        CTRL_SET_AIR_COORD,

        KEY_STATE = 96,
        KEY_PRESS,

        CMD_ACK = 128,
        CMD_NACK,

        DEV_READY = 255
    }
}
