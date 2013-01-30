/// @file moai.cc

#include <cstdio>
#include <string>
#include <string.h>
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"

#include "ppapi/cpp/file_ref.h"

#include "geturl_handler.h"
#include "NaClFileSystem.h"
#include "opengl_context.h"
#include "moaicore/pch.h"
#include "moai_nacl.h"

#include <unistd.h>
#include <stdio.h>
 
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include "zlcore/zlcore.h"
#include <aku/AKU.h>

#include "ppapi/gles2/gl2ext_ppapi.h"
#include <GLES2/gl2.h>

#include "ppapi/cpp/rect.h"
#include "ppapi/cpp/size.h"
#include "ppapi/cpp/var.h"

#include <aku/AKU-fmod-ex.h>

#include "moaicore/MOAIGfxDevice.h"
#include "MOAIApp.h"

#include <lua-headers/moai_lua.h>

namespace {

pthread_t gThreadId;
bool g_swapping = false;
bool g_generate_uid = false;

}

NaClFileSystem *g_FileSystem = NULL;

NaClQueue<std::string> *g_MessageQueue = NULL;
NaClQueue<pp::InputEvent> *g_InputQueue = NULL;

int g_width = 0;
int g_height = 0;
float g_scale = 0.0f;
int g_bInitialized = 0;
bool g_handlingInput;
MoaiInstance *g_instance = NULL;
pp::Core* g_core = NULL;
bool g_pauseUpdate;
int g_expectedWidth = 0;
int g_expectedHeight = 0;
#include <stdint.h>

extern "C" {

 __inline__ uint64_t rdtsc(void) {
   uint32_t lo, hi;

   __asm__ __volatile__ (      // serialize
   "xorl %%eax,%%eax \n        cpuid"
   ::: "%rax", "%rbx", "%rcx", "%rdx");

   /* We cannot use "=A", since this would use %rax on x86_64 and return only the lower 32bits of the TSC */
   __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));

   return (uint64_t)hi << 32 | lo;

 }
}

namespace NaClInputDeviceID {
	enum {
		DEVICE = 0,
		TOTAL,
	};
}

namespace NaClInputDeviceSensorID {
	enum {
		KEYBOARD = 0,
		POINTER,
		WHEEL,
		MOUSE_LEFT,
		MOUSE_MIDDLE,
		MOUSE_RIGHT,
		TOTAL,
	};
}

//----------------------------------------------------------------//
void NaClHandleInputEvent ( const pp::InputEvent & event ) {
	
	switch ( event.GetType() ) {

		case PP_INPUTEVENT_TYPE_MOUSEDOWN:
		case PP_INPUTEVENT_TYPE_MOUSEUP: {

			pp::MouseInputEvent mouse_event ( event );

			bool mouseDown = false;
			if( event.GetType() == PP_INPUTEVENT_TYPE_MOUSEDOWN ) {
				mouseDown = true;
			}

			switch ( mouse_event.GetButton() ) {
				case PP_INPUTEVENT_MOUSEBUTTON_LEFT:
					AKUEnqueueButtonEvent ( NaClInputDeviceID::DEVICE, NaClInputDeviceSensorID::MOUSE_LEFT, mouseDown );
					break;
				case PP_INPUTEVENT_MOUSEBUTTON_RIGHT:
					AKUEnqueueButtonEvent ( NaClInputDeviceID::DEVICE, NaClInputDeviceSensorID::MOUSE_RIGHT, mouseDown );
					break;
				case PP_INPUTEVENT_MOUSEBUTTON_MIDDLE:
					AKUEnqueueButtonEvent ( NaClInputDeviceID::DEVICE, NaClInputDeviceSensorID::MOUSE_MIDDLE, mouseDown );
					break;
				default:
					break;
			}
			break;
		}

		case PP_INPUTEVENT_TYPE_MOUSEMOVE: {
			pp::MouseInputEvent mouse_event ( event );
			AKUEnqueuePointerEvent ( NaClInputDeviceID::DEVICE, NaClInputDeviceSensorID::POINTER, mouse_event.GetPosition ().x () / MOAIGfxDevice::Get ().GetDeviceScale (), mouse_event.GetPosition ().y () / MOAIGfxDevice::Get ().GetDeviceScale () );
			//AKUEnqueuePointerEvent ( NaClInputDeviceID::DEVICE, NaClInputDeviceSensorID::POINTER, mouse_event.GetPosition ().x () , mouse_event.GetPosition ().y ());
			break;
		}

		case PP_INPUTEVENT_TYPE_WHEEL: {
			pp::WheelInputEvent wheel_event ( event );

			AKUEnqueueWheelEvent ( NaClInputDeviceID::DEVICE, NaClInputDeviceSensorID::WHEEL, wheel_event.GetTicks ().y ());

			break;
		}

		case PP_INPUTEVENT_TYPE_KEYUP:
		case PP_INPUTEVENT_TYPE_KEYDOWN: {

			pp::KeyboardInputEvent keyboard_event ( event );

			bool keyDown = false;
			if( event.GetType() == PP_INPUTEVENT_TYPE_KEYDOWN ) {
				keyDown = true;
			}

			int keycode = keyboard_event.GetKeyCode ();

			AKUEnqueueKeyboardEvent ( NaClInputDeviceID::DEVICE, NaClInputDeviceSensorID::KEYBOARD, keycode, keyDown );

			break;
		}

		default: {
			NACL_LOG ( "Moai_NaCl: unHandled event %d\n", event.GetType() );
			break;
		}
	}
}
//----------------------------------------------------------------//
void PostMessageThenDeleteMainThread ( void * userData, int32_t result ) {
	char *message  = ( char * ) userData;

	g_instance->PostMessage ( pp::Var ( message ));

	delete [] message;
}

