#ifndef	SDLJOYSTICK_H
#define	SDLJOYSTICK_H

#include <SDL.h>

#include <iostream>
#include <utility>

class Joystick
{
    public:
        
        explicit Joystick(const int index);
        ~Joystick();
        
        Joystick(const Joystick&) = delete;
        void operator= (const Joystick&) = delete;
    
        struct AXIS_MOTION {
            int x;
            int y;
            AXIS_MOTION() : x(0), y(0) { }
        };

        /* TODO: Wait, see pull #1026 */
        /* struct BALL_MOTION { 
            AXIS_MOTION axis;
            int ball;
            BALL_MOTION() : ball(0) { }
        };*/

    public:

        bool Open();
        bool isOpen() const;
        const char* getName() const;
        const AXIS_MOTION & HandleAxisMotion(const SDL_Event & event);

    private:

        AXIS_MOTION axis_motion;

        int index;
        SDL_Joystick* sdl_joystick;
        static const int JOYSTICK_DEAD_ZONE = 8000;
};

#endif
