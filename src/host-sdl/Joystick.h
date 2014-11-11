#pragma once 

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
    
        using AXIS_MOTION = std::pair<int, int>;

    public:

        bool Open();
        bool isOpen() const;
        const char* getName() const;
        const AXIS_MOTION HandleAxisMotion(const SDL_Event & event);

    private:

        int index;
        SDL_Joystick* sdl_joystick;
        static constexpr int JOYSTICK_DEAD_ZONE = 8000;
};
