/*
  
  FlasCC Host for Moai SDK
  Created by Francisco Tufró on 3/5/2013
  © Zipline Games, Inc.
  
*/

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <AS3/AS3.h>
#include <Flash++.h>

#include <aku/AKU.h>
#include <lua-headers/moai_lua.h>

using namespace std;
using namespace AS3::ui;

namespace FlasCCInputDeviceID {
	enum {
		DEVICE,
		TOTAL,
	};
}

namespace FlasCCInputDeviceSensorID {
	enum {
		KEYBOARD,
		POINTER,
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT,
		TOTAL,
	};
}

int width, height;
flash::display::Stage stage;

void initializeAKU () {
  AKUCreateContext ();
  // AKUUntzInit ();
  AKUSetInputConfigurationName ( "AKUFlasCC" );
  
  AKUReserveInputDevices     ( FlasCCInputDeviceID::TOTAL );
  AKUSetInputDevice        ( FlasCCInputDeviceID::DEVICE, "device" );
  
  AKUReserveInputDeviceSensors  ( FlasCCInputDeviceID::DEVICE, FlasCCInputDeviceSensorID::TOTAL );
  AKUSetInputDeviceKeyboard     ( FlasCCInputDeviceID::DEVICE, FlasCCInputDeviceSensorID::KEYBOARD,   "keyboard" );
  AKUSetInputDevicePointer      ( FlasCCInputDeviceID::DEVICE, FlasCCInputDeviceSensorID::POINTER,    "pointer" );
  AKUSetInputDeviceButton       ( FlasCCInputDeviceID::DEVICE, FlasCCInputDeviceSensorID::MOUSE_LEFT, "mouseLeft" );
  AKUSetInputDeviceButton       ( FlasCCInputDeviceID::DEVICE, FlasCCInputDeviceSensorID::MOUSE_MIDDLE, "mouseMiddle" );
  AKUSetInputDeviceButton       ( FlasCCInputDeviceID::DEVICE, FlasCCInputDeviceSensorID::MOUSE_RIGHT,  "mouseRight" );
  
  AKUDetectGfxContext ();

  AKUSetScreenSize ( width, height );
  AKUSetViewSize ( width, height );
  
  AKURunBytecode ( moai_lua, moai_lua_SIZE );
  
}

var keyHandler(void *arg, var as3Args){
    // get the event object
    flash::events::KeyboardEvent event = (flash::events::KeyboardEvent) as3Args[0];
    
    // pull some information out of it
    String type = event->type;
    Object target = event->target;
    int keyCode = event->keyCode;
    
    // convert to std::string
    std::string typeStr = AS3::sz2stringAndFree(internal::utf8_toString(type));
    std::string targetStr = AS3::sz2stringAndFree(internal::utf8_toString(target));

  	AKUEnqueueKeyboardEvent ( FlasCCInputDeviceID::DEVICE, FlasCCInputDeviceSensorID::KEYBOARD, keyCode, typeStr.c_str() == "keyDown" );

    return internal::_undefined;
}

var mouseButtonHandler(void *arg, var as3Args){
    // get the event object
    flash::events::MouseEvent event = (flash::events::MouseEvent) as3Args[0];
    
    // pull some information out of it
    String type = event->type;
    Object target = event->target;
    
    // convert to std::string
    std::string typeStr = AS3::sz2stringAndFree(internal::utf8_toString(type));
    std::string targetStr = AS3::sz2stringAndFree(internal::utf8_toString(target));

  	if ( typeStr == "mouseDown" || typeStr == "mouseUp" )
  			AKUEnqueueButtonEvent ( FlasCCInputDeviceID::DEVICE, FlasCCInputDeviceSensorID::MOUSE_LEFT, ( typeStr == "mouseDown" ));

    if ( typeStr == "rightMouseDown" || typeStr == "rightMouseUp" )
  			AKUEnqueueButtonEvent ( FlasCCInputDeviceID::DEVICE, FlasCCInputDeviceSensorID::MOUSE_RIGHT, ( typeStr == "rightMouseDown" ));

    if ( typeStr == "middleMouseDown" || typeStr == "middleMouseUp" )
  			AKUEnqueueButtonEvent ( FlasCCInputDeviceID::DEVICE, FlasCCInputDeviceSensorID::MOUSE_MIDDLE, ( typeStr == "middleMouseDown" ));

    return internal::_undefined;
}

var mouseMoveHandler(void *arg, var as3Args){
    // get the event object
    flash::events::MouseEvent event = var(as3Args[0]);

    //event.stopPropagation ();

    double x = event->stageX;
    double y = event->stageY;
    
  	AKUEnqueuePointerEvent ( FlasCCInputDeviceID::DEVICE, FlasCCInputDeviceSensorID::POINTER, x, y );
    return internal::_undefined;
}

void initializeInput () {
  // Keyboard input
  stage->addEventListener(flash::events::KeyboardEvent::KEY_DOWN, Function::_new(keyHandler, NULL));
  stage->addEventListener(flash::events::KeyboardEvent::KEY_UP, Function::_new(keyHandler, NULL));

  // Mouse input
  stage->addEventListener(flash::events::MouseEvent::MOUSE_DOWN, Function::_new(mouseButtonHandler, NULL));
  stage->addEventListener(flash::events::MouseEvent::MOUSE_UP, Function::_new(mouseButtonHandler, NULL));
  stage->addEventListener(flash::events::MouseEvent::RIGHT_MOUSE_DOWN, Function::_new(mouseButtonHandler, NULL));
  stage->addEventListener(flash::events::MouseEvent::RIGHT_MOUSE_UP, Function::_new(mouseButtonHandler, NULL));
  stage->addEventListener(flash::events::MouseEvent::MIDDLE_MOUSE_DOWN, Function::_new(mouseButtonHandler, NULL));
  stage->addEventListener(flash::events::MouseEvent::MIDDLE_MOUSE_UP, Function::_new(mouseButtonHandler, NULL));
  
  stage->addEventListener(flash::events::MouseEvent::MOUSE_MOVE, Function::_new(mouseMoveHandler, NULL));
}

extern "C" void mainLoopTick(){
  AKUUpdate();
  AKURender();
}

int main()
{
  stage = internal::get_Stage();

  // Retrieve width and height
  inline_as3(
      "%0 = CModule.rootSprite.stage.stageWidth;\n"
      "%1 = CModule.rootSprite.stage.stageHeight;\n"
      : "=r"(width),"=r"(height) :
  );
  
  initializeAKU ();
  initializeInput ();
  AKURunScript ( "main.lua" );

  AS3_GoAsync();
}