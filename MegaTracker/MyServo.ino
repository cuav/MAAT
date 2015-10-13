#include "MyServo.h"
#include <avr/io.h>

MyServo* _sa = NULL, *_sb = NULL, *_sc = NULL;
//OC4A = PH3
//OC4B = PH4


void MyServo::init()
{
//    DDRH |= _BV(3) | _BV(4) | _BV(5);
//    PORTH &= ~(_BV(3) | _BV(4) | _BV(5));
    DDRE |= _BV(3) | _BV(4);
    PORTH &= ~(_BV(3) | _BV(4));

    TCCR3A =  _BV(WGM31);
    TCCR3B = _BV(WGM33) | _BV(WGM32) | _BV(CS31);  // 1/8 freq, fast PWM
    ICR3 = 39999; // 100Hz, 10ms
    OCR3A = 0;
    OCR3B = 0;
//    DDRE |= _BV(4);
//    PORTE |= _BV(4);
//    TIMSK4 |= _BV(OCIE4A) | _BV(OCIE4B);
}

MyServo::MyServo(SERVO_PORT port, int16_t _min, int16_t _max, int16_t _cen)
{
    rom_addr = eeprom.alloc(offsetof(MyServo, rom_addr) - offsetof(MyServo, cen));
//        this->port = port;
    max = _max;
    min = _min;
    cen = _cen;

    hpi = 500;
    acc = 500;
    dzone = 0;
    //rev = 1;
    this->port = port;
}

void MyServo::load()
{
    if (eeprom.is_valid())
    {
        eeprom.read(&this->cen, rom_addr, offsetof(MyServo, rom_addr) - offsetof(MyServo, cen));
        if (max > Maxim)
            max = Maxim;
        if (min < Minim)
            min = Minim;
        if (cen < min || cen > max)
            cen = (min + max) >> 1;
        dst = cen;
        debug("MAX %d MIN %d CEN %d NTY %d ACC %d DZ %d\r\n", max, min, cen, ninty, acc, dzone);
    }
    else
        save();
}

void MyServo::reset()
{
	max = min = cen = Center;
	hpi = acc = 500;
	dzone = 0;
	
}

void MyServo::start()
{
    if (port == SERVO_PORT_T3A)
    {
        _sa = this;
        TCCR3A |= _BV(COM3A1);
        OCR3A = dst = cen << 1;
        TIMSK3 |= _BV(OCIE3A) ;
    }
    else if (port == SERVO_PORT_T3B)
    {
        _sb = this;
        TCCR3A |= _BV(COM3B1);
        OCR3B = dst = cen << 1;
        TIMSK3 |= _BV(OCIE3B) ;
    }
    /*
    else if (port == SERVO_PORT_T4C)
    {
        _sc = this;
        TCCR4A |= _BV(COM4C1);
        OCR4C = cen;
        TIMSK4 |= _BV(OCIE4C);
    }*/
}

void MyServo::stop()
{
}

void MyServo::set_pwm(int16_t value)
{
    value = constrain(value, min, max);
    if (value > cen && value < cen + dzone)
        value = cen + dzone;
    else if (value < cen && value > cen - dzone)
        value = cen - dzone;
//    Serial.printf("MyServo::set_pwm %d\n", value);
    cli();
    dst = value << 1;
    sei();

    /*
        if (port == SERVO_PORT_T4A)
            OCR4A = value << 1;
        else if (port == SERVO_PORT_T4B)
            OCR4B = value << 1;
    */
}

ISR(TIMER3_COMPA_vect)
{
    int16_t value = _sa->get_raw(OCR3A);//constrain(_sa->dst, OCR3A -_sa->get_acc(), OCR3A + _sa->get_acc());
    OCR3A = value;
}

ISR(TIMER3_COMPB_vect)
{
    int16_t value = _sb->get_raw(OCR3B);//constrain(_sb->dst, OCR3B -_sb->get_acc(), OCR3B + _sb->get_acc());
    OCR3B = value;
}

/*
ISR(TIMER4_COMPC_vect)
{
    int16_t value = constrain(_sc->dst, OCR4C -_sc->acc, OCR4C + _sb->acc);
    OCR4C = value;
}*/
