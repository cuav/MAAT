#ifndef CTRL_CMD_H
#define CTRL_CMD_H

enum CTRL_CMD
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
    LCD_DRAW_IMAGE,
    LCD_DRAW_LOGO,
    
    LCD_SET_BRIGHTNESS,
    LCD_GET_BRIGHTNESS,
    LCD_SET_CONTRAST,
    LCD_GET_CONTRAST,
    LCD_SET_BL_TIMEOUT,
    LCD_GET_BL_TIMEOUT,
    
    LCD_SAVE_SETTING,

    SERVO_GET_SETTING = 32,
    SERVO_SET_SETTING,
    SERVO_SET_ANGLE,
    SERVO_SET_PWM,

    COMPASS_GET_DECLINATION,
    COMPASS_SET_DECLINATION,


    TRACKER_STOP,
    TRACKER_START,
    
	CTRL_UPDATE,
	CTRL_RESET,
    CTRL_CALIBRATE,
    CTRL_SET_SERVO,
    CTRL_SET_HOME_COORD,
    CTRL_SET_AIR_COORD,
    CTRL_SET_ANGLES,

    KEY_STATE = 96,
    KEY_PRESS,

    CMD_ACK = 128,
    CMD_NACK,

	DEV_QUERY = 254,
    DEV_READY = 255,
};

enum IMAGE_ID
{
	IMG_ID_MAVLINK, IMG_ID_LCD_BIG, 
	IMG_ID_MODE, IMG_ID_SETUP, IMG_ID_UPGRADE, 
	IMG_ID_SERVO, IMG_ID_SENSOR, IMG_ID_LCD_SMALL,
	IMG_ID_AAT, IMG_ID_POWER, IMG_ID_SAT, IMG_ID_TF, IMG_ID_HOME, IMG_ID_RESET,
};

enum BLACK_LIGHT_MODE
{
	BLACK_LIGHT_ON,	BLACK_LIGHT_30, BLACK_LIGHT_60, BLACK_LIGHT_180, BLACK_LIGHT_360
};

struct SetColorMsgData
{
    uint8_t fgcolor;
    uint8_t bgcolor;
};

struct DrawHLineMsgData
{
    uint8_t x;
    uint8_t y;
    uint8_t width;
};

struct DrawVLineMsgData
{
    uint8_t x;
    uint8_t y;
    uint8_t height;
};

struct DrawHParallelMsgData
{
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t interval;
    uint8_t count;
};

struct DrawVParallelMsgData
{
    uint8_t x;
    uint8_t y;
    uint8_t height;
    uint8_t interval;
    uint8_t count;
};

struct DrawLineMsgData
{
    uint8_t x1;
    uint8_t y1;
    uint8_t x2;
    uint8_t y2;
};

struct DrawRectMsgData
{
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;
};

struct FillRectMsgData
{
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;
    uint8_t color;
};

struct DrawRoundRectMsgData
{
    uint8_t x1;
    uint8_t y1;
    uint8_t x2;
    uint8_t y2;
    uint8_t round;
};

struct DrawStringMsgData
{
    uint8_t x;
    uint8_t y;
    uint8_t str[62];
};

struct SetBrightnessMsgData
{
		uint8_t brightness;
};

struct ServoGetSettingMsgData
{
    uint8_t idx;
};

struct ServoSetSettingMsgData
{
    uint8_t idx;
    int16_t min;
    int16_t max;
    int16_t cen;
    int16_t nty;
    int16_t acc;
    int16_t  dzone;
    int8_t  save;
};

struct ServoSetAngleMsgData
{
    uint8_t idx;
    float		angle;
};

struct ServoSetPwmMsgData
{
    uint8_t idx;
    short pwm;
};

struct CompassSetDeclination
{
    float declination;
};



#endif