//----------------------------------------------------------------//
void NaClPostMessage ( char *str ) {

	int size = strlen ( str );
	char *message = new char [ size + 1];
	strcpy ( message, str );

	message [ size ] = 0;

	pp::CompletionCallback cc ( PostMessageThenDeleteMainThread, message );

	g_core->CallOnMainThread ( 0, cc , 0 );
}

//----------------------------------------------------------------//
void FlushMainThread ( void* userData, int32_t result ) {

	glFlush ();
}

void NaClFlush () {

	pp::CompletionCallback cc ( FlushMainThread, g_instance );
	g_core->CallOnMainThread ( 0, cc , 0 );

}

void GenerateUIDMainThread ( void* userData, int32_t result ) {
	
	g_instance->PostMessage ( "UID" );
}

//----------------------------------------------------------------//
void HandleSocialMessage ( std::string & message ) {

	if ( message.find ( "UID:" ) != std::string::npos ) {

		int responseStartIndex =  strlen ( "UID:" );

		MOAIEnvironment::Get().SetValue ( MOAI_ENV_udid, message.c_str () + responseStartIndex );

		g_generate_uid = false;
	}
}


void NaClGetUID () {

	g_generate_uid = true;

	pp::CompletionCallback cc ( GenerateUIDMainThread, g_instance );
	g_core->CallOnMainThread ( 0, cc , 0 );

	std::string message;
	while ( g_generate_uid ) {

		while ( g_MessageQueue->PopMessage ( message )) {
			HandleSocialMessage ( message );
			MOAIApp::HandleStoreMessage ( message );
		}

		usleep ( 1000 );
	}
}

//----------------------------------------------------------------//
void RenderMainThread ( void* userData, int32_t result ) {

	AKUFmodExUpdate ();

	g_instance->DrawSelf ();

	g_swapping = false;
}

//----------------------------------------------------------------//
void NaClRender () {

	g_swapping = true;

	while ( g_instance->GetOpenGLContext ()->flush_pending ()) {
		usleep ( 1000 );
	}

	pp::CompletionCallback cc ( RenderMainThread, g_instance );
	g_core->CallOnMainThread ( 0, cc , 0 );

	while ( g_swapping ) {

		usleep ( 1000 );
	}
}

//----------------------------------------------------------------//
void InputMainThread ( void* userData, int32_t result ) {

	pp::InputEvent ievent;
	while ( g_InputQueue->PopMessage ( ievent )) {
		NaClHandleInputEvent ( ievent );
	}

	g_handlingInput = false;
}

//----------------------------------------------------------------//
void NaClInput () {

	g_handlingInput = true;

	pp::CompletionCallback cc ( InputMainThread, g_instance );
	g_core->CallOnMainThread ( 0, cc , 0 );

	while ( g_handlingInput ) {

		usleep ( 1000 );
	}
}

//================================================================//
// AKU callbacks
//================================================================//

void	_AKUEnterFullscreenModeFunc		();
void	_AKUExitFullscreenModeFunc		();
void	_AKUOpenWindowFunc				( const char* title, int width, int height );
void	_AKUStartGameLoopFunc			();

