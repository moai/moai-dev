// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIInputMgr.h>
#include <moaicore/MOAINodeMgr.h>
#include <moaicore/MOAIProp2D.h>
#include <moaicore/MOAISim.h>
#include <moaicore/MOAIFmod.h>
#include <aku/AKU.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name clearRenderStack
	@text Clears the render stack.
	@return nil
*/
int MOAISim::_clearRenderStack ( lua_State* L ) {

	USLuaState state ( L );
	
	MOAISim& device = MOAISim::Get ();
	device.Clear ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name enterFullscreenMode
	@text Enters fullscreen mode on the device if possible.
	@return nil
*/
int MOAISim::_enterFullscreenMode ( lua_State* L ) {

	USLuaState state ( L );

	AKUEnterFullscreenModeFunc enterFullscreenMode = AKUGetFunc_EnterFullscreenMode ();
	if ( enterFullscreenMode ) {
		enterFullscreenMode ();
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name exitFullscreenMode
	@text Exits fullscreen mode on the device if possible.
	@return nil
*/
int MOAISim::_exitFullscreenMode ( lua_State* L ) {

	USLuaState state ( L );

	AKUEnterFullscreenModeFunc exitFullscreenMode = AKUGetFunc_ExitFullscreenMode ();
	if ( exitFullscreenMode ) {
		exitFullscreenMode ();
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name framesToTime
	@param1 frames @type number @text The number of frames.
	@text Converts the number of frames to time passed in seconds.
	@return The equivilant number of seconds for the specified number of frames. @type number
*/
int MOAISim::_framesToTime ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "N" )) return 0;
	
	float frames = state.GetValue < float >( 1, 0.0f );
	
	MOAISim& device = MOAISim::Get ();
	lua_pushnumber ( state, frames * device.mStep );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name getDeviceIDString
	@text Gets a unique string that identifies the device.
	@return The identifier string. @type string
*/
int MOAISim::_getDeviceIDString ( lua_State* L ) {

	#ifdef _WIN32
		USAdapterInfoList infoList;
		
		infoList.EnumerateAdapters();
		STLString name = infoList[0].GetName();
		if( name ){
		
			lua_pushstring( L, name );
			return 1;
		}
	#else 
		MOAISim& device = MOAISim::Get ();
		if( device.mUniqueID ){
		
			lua_pushstring( L, device.mUniqueID );
			return 1;
		}
	#endif
	return 0;
}

//----------------------------------------------------------------//
/**	@name getDeviceSize
	@text Gets the dimensions of the device screen as two return values (width, height).
	@return The width and height of the device screen. @type (number,number)
*/
int MOAISim::_getDeviceSize ( lua_State* L ) {

	USGfxDevice& gfxDevice = USGfxDevice::Get ();
	
	lua_pushnumber ( L, gfxDevice.GetWidth ());
	lua_pushnumber ( L, gfxDevice.GetHeight ());

	return 2;
}

//----------------------------------------------------------------//
/**	@name getElapsedFrames
	@text Gets the number of frames elapsed since the application was started.
	@return The number of elapsed frames. @type number
*/
int MOAISim::_getElapsedFrames ( lua_State* L ) {
	
	MOAISim& device = MOAISim::Get ();
	lua_pushnumber ( L, device.mTime / device.mStep );
	return 1;
}

//----------------------------------------------------------------//
/**	@name getElapsedTime
	@text Gets the number of seconds elapsed since the application was started.
	@return The number of elapsed seconds. @type number
*/
int MOAISim::_getElapsedTime ( lua_State* L ) {
	
	lua_pushnumber ( L, MOAISim::Get ().mTime );
	return 1;
}

//----------------------------------------------------------------//
/**	@name getFrameSize
	@text Gets the amount of time (in seconds) that it takes for one frame to pass.  This often will be a decimal number between 0 and 1.
	@return The size of the frame; the time it takes for one frame to pass. @type number
*/
int MOAISim::_getFrameSize ( lua_State* L ) {
	
	MOAISim& device = MOAISim::Get ();
	lua_pushnumber ( L, device.mStep );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name getNetworkStatus
	@text Returns whether this device is currently able to reach the internet.
	@return Whether the device can reach the internet. @type boolean
*/
int	MOAISim::_getNetworkStatus ( lua_State* L ) {

	MOAISim& device = MOAISim::Get ();
	lua_pushboolean( L, device.mHasNetwork );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name openWindow
	@param1 title @type string @text The title of the window.
	@param2 width @type number @text The width of the window in pixels.
	@param3 height @type number @text The height of the window in pixels.
	@text Opens a new window for the application to render on.  This must be called before any rendering can be done, and it must only be called once.
	@return nil
*/
int MOAISim::_openWindow ( lua_State* L ) {
	
	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "SNN" )) return 0;
	
	cc8* title = lua_tostring ( state, 1 );
	u32 width = state.GetValue < u32 >( 2, 320 );
	u32 height = state.GetValue < u32 >( 3, 480 );
	
	USGfxDevice::Get ().SetSize ( width, height );

	AKUOpenWindowFunc openWindow = AKUGetFunc_OpenWindow ();
	if ( openWindow ) {
		openWindow ( title, width, height );
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name pauseTimer
	@param1 pause @type boolean @text Whether the device timer should be paused.
	@text Pauses or unpauses the device timer, preventing any visual updates (rendering) while paused.
	@return nil
*/
int MOAISim::_pauseTimer ( lua_State* L ) {
	
	USLuaState state ( L );
	bool pause = state.GetValue < bool >( 1, true );
	
	if ( pause ) {
		MOAISim::Get ().mTimerState = MOAISim::RESUMING;
	}
	else {
		MOAISim::Get ().mTimerState = MOAISim::RUNNING;
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name popRenderPass
	@text Pops the rendering prim off the stack?
	@return nil
*/
int MOAISim::_popRenderPass ( lua_State* L ) {
	UNUSED ( L );
	
	MOAISim& device = MOAISim::Get ();
	device.PopRenderPass ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name pushRenderPass
	@param1 prim @type userdata @text The viewport of the render prim?
	@text Pushes the specified onto the render stack?
	@return nil
*/
int MOAISim::_pushRenderPass ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;
	
	MOAIProp2D* prop = state.GetLuaObject < MOAIProp2D >( 1 );
	if ( !prop ) return 0;
	
	MOAISim& device = MOAISim::Get ();
	device.PushRenderPass ( prop );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name setClearColor
	@param1 red @type number @text The red value of the color.
	@param2 green @type number @text The green value of the color.
	@param3 blue @type number @text The blue value of the color.
	@param4 alpha @type number @text The alpha value of the color.
	@text At the start of each frame the device will by default automatically render a background color.  Using this function you can set the background color that is drawn each frame.  If you specify no arguments to this function, then automatic redraw of the background color will be turned off (i.e. the previous render will be used as the background).
	@return nil
*/
int MOAISim::_setClearColor ( lua_State* L ) {

	USLuaState state ( L );
	MOAISim& sim = MOAISim::Get ();
	
	if ( state.GetTop () == 0 ) {
		sim.mClearFlags &= ~GL_COLOR_BUFFER_BIT;
	}
	else {
		float r = state.GetValue < float >( 1, 0.0f );
		float g = state.GetValue < float >( 2, 0.0f );
		float b = state.GetValue < float >( 3, 0.0f );
		float a = state.GetValue < float >( 4, 1.0f );
		
		sim.mClearColor = USColor::PackRGBA ( r, g, b, a );
		sim.mClearFlags |= GL_COLOR_BUFFER_BIT;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name setClearDepth
	@param1 clearDepth @type boolean @text Whether to clear the depth buffer each frame.
	@text At the start of each frame the device will by default automatically clear the depth buffer.  This function sets whether or not the depth buffer should be cleared at the start of each frame.
	@return nil
*/
int MOAISim::_setClearDepth ( lua_State* L ) {

	USLuaState state ( L );
	MOAISim& sim = MOAISim::Get ();
	
	bool clearDepth = state.GetValue < bool >( 1, false );
	
	if ( clearDepth ) {
		sim.mClearFlags |= GL_DEPTH_BUFFER_BIT;
	}
	else {
		sim.mClearFlags &= ~GL_DEPTH_BUFFER_BIT;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name setFrameSize
	@param1 size @type number @text The frame size (how long in seconds it takes for one frame to be rendered).
	@text Sets the amount of time it takes for one frame to pass.  This in effect can be used to set the FPS limit of the application by passing (1 / FPS).
	@return nil
*/
int MOAISim::_setFrameSize ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "N" )) return 0;
	
	MOAISim& device = MOAISim::Get ();
	device.mStep = state.GetValue < double >( 1, device.mStep );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name timeToFrames
	@param1 time @type number @text The number of seconds.
	@text Converts the number of time passed in seconds to frames.
	@return The equivilant number of frames for the specified number of seconds. @type number
*/
int MOAISim::_timeToFrames ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "N" )) return 0;
	
	float time = state.GetValue < float >( 1, 0.0f );
	
	MOAISim& device = MOAISim::Get ();
	lua_pushnumber ( state, time / device.mStep );
	
	return 0;
}

//================================================================//
// MOAISim
//================================================================//

//----------------------------------------------------------------//
void MOAISim::Clear () {

	while ( this->mRenderPasses.Count ()) {
		this->PopRenderPass ();
	}
}

//----------------------------------------------------------------//
MOAISim::MOAISim () :
	mTimerState ( RESUMING ),
	mDeviceTime ( 0.0f ),
	mStep ( 0.01f ),
	mTime ( 0.0f ),
	mClearFlags ( GL_COLOR_BUFFER_BIT ),
	mClearColor ( 0xff000000 ),
	mHasNetwork ( false ) {
	
	RTTI_SINGLE ( USLuaObject )

	this->mDeviceTime = USDeviceTime::GetTimeInSeconds ();
	
	// Start Lua
	USLuaRuntime& luaRuntime = USLuaRuntime::Get ();

	luaRuntime.Open ();
	luaRuntime.LoadLibs ( "moai" );
}

//----------------------------------------------------------------//
MOAISim::~MOAISim () {

	//this->mDataIOThread.Stop ();
	this->Clear ();
}

//----------------------------------------------------------------//
void MOAISim::PauseMOAI () {

	this->mTimerState = PAUSED;
}

//----------------------------------------------------------------//
void MOAISim::PopRenderPass () {

	if ( this->mRenderPasses.Count ()) {
		MOAIProp2D* prop = this->mRenderPasses.Back ();
		this->mRenderPasses.PopBack ();
		prop->Release ();
	}
}

//----------------------------------------------------------------//
void MOAISim::PushRenderPass ( MOAIProp2D* prop ) {

	if ( prop ) {
		if ( !this->mRenderPasses.Contains ( prop )) {
			prop->Retain ();
			this->mRenderPasses.PushBack ( prop );
		}
	}
}

//----------------------------------------------------------------//
void MOAISim::RegisterLuaClass ( USLuaState& state ) {

	LuaReg regTable [] = {
		{ "clearRenderStack",			_clearRenderStack },
		{ "enterFullscreenMode",		_enterFullscreenMode },
		{ "exitFullscreenMode",			_exitFullscreenMode },
		{ "framesToTime",				_framesToTime },
		{ "getDeviceIDString",			_getDeviceIDString },
		{ "getDeviceSize",				_getDeviceSize },
		{ "getElapsedFrames",			_getElapsedFrames },
		{ "getElapsedTime",				_getElapsedTime },
		{ "getFrameSize",				_getFrameSize },
		{ "getNetworkStatus",			_getNetworkStatus },
		{ "openWindow",					_openWindow },
		{ "pauseTimer",					_pauseTimer },
		{ "popRenderPass",				_popRenderPass },
		{ "pushRenderPass",				_pushRenderPass },
		{ "setClearColor",				_setClearColor },
		{ "setClearDepth",				_setClearDepth },
		{ "setFrameSize",				_setFrameSize },
		{ "timeToFrames",				_timeToFrames },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAISim::Render () {

	if ( this->mClearFlags & GL_COLOR_BUFFER_BIT ) {
	
		USColorVec clearColor;
		clearColor.SetRGBA ( this->mClearColor );
	
		glClearColor (
			clearColor.mR,
			clearColor.mG,
			clearColor.mB,
			clearColor.mA 
		);
	}

	if ( this->mClearFlags ) {
		glClear ( this->mClearFlags );
	}

	USDrawBuffer::Get ().Reset ();

	RenderPassIt passIt = this->mRenderPasses.Head ();
	for ( ; passIt; passIt = passIt->Next ()) {
		MOAIProp2D* renderPass = passIt->Data ();
		USCanvas::BeginDrawing ();
		renderPass->Draw ();
	}
	
	USDrawBuffer::Get ().Flush ();
}

//----------------------------------------------------------------//
void MOAISim::ResumeMOAI() {

	if ( this->mTimerState == PAUSED ) {
		this->mTimerState = RESUMING;
	}
}

//----------------------------------------------------------------//
void MOAISim::RunFile ( cc8* filename ) {

	int status;
	USLuaStateHandle state = USLuaRuntime::Get ().State ();
	
	status = luaL_loadfile ( state, filename );
	if ( state.PrintErrors ( status )) return;
	
	this->mRenderPasses.Clear ();
	MOAIActionMgr::Get ().Clear ();
	this->mTime = 0.0f;
	this->mDeviceTime = 0.0;
	
	state.DebugCall ( 0, 0 );

	AKUStartGameLoopFunc startGameLoop = AKUGetFunc_StartGameLoop ();
	if ( startGameLoop ) {
		startGameLoop ();
	}
}

//----------------------------------------------------------------//
void MOAISim::RunString ( cc8* script ) {

	int status;
	USLuaStateHandle state = USLuaRuntime::Get ().State ();
	
	status = luaL_loadstring ( state, script );
	if ( state.PrintErrors ( status )) return;
	
	this->mRenderPasses.Clear ();
	MOAIActionMgr::Get ().Clear ();
	this->mTime = 0.0f;
	this->mDeviceTime = 0.0;
	
	state.DebugCall ( 0, 0 );

	AKUStartGameLoopFunc startGameLoop = AKUGetFunc_StartGameLoop ();
	if ( startGameLoop ) {
		startGameLoop ();
	}
}

//----------------------------------------------------------------//
void MOAISim::SetReachability ( bool networkStatus ) {

	mHasNetwork = networkStatus;
}

//----------------------------------------------------------------//
void MOAISim::SetUniqueIdentifier ( cc8* uniqueID ) {

	mUniqueID = uniqueID;
}

//----------------------------------------------------------------//
void MOAISim::Update () {

	this->mDeviceTime = USDeviceTime::GetTimeInSeconds ();

	if ( this->mTimerState != RUNNING ) {
		this->mTime = this->mDeviceTime;
		
		if ( this->mTimerState == RESUMING ) {
			this->mTimerState = RUNNING;
			
			MOAIDebugLines::Get ().Reset ();
			MOAIInputMgr::Get ().Update ();
			MOAIActionMgr::Get ().Update ( 0.0f );
			MOAINodeMgr::Get ().Update ();
		}
		else {
			return;
		}
	}

	USLuaStateHandle state = USLuaRuntime::Get ().State ();

	while (( this->mTime + this->mStep ) < this->mDeviceTime ) {
	
		MOAIDebugLines::Get ().Reset ();
		MOAIInputMgr::Get ().Update ();
		MOAIActionMgr::Get ().Update (( float )this->mStep );
		MOAINodeMgr::Get ().Update ();
		
		this->mTime += this->mStep;
	}
	
	USUrlMgr::Get ().Process ();
	
	#if USE_FMOD
		MOAIFmod::Get ().Update ();
	#endif
	
	this->mDataIOThread.Publish ();
	
	lua_gc ( state, LUA_GCCOLLECT, 0 );
}

//----------------------------------------------------------------//
STLString MOAISim::ToString () {

	STLString repr;

	const char *timer_state;

	switch ( mTimerState ) {
		case PAUSED:
			timer_state = "paused";
			break;
		case RESUMING:
			timer_state = "resuming";
			break;
		case RUNNING:
			timer_state = "running";
			break;
		default:
			timer_state = "INVALID";
	}

	PrettyPrint ( repr, "mTimerState", timer_state );
	PRETTY_PRINT ( repr, mTime )
	PRETTY_PRINT ( repr, mDeviceTime )
	PRETTY_PRINT ( repr, mHasNetwork )
	PRETTY_PRINT ( repr, mUniqueID )

	return repr;
}