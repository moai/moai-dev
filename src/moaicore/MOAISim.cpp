// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIInputMgr.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAINodeMgr.h>
#include <moaicore/MOAIProp2D.h>
#include <moaicore/MOAISim.h>
#include <aku/AKU.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	clearRenderStack
	@text	Clears the render stack.

	@out	nil
*/
int MOAISim::_clearRenderStack ( lua_State* L ) {

	USLuaState state ( L );
	
	MOAISim& device = MOAISim::Get ();
	device.Clear ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	enterFullscreenMode
	@text	Enters fullscreen mode on the device if possible.

	@out	nil
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
/**	@name	exitFullscreenMode
	@text	Exits fullscreen mode on the device if possible.

	@out	nil
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
/**	@name	framesToTime
	@text	Converts the number of frames to time passed in seconds.

	@in		number frames		The number of frames.
	@out	number time			The equivilant number of seconds for the specified number of frames.
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
/**	@name	getDeviceIDString
	@text	Gets a unique string that identifies the device.

	@out	string id			The identifier string.	
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
/**	@name	getDeviceSize
	@text	Gets the dimensions of the device screen as two return values (width, height).

	@out	number width		The width of the device screen.
	@out	number height		The height of the device screen.
*/
int MOAISim::_getDeviceSize ( lua_State* L ) {

	USGfxDevice& gfxDevice = USGfxDevice::Get ();
	
	lua_pushnumber ( L, gfxDevice.GetWidth ());
	lua_pushnumber ( L, gfxDevice.GetHeight ());

	return 2;
}

//----------------------------------------------------------------//
/**	@name	getElapsedFrames
	@text	Gets the number of frames elapsed since the application was started.

	@out	number frames		The number of elapsed frames.
*/
int MOAISim::_getElapsedFrames ( lua_State* L ) {
	
	MOAISim& device = MOAISim::Get ();
	lua_pushnumber ( L, device.mTime / device.mStep );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getElapsedTime
	@text	Gets the number of seconds elapsed since the application was started.

	@out	number time			The number of elapsed seconds.
*/
int MOAISim::_getElapsedTime ( lua_State* L ) {
	
	lua_pushnumber ( L, MOAISim::Get ().mTime );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getFrameSize
	@text	Gets the amount of time (in seconds) that it takes for one frame to pass.  This often will be a decimal number between 0 and 1.

	@out	number size			The size of the frame; the time it takes for one frame to pass.
*/
int MOAISim::_getFrameSize ( lua_State* L ) {
	
	MOAISim& device = MOAISim::Get ();
	lua_pushnumber ( L, device.mStep );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getNetworkStatus
	@text	Returns whether this device is currently able to reach the internet.

	@out	boolean status		Whether the device can reach the internet.
*/
int	MOAISim::_getNetworkStatus ( lua_State* L ) {

	MOAISim& device = MOAISim::Get ();
	lua_pushboolean ( L, device.mHasNetwork );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getPerformance
	@text	Returns an estimated frames per second based on measurements
			taked at every render.

	@out	number fps		Estimated frames per second.
*/
int MOAISim::_getPerformance ( lua_State* L ) {

	MOAISim& device = MOAISim::Get ();
	lua_pushnumber ( L, device.mFrameRate );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	openWindow
	@text	Opens a new window for the application to render on.  This must be called before any rendering can be done, and it must only be called once.

	@in		string title		The title of the window.
	@in		number width		The width of the window in pixels.
	@in		number height		The height of the window in pixels.
	@out	nil
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
/**	@name	pauseTimer
	@text	Pauses or unpauses the device timer, preventing any visual updates (rendering) while paused.

	@in		boolean pause		Whether the device timer should be paused.
	@out	nil
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
/**	@name	popRenderPass
	@text	Pops the rendering prim off the stack.

	@out	nil
*/
int MOAISim::_popRenderPass ( lua_State* L ) {
	UNUSED ( L );
	
	MOAISim& device = MOAISim::Get ();
	device.PopRenderPass ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	pushRenderPass
	@text	Pushes the specified prim onto the render stack.

	@in		MOAIProp2D prop		The viewport of the render prim.
	@out	nil
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
/**	@name	setClearColor
	@text	At the start of each frame the device will by default automatically render a background color.  Using this function you can set the background color that is drawn each frame.  If you specify no arguments to this function, then automatic redraw of the background color will be turned off (i.e. the previous render will be used as the background).

	@opt	number red			The red value of the color.
	@opt	number green		The green value of the color.
	@opt	number blue			The blue value of the color.
	@opt	number alpha		The alpha value of the color.
	@out	nil
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
/**	@name	setClearDepth
	@text	At the start of each frame the device will by default automatically clear the depth buffer.  This function sets whether or not the depth buffer should be cleared at the start of each frame.

	@in		boolean clearDepth	Whether to clear the depth buffer each frame.
	@out	nil
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
/**	@name	setFrameSize
	@text	Sets the amount of time it takes for one frame to pass.  This in effect can be used to set the FPS limit of the application by passing (1 / FPS).

	@in		number size			The frame size (how long in seconds it takes for one frame to be rendered).
	@out	nil
*/
int MOAISim::_setFrameSize ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "N" )) return 0;
	
	MOAISim& device = MOAISim::Get ();
	device.mStep = state.GetValue < double >( 1, device.mStep );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	timeToFrames
	@text	Converts the number of time passed in seconds to frames.

	@in		number time			The number of seconds.
	@out	number frames		The equivilant number of frames for the specified number of seconds.
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
	mFrameTime ( 0.0 ),
	mFrameRate ( 0.0f ),
	mFrameRateIdx ( 0 ),
	mClearFlags ( GL_COLOR_BUFFER_BIT ),
	mClearColor ( 0xff000000 ),
	mHasNetwork ( false ) {
	
	RTTI_SINGLE ( USLuaObject )

	this->mDeviceTime = USDeviceTime::GetTimeInSeconds ();
	
	// Start Lua
	USLuaRuntime& luaRuntime = USLuaRuntime::Get ();

	luaRuntime.Open ();
	luaRuntime.LoadLibs ( "moai" );
	
	for ( u32 i = 0; i < FPS_BUFFER_SIZE; ++i ) {
		this->mFrameRateBuffer [ i ] = 0.0f;
	}
}

//----------------------------------------------------------------//
MOAISim::~MOAISim () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAISim::MeasureFrameRate () {

	double delay = USDeviceTime::GetTimeInSeconds () - this->mFrameTime;
	this->mFrameTime = USDeviceTime::GetTimeInSeconds ();
	
	if ( delay > 0.0 ) {
	
		float sample = ( float )( 1.0 / delay );
		
		this->mFrameRateBuffer [ this->mFrameRateIdx++ ] = sample;
		this->mFrameRateIdx %= FPS_BUFFER_SIZE;
		
		sample = 0.0f;
		for ( u32 i = 0; i < FPS_BUFFER_SIZE; ++i ) {
			sample += this->mFrameRateBuffer [ i ];
		}
		this->mFrameRate = sample / ( float )FPS_BUFFER_SIZE;
	}
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

	luaL_Reg regTable [] = {
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
		{ "getPerformance",				_getPerformance },
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

	if ( !USFileSys::CheckFileExists ( filename )) return;

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

	this->MeasureFrameRate ();

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