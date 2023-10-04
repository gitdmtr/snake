#include <Arduino.h>
#include "Joystick.h"

/// The neutral level for the joystick pin values.
const int PIN_NEUTRAL_LVL = 2048; 

/// The threshold for pin value change to activate a direction.
const int PIN_ACTIVATION_THRESHOLD = 50;

/**
 * @brief Constructor for the Joystick class.
 * @param PIN_VERTICAL_ Pin number for the vertical axis of the joystick.
 * @param PIN_HORIZONTAL_ Pin number for the horizontal axis of the joystick.
 * @param PIN_SELECT_ Pin number for the select button of the joystick.
 */
Joystick::Joystick(int PIN_VERTICAL_, int PIN_HORIZONTAL_, int PIN_SELECT_) : 
    PIN_VERTICAL(PIN_VERTICAL_), 
    PIN_HORIZONTAL(PIN_HORIZONTAL_),  
    PIN_SELECT(PIN_SELECT_) 
{ }

/**
 * @brief Read the direction of the joystick.
 * @param direction A reference to a direction_t variable to store the 
 *        detected direction.
 */
void Joystick::read_direction(direction_t& direction)
{
    int horz = analogRead(PIN_HORIZONTAL);
    int vert = analogRead(PIN_VERTICAL);
    if (vert < (PIN_NEUTRAL_LVL - PIN_ACTIVATION_THRESHOLD))  
    {
        direction = DOWN;
    }
    if (vert > (PIN_NEUTRAL_LVL + PIN_ACTIVATION_THRESHOLD))
    {
        direction = UP;
    }
    if (horz > (PIN_NEUTRAL_LVL + PIN_ACTIVATION_THRESHOLD))
    {
        direction = LEFT;
    }
    if (horz < (PIN_NEUTRAL_LVL - PIN_ACTIVATION_THRESHOLD))
    {
        direction = RIGHT;
    }
}

/**
 * @brief Initialize the joystick pins.
 */
void Joystick::init()
{
    pinMode(PIN_VERTICAL, INPUT);
    pinMode(PIN_HORIZONTAL, INPUT);
    pinMode(PIN_SELECT, INPUT_PULLUP);
}