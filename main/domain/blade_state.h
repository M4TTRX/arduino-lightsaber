#pragma once

#include <string.h>

// BladeState describes the state of the blade
enum BladeState
{
    // OFF: The blade is off
    OFF,

    // IDLE: The blade is on and not moving
    IDLE,

    // POWER_ON the blade is turning on
    POWER_ON,

    // POWER_OFF the blade is turning off
    POWER_OFF,

    // SWING: The blade is on and moving
    SWING,

    // CLASH: The blade is on and hiting something
    CLASH
};

String state_to_string(BladeState state)
{
    switch (state)
    {
    case OFF:
        return "OFF";
        break;
    case IDLE:
        return "IDLE";
        break;
    case POWER_ON:
        return "POWER_ON";
        break;
    case POWER_OFF:
        return "POWER_OFF";
        break;
    case SWING:
        return "SWING";
        break;
    case CLASH:
        return "CLASH";
        break;
    default:
        return "UNKNOWN";
        break;
    }
}