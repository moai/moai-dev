
#include "SDLJoystick.h"

Joystick::Joystick(const int idx) :
    index(idx),
    sdl_joystick(NULL)
{ }

Joystick::~Joystick() {

    if ( isOpen() ) {
        SDL_JoystickClose(sdl_joystick);
        sdl_joystick = NULL;
    }
}



bool Joystick::Open() {
        
    sdl_joystick = SDL_JoystickOpen(index);

    if ( sdl_joystick == NULL ) {
        std::cerr << "Unable to open joystick" << std::endl;
        return false;
    }

    return true;
}



bool Joystick::isOpen() const {

    if ( NULL != sdl_joystick ) {
        std::cerr << "Joystick already open! " << std::endl;
        return true;
    }

    return false;
}


const char* Joystick::getName() const {
    return SDL_JoystickNameForIndex(index);
}



const Joystick::AXIS_MOTION & Joystick::HandleAxisMotion(const SDL_Event & event) {

    if( event.jaxis.axis == 0 ) { //X axis

        if( event.jaxis.value < -JOYSTICK_DEAD_ZONE ) {
            
            axis_motion.x = -1;
        
        } else if( event.jaxis.value > JOYSTICK_DEAD_ZONE ) {
            
            axis_motion.x =  1;
        
        } else { 
            
            axis_motion.x = 0;
        }

    } else if ( event.jaxis.axis == 1 ) { // Y axis
        
        if( event.jaxis.value < -JOYSTICK_DEAD_ZONE ) {
            
            axis_motion.y = -1;

        } else if( event.jaxis.value > JOYSTICK_DEAD_ZONE ) {
            
            axis_motion.y =  1;

        } else {
            
            axis_motion.y = 0;
        }
    }

    return axis_motion;
}

