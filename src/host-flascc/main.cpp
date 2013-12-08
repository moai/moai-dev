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

extern "C" void initializeAKU () {
  printf ( "Initializing AKU\n" );

	AKUCreateContext ();
   AKUSetInputConfigurationName ( "AKUFlasCC" );
   AKUReserveInputDevices     ( FlasCCInputDeviceID::TOTAL );
   AKUSetInputDevice        ( FlasCCInputDeviceID::DEVICE, "device" );
   
   AKUReserveInputDeviceSensors  ( FlasCCInputDeviceID::DEVICE, FlasCCInputDeviceSensorID::TOTAL );
   AKUSetInputDeviceKeyboard     ( FlasCCInputDeviceID::DEVICE, FlasCCInputDeviceSensorID::KEYBOARD,   "keyboard" );
   AKUSetInputDevicePointer      ( FlasCCInputDeviceID::DEVICE, FlasCCInputDeviceSensorID::POINTER,    "pointer" );
   AKUSetInputDeviceButton       ( FlasCCInputDeviceID::DEVICE, FlasCCInputDeviceSensorID::MOUSE_LEFT, "mouseLeft" );
   AKUSetInputDeviceButton       ( FlasCCInputDeviceID::DEVICE, FlasCCInputDeviceSensorID::MOUSE_MIDDLE, "mouseMiddle" );
   AKUSetInputDeviceButton       ( FlasCCInputDeviceID::DEVICE, FlasCCInputDeviceSensorID::MOUSE_RIGHT,  "mouseRight" );
   
   //AKUDetectGfxContext ();
   
   AKUSetScreenSize ( width, height );
   AKUSetViewSize ( width, height );
   
  AKURunBytecode ( moai_lua, moai_lua_SIZE );
  
}

extern "C" void tick(){
  //printf( "Tick!\n" );
  AKUUpdate();
  //AKURender();
}

int main()
{
  stage = internal::get_Stage();

  printf ( "=======================\n" );
  printf ( "FlasCC Host\n" );
  printf ( "=======================\n" );

  // Retrieve width and height
  inline_as3(
      "%0 = CModule.rootSprite.stage.stageWidth;\n"
      "%1 = CModule.rootSprite.stage.stageHeight;\n"
      : "=r"(width),"=r"(height) :
  );
  
  initializeAKU ();
  AKURunScript( "main.lua" );
  AKURunString ( " print ( 'hi' ) " );

  AS3_GoAsync();
}