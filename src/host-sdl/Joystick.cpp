
#include "Joystick.h"

Joystick::Joystick(const int idx) :
    index(idx),
    sdl_joystick(nullptr)
{ }

Joystick::~Joystick() {

    if ( isOpen() ) {
        SDL_JoystickClose(sdl_joystick);
        sdl_joystick = nullptr;
    }
}



bool Joystick::Open() {
        
    sdl_joystick = SDL_JoystickOpen(index);

    if ( sdl_joystick == nullptr ) {
        std::cerr << "Unable to open joystick" << std::endl;
        return false;
    }

    return true;
}



bool Joystick::isOpen() const {

    if ( nullptr != sdl_joystick ) {
        std::cerr << "Joystick already open! " << std::endl;
        return true;
    }

    return false;
}


const char* Joystick::getName() const {
    return SDL_JoystickNameForIndex(index);
}



const Joystick::AXIS_MOTION Joystick::HandleAxisMotion(const SDL_Event & event) {

    int x = 0;
    int y = 0;
    
    if( event.jaxis.axis == 0 ) { //X axis
        if( event.jaxis.value < -JOYSTICK_DEAD_ZONE ) {
            x = -1;
        } else if( event.jaxis.value > JOYSTICK_DEAD_ZONE ) {
            x =  1;
        }
    } else if ( event.jaxis.axis == 1 ) { // Y axis
        if( event.jaxis.value < -JOYSTICK_DEAD_ZONE ) {
            y = -1;
        } else if( event.jaxis.value > JOYSTICK_DEAD_ZONE ) {
            y =  1;
        }
    }

    return std::make_pair(x, y);
}

