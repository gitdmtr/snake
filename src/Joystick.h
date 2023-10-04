
#ifndef JOYSTICK
#define JOYSTICK

enum direction_t 
{   
    DOWN, 
    UP, 
    LEFT,
    RIGHT
};

class Joystick 
{
    public:       
        Joystick(int PIN_VERTICAL_, int PIN_HORIZONTAL_, int PIN_SELECT_);
        void read_direction(direction_t& direction);
        void init();
    private:
        const int PIN_VERTICAL;    
        const int PIN_HORIZONTAL;  
        const int PIN_SELECT; 
};

#endif