#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>

//#define DEBUG

#define MANUAL_UPGRADE

#ifndef DEBUG
#define debug(x)
#else
#define debug(x) uart_puts(x)
#endif

#include "glcd.h"
#include "uart.h"
#include "keyboard.h"
#include "ctrl_link.h"
#include "ctrl_cmd.h"
#include "image.h"


volatile bool tick = false;
volatile uint8_t update_mode = 0;
volatile uint16_t back_light_timeout = 0;

const uint8_t UPDATE_MODE_NONE = 0;
const uint8_t UPDATE_MODE_M2560 = 1;
const uint8_t UPDATE_MODE_M16  = 2;

void on_recv_message();
ControlLinkMessage msg;
ControlLink	link(uart_putc);


#define EEPROM_VERSION	1

struct setting_t
{
	uint8_t 	mark;
	uint8_t 	ver;
	//uint8_t update_mode;
	int8_t		contrast;
	uint8_t		brightness;	
	int16_t 	el_timeout;	
} setting;

//inline void switch_uart(uint8_t chip)
//{
		
//}

/*inline void switch_uart_to_m16()
{
	PORTD &= !_BV(3);
}*/
//void draw_logo()
//{
//	GLCD.DrawMBitmap_P(BmpLogo1, 30, 5);
	//GLCD.DrawGBitmap_P(BmpLogo2, 9, 60);
//}

//inline void draw_image(uint8_t left, uint8_t top, IMAGE_ID id)
//{
//    GLCD.DrawMBitmap_P(IMAGES[id], left, top);
//}

void draw_logo()
{
	//draw_image(0, 1, IMG_ID_MODE);//IMG_ID_MAVLINK);
    //draw_image(57, 98, IMG_ID_LCD_BIG);
    GLCD.DrawMBitmap_P(IMAGES[IMG_ID_MAVLINK], 0, 1);
    GLCD.DrawMBitmap_P(IMAGES[IMG_ID_LCD_BIG], 57, 98);
    GLCD.FillRect(80, 45, 2, 15, WHITE);
    GLCD.FillRect(80, 80, 2, 15, WHITE);
}

void reset_2560()
{
	PORTC &= ~_BV(3);
	for (int i = 0; i < 255; i++)
	{
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
	}
	PORTC |= _BV(3);
}

#define UART_TO_M16()	(PORTD &= !_BV(3))
#define UART_TO_M2560()	(PORTD |= _BV(3))

//bool ep = true;

void init()
{
		//_delay_ms(1);
//	bool ep = true;
    
    PORTC |= _BV(3);	//2560rst
    DDRC |= _BV(3);	
    
    
    eeprom_read_block(&setting, (const void*)1, sizeof(setting));
    if (setting.mark != 'H' || setting.ver != EEPROM_VERSION)
    {
    	setting.mark = 'H';
    	setting.ver = EEPROM_VERSION;
    	setting.contrast = 0x1e;
    	setting.brightness = 250;
    	setting.el_timeout = 0;
    	eeprom_write_block(&setting, (void*)1, sizeof(setting));
    	//ep = false;
    }
    
    cli();
    
    GLCD.Init(setting.contrast, setting.brightness);
    //GLCD.SetBrightness(100);
    
    DDRD |= _BV(1);
    uart_init(UART_BAUD_SELECT_DOUBLE_SPEED(115200, F_CPU));

		//rst pull up, INT0
    PORTD |= _BV(2);
    MCUCR = _BV(ISC00);
    GICR = _BV(INT0);
		
		
    DDRD |= _BV(3);
	//PORTD |= _BV(3);
    UART_TO_M2560();
	//UART_TO_M16();
		
    //TCCR0 = _BV(CS01) | _BV(CS00);
    // enable timer 0 overflow interrupt
    
    TCCR1A = 0;
    TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS12);	//256·ÖÆµ, CTC
    ICR1 = 1250; //20ms
    //TCNT1 = 1250; //20ms
    //TIMSK1 |= _BV(TOIE1);
    TIMSK |= _BV(OCIE1A);


    Keyboard.Init();
    wdt_enable(WDTO_500MS);
    sei();
}



int main()
{
    
		
    init();
    _delay_ms(100);
    
    debug("uart port test\r\n");
    //GLCD.SetColor(0, 0);
    GLCD.ClearScreen();
    draw_logo();
    GLCD.DrawString(30, 65, "System Booting...");
    
  	/*if (!ep)
  	{
  		GLCD.DrawString(30, 65, "read eeprom error");
  	}  
  	else
  	{
  		char str[64];
  		sprintf(str, "%d bright %d ", sizeof(setting), setting.brightness);
  		GLCD.DrawString(30, 65, str);
  	}
    */
    
    reset_2560();
    //draw_image(0, 0, IMG_
    //GLCD.DrawString(9, 40, "Mavlink Antenna Tracker");
    //GLCD.DrawString(18, 50, "System Initiating...");
   
    
    //link.send_message(DEV_READY, NULL, 0);
    
	back_light_timeout = setting.el_timeout * 50;
	

    while (1)
    {
    	wdt_reset();
        while (uart_available())
        {
            uint8_t c = uart_getc();
            if (link.parse_message(c, &msg))
            {
            //	got_msg = true;
                on_recv_message();
            }
            wdt_reset();
        }

        if (tick)
        {
            tick = false;
            //uart_putc('t');
            debug("t");
            
            link.timeout_tick();
            Keyboard.Update();
            uint8_t press = Keyboard.GetPressKey();
            
            if (press)
            {
            	debug("key pressed\r\n");
                link.send_message(KEY_PRESS, &press, 1);
                if (back_light_timeout == 0)
                	GLCD.TurnOnBackLight();
                back_light_timeout = setting.el_timeout * 50;
            }
            
            if (setting.el_timeout > 0 && back_light_timeout > 0 && --back_light_timeout == 0)
            	GLCD.TurnOffBackLight();
        }
    }

    return 0;
}