//----------------------------------------------------------------//
void _AKUEnterFullscreenModeFunc () {

	NACL_LOG ( "Moai_NaCl: unimplemented _AKUEnterFullscreenModeFunc\n" );
}

//----------------------------------------------------------------//
void _AKUExitFullscreenModeFunc () {

	NACL_LOG ( "Moai_NaCl: unimplemented _AKUExitFullscreenModeFunc\n" );
}

//----------------------------------------------------------------//
void _AKUOpenWindowFunc ( const char* title, int width, int height ) {
	
	g_expectedWidth = width;
	g_expectedHeight = height;

	NACL_LOG ( "AKUOpenWindowFunc: %d, %d\n", g_expectedWidth, g_expectedHeight );

	if ( g_expectedWidth && g_width ) {
		MOAIGfxDevice::Get ().SetBufferScale ( g_width / ( float )g_expectedWidth );
	}
	else {
		MOAIGfxDevice::Get ().SetBufferScale ( 1.0f );
	}
}

//----------------------------------------------------------------//
void _AKUStartGameLoopFunc () {

	NACL_LOG ( "Moai_NaCl: unimplemented _AKUStartGameLoopFunc\n" );
}

int g_LuaMem = 0;
int g_TexMem = 0;
void* moai_main ( void *_instance ) {

	g_instance = ( MoaiInstance * ) _instance;
	g_FileSystem->Init ();
	NACL_LOG ( "File System Initialized\n" );

	AKURunBytecode ( moai_lua, moai_lua_SIZE );

	AKURunScript ( "main.lua" );

	NACL_LOG ( "Main Lua\n" );

	AKURunScript ( "config.lua" );
	AKURunScript ( "game.lua" );

	//NaClGetUID ();

	while ( true ) {

		while ( g_pauseUpdate ) {
			usleep ( 1000 );
		}

		//handle messages
		std::string message;
		while ( g_MessageQueue->PopMessage ( message )) {
			HandleSocialMessage ( message );
			MOAIApp::HandleStoreMessage ( message );
		}
		
		NaClInput ();

		AKUUpdate ();

		NaClRender ();
	}

	return NULL;
}

//----------------------------------------------------------------//
MoaiInstance::~MoaiInstance() {
}

//----------------------------------------------------------------//
bool MoaiInstance::Init ( uint32_t /* argc */, const char* /* argn */[], const char* /* argv */[] ) {

	g_instance = this;

	opengl_context = NULL;

	return true;
}

//----------------------------------------------------------------//
bool MoaiInstance::HandleInputEvent	( const pp::InputEvent & event ) {
	
	g_InputQueue->Push ( event );

	return PP_TRUE;
}

void MoaiInstance::DidChangeFocus (	bool has_focus ) {
	NACL_LOG ( "NaCl DidChangeFocus %d\n", has_focus );
	g_pauseUpdate = !has_focus;
}

