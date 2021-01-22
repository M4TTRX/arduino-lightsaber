#include "domain/blade_state.h"

#define RED_LED 9
#define GREEN_LED 10
#define BLUE_LED 11

struct rgb
{
    int r;
    int g;
    int b;
};

void blade_light(BladeState blade_state, rgb blade_color)
{

    switch (blade_state)
    {
    case OFF:
        // turn off the led
        analogWrite(RED_LED, 0);
        analogWrite(GREEN_LED, 0);
        analogWrite(BLUE_LED, 0);
        break;
    case IDLE:
        analogWrite(RED_LED, 0);
        analogWrite(GREEN_LED, 0);
        analogWrite(BLUE_LED, 0);
        break;
    case SWING:
        analogWrite(RED_LED, blade_color.r);
        analogWrite(GREEN_LED, blade_color.g);
        analogWrite(BLUE_LED, blade_color.b);
        break;
    case CLASH:
        analogWrite(RED_LED, 255);
        analogWrite(GREEN_LED, 255);
        analogWrite(BLUE_LED, 255);
        break;
    default:
        break;
    }
    return;
}