void on_recv_message()
{
	//bool ack = false;
    switch (msg.cmd)
    {
    case LCD_SEND_CMD:
        for (uint8_t i = 0; i < msg.len; i++)
            GLCD.SendCmd(msg.data[i]);
        break;
    case LCD_SEND_DATA:
        for (uint8_t i = 0; i < msg.len; i++)
            GLCD.SendData(msg.data[i]);
        break;
    case LCD_READ_REG:
        break;
    case LCD_READ_DATA:
        break;
    case LCD_SET_COLOR:
        GLCD.SetColor(msg.data[0], msg.data[1]);
        break;
    case LCD_CLEAR_SCREEN:
        GLCD.ClearScreen();
        break;
    case LCD_DRAW_HLINE:
        GLCD.DrawHLine(msg.data[0], msg.data[1], msg.data[2]);
        break;
    case LCD_DRAW_VLINE:
        GLCD.DrawVLine(msg.data[0], msg.data[1], msg.data[2]);
        break;
    case LCD_DRAW_LINE:
        GLCD.DrawLine(msg.data[0], msg.data[1], msg.data[2], msg.data[3]);
        break;
    case LCD_DRAW_RECT:
        GLCD.DrawRect(msg.data[0], msg.data[1], msg.data[2], msg.data[3]);
        break;
    case LCD_FILL_RECT:
        GLCD.FillRect(msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4]);
        break;
    case LCD_DRAW_ROUND_RECT:
        GLCD.DrawRoundRect(msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4]);
        break;
    case LCD_DRAW_CIRCLE:
        GLCD.DrawCircle(msg.data[0], msg.data[1], msg.data[2]);
        break;
    case LCD_FILL_CIRCLE:
        GLCD.FillCircle(msg.data[0], msg.data[1], msg.data[2]);
        break;
    case LCD_DRAW_STRING:
        GLCD.DrawString(msg.data[0], msg.data[1], (char*)&msg.data[2], msg.len - 2);
        break;
    case LCD_DRAW_HPARALLEL:
    	GLCD.DrawHParallel(msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4]);
    	break;
    case LCD_DRAW_VPARALLEL:
    	GLCD.DrawVParallel(msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4]);
    	break;
   	case LCD_DRAW_IMAGE:
   		//draw_image(msg.data[0], msg.data[1], (IMAGE_ID)msg.data[2]);
   		GLCD.DrawMBitmap_P(IMAGES[msg.data[2]], msg.data[0], msg.data[1]);
   		//GLCD.DrawMBitmap_P(IMAGES[IMG_ID_MODE], 10, 20);
   		break;
   	case LCD_DRAW_LOGO:
   		draw_logo();
   		break;
    case LCD_SET_BRIGHTNESS:
    	setting.brightness = msg.data[0];
    	GLCD.SetBrightness(setting.brightness);
    	break;
   	case LCD_GET_BRIGHTNESS:
   		link.send_message(LCD_SET_BRIGHTNESS, &setting.brightness, sizeof(setting.brightness));
   		break;
   	case LCD_SET_CONTRAST:
   		setting.contrast = msg.data[0];
   		GLCD.SetContrast(setting.contrast);
   		break;
   	case LCD_GET_CONTRAST:
   		link.send_message(LCD_SET_CONTRAST, &setting.contrast, sizeof(setting.contrast));
   		break;
   	case LCD_SET_BL_TIMEOUT:
   		setting.el_timeout = *((int16_t*)msg.data);
   		back_light_timeout = setting.el_timeout * 50;
   		break;
   	case LCD_GET_BL_TIMEOUT:
   		link.send_message(LCD_SET_BL_TIMEOUT, &setting.el_timeout, sizeof(setting.el_timeout));
   		break;
   	case LCD_SAVE_SETTING:
   		eeprom_write_block(&setting, (void*)1, sizeof(setting));
   		//link.send_message(LCD_SAVE_SETTING);
   		break;
    case KEY_STATE:
    {
        uint8_t key_state = Keyboard.GetKeyState();
        link.send_message(KEY_STATE, &key_state, 1);
        //ack = false;
    }
    	break;
    //case CMD_ACK:
    //case CMD_NACK:
    //	  ack = false;
    //	  break;
    case CTRL_UPDATE:
    	update_mode = msg.data[0];
    	break;
    case CTRL_RESET:
    	break;
    case DEV_QUERY:
    	link.send_message(DEV_READY, 0, 0);
    	break;
    }

	//if (ack)
    //	link.send_message(CMD_ACK, &msg.seq, 1);
}


ISR(TIMER1_COMPA_vect)
{
    tick = true;
}

ISR(INT0_vect)
{

	if (update_mode == UPDATE_MODE_M16)
	{
		if (!(PIND & _BV(2)))
			while(1);	//trigger watch dog
	}
#ifdef MANUAL_UPGRADE	
	else if (update_mode == UPDATE_MODE_M2560)
#else
	else
#endif		
	{

		if (!(PIND & _BV(2)))
		{
			reset_2560();
		}
	}

		//PORTC |= ((PIND & _BV(2)) << 1);
//	}
}