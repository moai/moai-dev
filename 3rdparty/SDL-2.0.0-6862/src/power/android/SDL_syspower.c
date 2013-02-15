#include "SDL_config.h"

#ifndef SDL_POWER_DISABLED
#if SDL_POWER_ANDROID

#include "SDL_power.h"

#include "../../core/android/SDL_android.h"

SDL_bool
SDL_GetPowerInfo_Android(SDL_PowerState * state, int *seconds, int *percent)
{
    int battery;
    int plugged;
    int charged;

    if (Android_JNI_GetPowerInfo(&plugged, &charged, &battery, seconds, percent) != -1) {
        if (plugged) {
            if (charged) {
                *state = SDL_POWERSTATE_CHARGED;
            } else if (battery) {
                *state = SDL_POWERSTATE_CHARGING;
            } else {
                *state = SDL_POWERSTATE_NO_BATTERY;
                *seconds = -1;
                *percent = -1;
            }
        } else {
            *state = SDL_POWERSTATE_ON_BATTERY;
        }
    } else {
        *state = SDL_POWERSTATE_UNKNOWN;
        *seconds = -1;
        *percent = -1;
    }

    return SDL_TRUE;
}

#endif /* SDL_POWER_ANDROID */
#endif /* SDL_POWER_DISABLED */
