// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIActionMgr.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIInputMgr.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAINodeMgr.h>
#include <moaicore/MOAIProp2D.h>
#include <moaicore/MOAISim.h>
#include <moaicore/MOAITexture.h>
#include <aku/AKU.h>

#if defined(_WIN32)
#include <Psapi.h>
#elif defined(__APPLE__) //&& defined(TARGET_IPHONE_SIMULATOR)
// Not sure if using mach API is disallowed in the app store. :/
#include <mach/mach.h>
#endif



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
/**	@name	flagLongLoad
	@text	Lets the sim know to skip the catch up process after a long load

	@out	nil
*/
int MOAISim::_flagLongLoad ( lua_State* L ) {
	UNUSED ( L );
	
	MOAISim& device = MOAISim::Get ();
	device.mLongLoadFlag = true;
		
	return 0;
}

//----------------------------------------------------------------//
/**	@name forceGarbageCollection
	@text	Runs the garbage collector repeatedly until no more USLuaObjects
			can be collected.

	@out	nil
*/
int MOAISim::_forceGarbageCollection ( lua_State* L ) {
	UNUSED ( L );

	MOAINodeMgr::Get ().Update ();
	USLuaRuntime::Get ().ForceGarbageCollection ();
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
/**	@name	getDeviceSize
	@text	Gets the dimensions of the device screen as two return values (width, height).

	@out	number width		The width of the device screen.
	@out	number height		The height of the device screen.
*/
int MOAISim::_getDeviceSize ( lua_State* L ) {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	lua_pushnumber ( L, gfxDevice.GetWidth ());
	lua_pushnumber ( L, gfxDevice.GetHeight ());

	return 2;
}

//----------------------------------------------------------------//
/**	@name	getDeviceTime
	@text	Gets the raw device clock. This is a replacement for Lua's os.time ().

	@out	number time			The device clock time in seconds.
*/
int MOAISim::_getDeviceTime ( lua_State* L ) {
	
	lua_pushnumber ( L, USDeviceTime::GetTimeInSeconds ());
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getElapsedFrames
	@text	Gets the number of frames elapsed since the application was started.

	@out	number frames		The number of elapsed frames.
*/
int MOAISim::_getElapsedFrames ( lua_State* L ) {
	
	MOAISim& device = MOAISim::Get ();
	lua_pushnumber ( L, device.mSimTime / device.mStep );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getElapsedTime
	@text	Gets the number of seconds elapsed since the application was started.

	@out	number time			The number of elapsed seconds.
*/
int MOAISim::_getElapsedTime ( lua_State* L ) {
	
	lua_pushnumber ( L, MOAISim::Get ().mSimTime );
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
/**	@name	getLuaObjectCount
	@text	Gets the total number of objects in memory that inherit USLuaObject. Count includes
			objects that are not bound to the Lua runtime.

	@out	number count
*/
int MOAISim::_getLuaObjectCount ( lua_State* L ) {
	lua_pushnumber ( L, USLuaRuntime::Get ().GetObjectCount ());
	return 1;
}

//----------------------------------------------------------------//
/** @name	getMemoryUsage
	@text	Get the current amount of memory used by MOAI and its subsystems. This will
			attempt to return reasonable estimates where exact values cannot be obtained.
			Some fields represent informational fields (i.e. are not double counted in the
			total, but present to assist debugging) and may be only available on certain
			platforms (e.g. Windows, etc). These fields begin with a '_' character.
 
	@out	table	usage		The breakdown of each subsystem's memory usage, in bytes. There is also a "total" field that contains the summed value.
*/
int MOAISim::_getMemoryUsage ( lua_State* L ) {
	
	float divisor = 1.0f;
	
	if( lua_type(L, 1) == LUA_TSTRING )
	{
		cc8* str = lua_tostring(L, 1);
		if( str[0] == 'k' || str[0] == 'K' )
			divisor = 1024.0f;
		else if( str[0] == 'm' || str[0] == 'M' )
			divisor = 1024.0f * 1024.0f;
		else if( str[0] == 'b' || str[0] == 'B' )
			divisor = 1.0f;
	}
	
	size_t total = 0;
	
	lua_newtable(L);
	
	size_t count;
	
	count = USLuaRuntime::Get().GetMemoryUsage ();
	lua_pushnumber(L, count / divisor);
	lua_setfield(L, -2, "lua");
	total += count;

	// This is informational only (i.e. don't double count with the previous field).
	// It doesn't actually seem to represent the real usage of lua, but maybe
	// someone is interested.
	lua_pushnumber ( L, lua_gc ( L, LUA_GCCOUNTB, 0 ) / divisor );
	lua_setfield ( L, -2, "_luagc_count" );
	
	count = MOAIGfxDevice::Get ().GetTextureMemoryUsage ();
	lua_pushnumber ( L, count / divisor );
	lua_setfield ( L, -2, "texture" );
	total += count;
	
#if defined(_WIN32)
    PROCESS_MEMORY_COUNTERS pmc;

    // Print the process identifier.
    if ( GetProcessMemoryInfo( GetCurrentProcess(), &pmc, sizeof(pmc)) )
    {
		lua_pushnumber(L, pmc.PagefileUsage / divisor);
		lua_setfield(L, -2, "_sys_vs");
		lua_pushnumber(L, pmc.WorkingSetSize / divisor);
		lua_setfield(L, -2, "_sys_rss");
    }
#elif defined(__APPLE__) //&& defined(TARGET_IPHONE_SIMULATOR) 
	// Tricky undocumented mach polling of memory
	struct task_basic_info t_info;
	mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
	kern_return_t kr = task_info(mach_task_self(),
								 TASK_BASIC_INFO,
								 reinterpret_cast<task_info_t>(&t_info),
								 &t_info_count);
	// Most likely cause for failure: |task| is a zombie.
	if( kr == KERN_SUCCESS )
	{
		lua_pushnumber(L, t_info.virtual_size / divisor);
		lua_setfield(L, -2, "_sys_vs");
		lua_pushnumber(L, t_info.resident_size / divisor);
		lua_setfield(L, -2, "_sys_rss");
	}
#endif
	
	lua_pushnumber(L, total / divisor);
	lua_setfield(L, -2, "total");
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getPerformance
	@text	Returns an estimated frames per second based on measurements
			taken at every render.

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
	
	MOAIGfxDevice::Get ().SetSize ( width, height );

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
		MOAISim::Get ().mLoopState = MOAISim::PAUSED;
	}
	else {
		MOAISim::Get ().mLoopState = MOAISim::RUNNING;
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
/**	@name	reportHistogram
	@text	Generates a histogram of active USLuaObjects.

	@out	nil
*/
int MOAISim::_reportHistogram ( lua_State* L ) {
	USLuaState state ( L );
	USLuaRuntime::Get ().ReportHistogram ( MOAILogMgr::Get ().GetFile ());
	return 0;
}

//----------------------------------------------------------------//
/**	@name	reportLeaks
	@text	Analyze the currently allocated MOAI objects and create a textual
			report of where they were declared, and what Lua references (if any)
			can be found. NOTE: This is incredibly slow, so only use to debug
			leaking memory issues.
 
			This will also trigger a full garbage collection before performing
			the required report. (Equivalent of collectgarbage("collect").)
 
	@in		bool clearAfter	If true, it will reset the allocation tables (without
							freeing the underlying objects). This allows this
							method to be called after a known operation and
							get only those allocations created since the last call
							to this function.
	@out	nil
*/
int MOAISim::_reportLeaks ( lua_State* L ) {
	
	USLuaState state ( L );
	bool clearAfter = state.GetValue < bool >( 1, false );
	
	USLuaRuntime& luaRuntime = USLuaRuntime::Get ();
	luaRuntime.ReportLeaksFormatted ( MOAILogMgr::Get ().GetFile ());

	if ( clearAfter ) {
		luaRuntime.ResetLeakTracking ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setBoostThreshold
	@text	Sets the boost threshold, a scalar applied to step. If the gap
			between simulation time and device time is greater than the step
			size multiplied by the boost threshold and MOAISim.SIM_LOOP_ALLOW_BOOST
			is set in the loop flags, then the simulation is updated once with a
			large, variable step to make up the entire gap.

	@opt	number boostThreshold		Default value is 3.
	@out	nil
*/
int MOAISim::_setBoostThreshold ( lua_State* L ) {
	USLuaState state ( L );
	MOAISim::Get ().mBoostThreshold = state.GetValue < double >( 1, DEFAULT_BOOST_THRESHOLD );
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
/**	@name	setFramesToDo
	@text	Sets the number of frames to do for the next loops. This is used if you want to speed up the simulation temporarily
	
	@in		number count		The number of steps to take during the next loop
	@out	nil
*/
int MOAISim::_setFramesToDo ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "N" )) return 0;
	
	MOAISim& device = MOAISim::Get ();
	device.mFramesToDo = state.GetValue < u32 >( 1, 1 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setHistogramEnabled
	@text	Enable tracking of every USLuaObject so that an object count
			histogram may be generated.
 
	@opt	bool enable		Default value is false.
	@out	nil
*/
int MOAISim::_setHistogramEnabled ( lua_State* L ) {
	USLuaState state ( L );
	USLuaRuntime::Get ().EnableHistogram ( state.GetValue < bool >( 1, false ));
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLeakTrackingEnabled
	@text	Enable extra memory book-keeping measures that allow all MOAI objects to be
			tracked back to their point of allocation (in Lua). Use together with
			MOAISim.reportLeaks() to determine exactly where your memory usage is
			being created. NOTE: This is very expensive in terms of both CPU and
			the extra memory associated with the stack info book-keeping. Use only
			when tracking down leaks.
 
	@opt	bool enable		Default value is false.
	@out	nil
*/
int MOAISim::_setLeakTrackingEnabled ( lua_State* L ) {
	USLuaState state ( L );
	USLuaRuntime::Get ().EnableLeakTracking ( state.GetValue < bool >( 1, false ));
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLoopFlags
	@text	Fine tune behavior of the simulation loop. MOAISim.SIM_LOOP_ALLOW_SPIN
			will allow the simulation step to run multiple times per update to try
			and catch up with device time, but will abort if processing the simulation
			exceeds the configfured step time. MOAISim.SIM_LOOP_ALLOW_BOOST will permit
			a *variable* update step if simulation time falls too far behind
			device time (based on the boost threshold). Be warned: this can wreak
			havok with physics and stepwise animation or game AI.

	@opt	number flags		Mask or MOAISim.SIM_LOOP_ALLOW_BOOST, MOAISim.SIM_LOOP_ALLOW_SPIN. Default value is MOAISim.SIM_LOOP_ALLOW_SPIN.
	@out	nil
*/
int MOAISim::_setLoopFlags ( lua_State* L ) {
	USLuaState state ( L );
	USLuaRuntime::Get ().EnableLeakTracking ( state.GetValue < bool >( 1, false ));
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLuaAllocLogEnabled
	@text	Toggles log messages from Lua allocator.

	@opt	boolean enable			Default value is 'false.'
	@out	nil
*/
int MOAISim::_setLuaAllocLogEnabled ( lua_State* L ) {
	USLuaState state ( L );
	USLuaRuntime::Get ().SetAllocLogEnabled ( state.GetValue < bool >( 1, false ));
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
	
	return 1;
}

//================================================================//
// MOAISim
//================================================================//

const double MOAISim::DEFAULT_BOOST_THRESHOLD = 3.0;

//----------------------------------------------------------------//
void MOAISim::Clear () {

	while ( this->mRenderPasses.Count ()) {
		this->PopRenderPass ();
	}
}

//----------------------------------------------------------------//
MOAISim::MOAISim () :
	mLoopState ( START ),
	mStep ( 0.01f ),
	mSimTime ( 0.0f ),
	mBaseTime ( 0.0f ),
	mFrameTime ( 0.0 ),
	mRenderCounter ( 0 ),
	mFrameRate ( 0.0f ),
	mFrameRateIdx ( 0 ),
	mClearFlags ( GL_COLOR_BUFFER_BIT ),
	mClearColor ( 0xff000000 ),
	mLoopFlags ( DEFAULT_LOOP_FLAGS ),
	mBoostThreshold ( DEFAULT_BOOST_THRESHOLD ),
	mLongLoadFlag ( false ),
	mFramesToDo ( 1 ) {
	
	RTTI_SINGLE ( MOAIGlobalEventSource )
	
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

	this->mLoopState = PAUSED;
}

//----------------------------------------------------------------//
void MOAISim::PopRenderPass () {

	if ( this->mRenderPasses.Count ()) {
		MOAIProp2D* prop = this->mRenderPasses.Back ();
		this->mRenderPasses.PopBack ();
		this->LuaRelease ( *prop );
	}
}

//----------------------------------------------------------------//
void MOAISim::PushRenderPass ( MOAIProp2D* prop ) {

	if ( prop ) {
		if ( !this->mRenderPasses.Contains ( prop )) {
			this->LuaRetain ( *prop );
			this->mRenderPasses.PushBack ( prop );
		}
	}
}

//----------------------------------------------------------------//
void MOAISim::RegisterLuaClass ( USLuaState& state ) {

	state.SetField ( -1, "EVENT_FINALIZE", ( u32 )EVENT_FINALIZE );

	state.SetField ( -1, "SIM_LOOP_ALLOW_BOOST", ( u32 )SIM_LOOP_ALLOW_BOOST );
	state.SetField ( -1, "SIM_LOOP_ALLOW_SPIN", ( u32 )SIM_LOOP_ALLOW_SPIN );

	luaL_Reg regTable [] = {
		{ "clearRenderStack",			_clearRenderStack },
		{ "enterFullscreenMode",		_enterFullscreenMode },
		{ "exitFullscreenMode",			_exitFullscreenMode },
		{ "flagLongLoad",				_flagLongLoad },
		{ "forceGarbageCollection",		_forceGarbageCollection },
		{ "framesToTime",				_framesToTime },
		{ "getDeviceSize",				_getDeviceSize },
		{ "getDeviceTime",				_getDeviceTime },
		{ "getElapsedFrames",			_getElapsedFrames },
		{ "getElapsedTime",				_getElapsedTime },
		{ "getFrameSize",				_getFrameSize },
		{ "getLuaObjectCount",			_getLuaObjectCount },
		{ "getMemoryUsage",				_getMemoryUsage },
		{ "getPerformance",				_getPerformance },
		{ "openWindow",					_openWindow },
		{ "pauseTimer",					_pauseTimer },
		{ "popRenderPass",				_popRenderPass },
		{ "pushRenderPass",				_pushRenderPass },
		{ "reportHistogram",			_reportHistogram },
		{ "reportLeaks",				_reportLeaks },
		{ "setBoostThreshold",			_setBoostThreshold },
		{ "setClearColor",				_setClearColor },
		{ "setClearDepth",				_setClearDepth },
		{ "setFrameSize",				_setFrameSize },
		{ "setFramesToDo",				_setFramesToDo },
		{ "setHistogramEnabled",		_setHistogramEnabled },
		{ "setLeakTrackingEnabled",		_setLeakTrackingEnabled },
		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAISim > },
		{ "setLoopFlags",				_setLoopFlags },
		{ "setLuaAllocLogEnabled",		_setLuaAllocLogEnabled },
		{ "timeToFrames",				_timeToFrames },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAISim::RegisterLuaFuncs ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAISim::Render () {

	this->mRenderCounter++;

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

	RenderPassIt passIt = this->mRenderPasses.Head ();
	for ( ; passIt; passIt = passIt->Next ()) {
		MOAIProp2D* renderPass = passIt->Data ();
		
		MOAIGfxDevice::Get ().BeginDrawing ();
		renderPass->Draw ();
	}
	
	MOAIGfxDevice::Get ().Flush ();
}

//----------------------------------------------------------------//
void MOAISim::ResumeMOAI() {

	if ( this->mLoopState == PAUSED ) {
		this->mLoopState = START;
	}
}

//----------------------------------------------------------------//
void MOAISim::RunFile ( cc8* filename ) {

	if ( !USFileSys::CheckFileExists ( filename )) return;

	int status;
	USLuaStateHandle state = USLuaRuntime::Get ().State ();
	
	status = luaL_loadfile ( state, filename );
	if ( state.PrintErrors ( USLog::CONSOLE, status )) return;
	
	this->mRenderPasses.Clear ();
	MOAIActionMgr::Get ().Clear ();
	this->mSimTime = 0.0f;
	
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
	if ( state.PrintErrors ( USLog::CONSOLE, status )) return;
	
	this->mRenderPasses.Clear ();
	MOAIActionMgr::Get ().Clear ();
	this->mSimTime = 0.0f;
	
	state.DebugCall ( 0, 0 );

	AKUStartGameLoopFunc startGameLoop = AKUGetFunc_StartGameLoop ();
	if ( startGameLoop ) {
		startGameLoop ();
	}
}

//----------------------------------------------------------------//
void MOAISim::SendFinalizeEvent () {

	USLuaStateHandle state = USLuaRuntime::Get ().State ();
	if ( this->PushListener ( EVENT_FINALIZE, state )) {
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
double MOAISim::StepSim ( double step ) {

	double time = USDeviceTime::GetTimeInSeconds ();

	for ( u32 s = 0; s < this->mFramesToDo; ++s ) {
		MOAIDebugLines::Get ().Reset ();
		MOAIInputMgr::Get ().Update ();
		MOAIActionMgr::Get ().Update (( float )step );
		MOAINodeMgr::Get ().Update ();
	}
	this->mFramesToDo = 1;
	
	this->mSimTime += step;
	
	return USDeviceTime::GetTimeInSeconds () - time;
}

//----------------------------------------------------------------//
void MOAISim::Update () {

	this->MeasureFrameRate ();
	
	// bail if we're paused
	if ( this->mLoopState == PAUSED ) {
		return;
	}
	
	// 'budget' will be used to measure the actual time each update takes
	// under no circumstances should we continue updating if we've exceeded the size of a single frame
	double budget = this->mStep;
	
	// the actual device time
	double realTime = USDeviceTime::GetTimeInSeconds () - this->mBaseTime;

	// reset sim time on start
	if ( this->mLoopState == START ) {
	
		this->mBaseTime = USDeviceTime::GetTimeInSeconds ();
		realTime = 0.0f;
	
		this->mSimTime = realTime;
		this->mLoopState = RUNNING;
		
		budget -= this->StepSim ( 0.0 );
	}


	// if we just had a long load, skip the updates for the time difference
	if ( mLongLoadFlag ) {
		
		this->mSimTime = realTime;
		this->mLongLoadFlag = false;
	}

	// 'gap' is the time left to make up between sim time and real time
	double gap = realTime - this->mSimTime;	

	if ( this->mLoopFlags & SIM_LOOP_ALLOW_BOOST ) {
		double boost = gap - ( this->mStep * this->mBoostThreshold );
		if ( boost > 0.0f ) {
			budget -= this->StepSim ( gap );
			gap = 0.0f;
		}
	}
	
	// single step
	if (( this->mStep <= gap ) && ( budget > 0.0 )) {
		budget -= this->StepSim ( this->mStep );
		gap -= this->mStep;
	}
		
	// spin to use up any additional budget
	if ( this->mLoopFlags & SIM_LOOP_ALLOW_SPIN ) {
		while (( this->mStep <= gap ) && ( budget > 0.0 )) {
			budget -= this->StepSim ( this->mStep );
			gap -= this->mStep;
		}
	}	

	// these stay out of the sim step for now
	USUrlMgr::Get ().Process ();
	this->mDataIOThread.Publish ();
}
