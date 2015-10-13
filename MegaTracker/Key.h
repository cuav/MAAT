#ifndef KEY_H
#define KEY_H

class Key
{
public:
    void init();
    bool is_pressed();
    bool is_down();
    void update();

private:
    int8_t state;
    bool pressed;
};

#endif //KEY_H