//----------------------------------------------------------------//
void MoaiInstance::DidChangeView ( const pp::Rect& position, const pp::Rect& clip ) {

	NACL_LOG ( "NaCl DidChangeView %d,%d\n", clip.size ().width (), clip.size ().height ());
	if ( g_width == position.size ().width () && g_height == position.size ().height () ) {
		return;
	}

	g_width = position.size ().width ();
	g_height = position.size ().height ();

	//lazy init time
	USDeviceTime::GetTimeInSeconds ();

	NACL_LOG ( "resize to %d, %d\n", position.size ().width (), position.size ().height () );

	if (opengl_context == NULL) {
		opengl_context = new OpenGLContext ( this );
	}

	NACL_LOG ( "OpenGLContext %p\n", opengl_context );

	opengl_context->InvalidateContext ( this );
	
	opengl_context->ResizeContext ( position.size ());

	NACL_LOG ( "OpenGLContext MakeContextCurrent\n" );

	if ( !opengl_context->MakeContextCurrent ( this )) {
		printf ( "Error: OpenGLContext failed to MakeContextCurrent\n" );
		g_instance->PostMessage ( pp::Var ( "ALERT:OPENGL" ));
		return;
	}

	NACL_LOG ( "OpenGLContext Created\n" );
	if ( !g_bInitialized ) {

		//AJV flush early to avoid uninited memory being shown
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT);
		glFinish ();
		opengl_context->FlushContext();

		//init all systems
		g_FileSystem = new NaClFileSystem ( g_core, this );
		NACL_LOG ( "NaClFileSystem Created\n" );

		g_MessageQueue = new  NaClQueue<std::string> ();
		g_InputQueue = new  NaClQueue<pp::InputEvent> ();
		NACL_LOG ( "NaClQueue Created\n" );

		AKUCreateContext ();
		NACL_LOG ( "AKUContext Created\n" );

		AKUSetInputConfigurationName ( "AKUNaCl" );

		AKUReserveInputDevices			( NaClInputDeviceID::TOTAL );
		AKUSetInputDevice				( NaClInputDeviceID::DEVICE, "device" );
	
		AKUReserveInputDeviceSensors	( NaClInputDeviceID::DEVICE, NaClInputDeviceSensorID::TOTAL );
		AKUSetInputDeviceKeyboard		( NaClInputDeviceID::DEVICE, NaClInputDeviceSensorID::KEYBOARD,		"keyboard" );
		AKUSetInputDevicePointer		( NaClInputDeviceID::DEVICE, NaClInputDeviceSensorID::POINTER,		"pointer" );

		AKUSetInputDeviceWheel			( NaClInputDeviceID::DEVICE, NaClInputDeviceSensorID::WHEEL,		"wheel" );

		AKUSetInputDeviceButton			( NaClInputDeviceID::DEVICE, NaClInputDeviceSensorID::MOUSE_LEFT,	"mouseLeft" );
		AKUSetInputDeviceButton			( NaClInputDeviceID::DEVICE, NaClInputDeviceSensorID::MOUSE_MIDDLE,	"mouseMiddle" );
		AKUSetInputDeviceButton			( NaClInputDeviceID::DEVICE, NaClInputDeviceSensorID::MOUSE_RIGHT,	"mouseRight" );

		RequestInputEvents ( PP_INPUTEVENT_CLASS_MOUSE );
		RequestInputEvents ( PP_INPUTEVENT_CLASS_KEYBOARD );
		RequestInputEvents ( PP_INPUTEVENT_CLASS_WHEEL );
		NACL_LOG ( "Input Initialized\n" );

		AKUSetFunc_EnterFullscreenMode ( _AKUEnterFullscreenModeFunc );
		AKUSetFunc_ExitFullscreenMode ( _AKUExitFullscreenModeFunc );
		AKUSetFunc_OpenWindow ( _AKUOpenWindowFunc );

		NACL_LOG ( "AKU functions registered Initialized\n" );

		AKUFmodExInit ();
		NACL_LOG ( "Fmod Initialized\n" );

		AKUDetectGfxContext ();
		NACL_LOG ( "AKUDetectGfxContext\n" );

		REGISTER_LUA_CLASS ( MOAIApp )

		pthread_create( &gThreadId, NULL, moai_main, g_instance );
		NACL_LOG ( "Main Thread Created.\n" );

		g_bInitialized = 1;
	}

	//TODO remove hardcode
	//AKUSetScreenSize ( g_width, g_height );
	//AKUSetViewSize ( g_width, g_height );
	//MOAIGfxDevice::Get ().SetDeviceScale ( g_width / MOAIGfxDevice::Get ().GetWidth ());

	NACL_LOG ( "AKUOpenWindowFunc: %d, %d\n", g_expectedWidth, g_width );

	if ( g_expectedWidth && g_width ) {
		MOAIGfxDevice::Get ().SetBufferScale ( g_width / ( float )g_expectedWidth );
	}
	else {
		MOAIGfxDevice::Get ().SetBufferScale ( 1.0f );
	}
}

//----------------------------------------------------------------//
void MoaiInstance::DrawSelf() {

	if ( !opengl_context->flush_pending () ) {

		opengl_context->MakeContextCurrent(this);
		
		AKURender ();

		//glFinish ();

		opengl_context->FlushContext();
	}

}

//----------------------------------------------------------------//
void MoaiInstance::HandleMessage ( const pp::Var& var_message ) {
  
	if ( !var_message.is_string ()) {
		return;
	}

	std::string message = var_message.AsString ();

	//AJV send to queue and proccess on background thread ( due to thread safety issues / file I/O )
	g_MessageQueue->Push ( message );
}

//----------------------------------------------------------------//
class MoaiModule : public pp::Module {
public:
	MoaiModule () : pp::Module () {}

	virtual ~MoaiModule () {
		glTerminatePPAPI();
	}

	virtual bool Init () {

		g_core = core ();
		return glInitializePPAPI ( get_browser_interface ()) == GL_TRUE;
	}

	virtual pp::Instance* CreateInstance ( PP_Instance instance ) {
		return new MoaiInstance ( instance );
	}
};

namespace pp {

Module* CreateModule () {
  return new MoaiModule ();
}
}
