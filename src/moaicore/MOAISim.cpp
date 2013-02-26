// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIActionMgr.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIInputMgr.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAINodeMgr.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAISim.h>
#include <moaicore/MOAITextureBase.h>

#if USE_CURL
	#include <moaicore/MOAIUrlMgrCurl.h>
#endif

#if MOAI_OS_NACL
	#include <moaicore/MOAIUrlMgrNaCl.h>
#endif

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
/**	@name	clearLoopFlags
	@text	Uses the mask provided to clear the loop flags.

	@opt	number mask		Default value is 0xffffffff.
	@out	nil
*/
int MOAISim::_clearLoopFlags ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAISim::Get ().mLoopFlags &= ~state.GetValue < u32 >( 1, 0xffffffff );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	crash
	@text	Crashes moai with a null pointer dereference.
 
	@out	nil
*/
int MOAISim::_crash ( lua_State* L ) {
	UNUSED(L);
	
	int *p = NULL;
	(*p) = 0;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	enterFullscreenMode
	@text	Enters fullscreen mode on the device if possible.

	@out	nil
*/
int MOAISim::_enterFullscreenMode ( lua_State* L ) {

	MOAILuaState state ( L );

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

	MOAILuaState state ( L );

	AKUEnterFullscreenModeFunc exitFullscreenMode = AKUGetFunc_ExitFullscreenMode ();
	if ( exitFullscreenMode ) {
		exitFullscreenMode ();
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name forceGarbageCollection
	@text	Runs the garbage collector repeatedly until no more MOAIObjects
			can be collected.

	@out	nil
*/
int MOAISim::_forceGarbageCollection ( lua_State* L ) {
	UNUSED ( L );

	MOAINodeMgr::Get ().Update ();
	MOAILuaRuntime::Get ().ForceGarbageCollection ();
	return 0;
}

//----------------------------------------------------------------//
/**	@name	framesToTime
	@text	Converts the number of frames to time passed in seconds.

	@in		number frames		The number of frames.
	@out	number time			The equivilant number of seconds for the specified number of frames.
*/
int MOAISim::_framesToTime ( lua_State* L ) {

	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "N" )) return 0;
	
	float frames = state.GetValue < float >( 1, 0.0f );
	
	MOAISim& device = MOAISim::Get ();
	lua_pushnumber ( state, frames * device.mStep );
	
	return 1;
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
/**	@name	getHistogram
	@text	Generates a histogram of active MOAIObjects and returns it
			in a table containing object tallies indexed by object
			class names.

	@out	table histogram
*/
int MOAISim::_getHistogram ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAILuaRuntime::Get ().PushHistogram ( state );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getLoopFlags
	@text	Returns the current loop flags.

	@out	number mask
*/
int MOAISim::_getLoopFlags ( lua_State* L ) {
	lua_pushnumber ( L, MOAISim::Get ().mLoopFlags );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getLuaObjectCount
	@text	Gets the total number of objects in memory that inherit MOAILuaObject. Count includes
			objects that are not bound to the Lua runtime.

	@out	number count
*/
int MOAISim::_getLuaObjectCount ( lua_State* L ) {
	lua_pushnumber ( L, MOAILuaRuntime::Get ().GetObjectCount ());
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
	
	count = MOAILuaRuntime::Get().GetMemoryUsage ();
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
/**	@name	getStep
	@text	Gets the amount of time (in seconds) that it takes for one frame to pass.

	@out	number size			The size of the frame; the time it takes for one frame to pass.
*/
int MOAISim::_getStep ( lua_State* L ) {
	
	lua_pushnumber ( L, MOAISim::Get ().GetStep ());
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAISim::_getTaskSubscriber ( lua_State* L ) {

	MOAISim& device = MOAISim::Get ();
	MOAILuaState state ( L );
	device.mTaskSubscriber->PushLuaUserdata ( state );

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
	
	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "SNN" )) return 0;
	
	cc8* title = lua_tostring ( state, 1 );
	u32 width = state.GetValue < u32 >( 2, 320 );
	u32 height = state.GetValue < u32 >( 3, 480 );

	AKUOpenWindowFunc openWindow = AKUGetFunc_OpenWindow ();
	if ( openWindow ) {
		MOAIGfxDevice::Get ().SetBufferSize ( width, height );
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
	
	MOAILuaState state ( L );
	bool pause = state.GetValue < bool >( 1, true );
	
	if ( pause ) {
		MOAISim::Get ().PauseMOAI ();
	}
	else {
		MOAISim::Get ().ResumeMOAI ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	reportHistogram
	@text	Generates a histogram of active MOAIObjects.

	@out	nil
*/
int MOAISim::_reportHistogram ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAILuaRuntime::Get ().ReportHistogram ( MOAILogMgr::Get ().GetFile ());
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
	
	MOAILuaState state ( L );
	bool clearAfter = state.GetValue < bool >( 1, false );
	
	MOAILuaRuntime& luaRuntime = MOAILuaRuntime::Get ();
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

	@opt	number boostThreshold		Default value is DEFAULT_BOOST_THRESHOLD.
	@out	nil
*/
int MOAISim::_setBoostThreshold ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAISim::Get ().mBoostThreshold = state.GetValue < double >( 1, DEFAULT_BOOST_THRESHOLD );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setCpuBudget
	@text	Sets the amount of time (given in simulation steps) to allow
			for updating the simulation.
	
	@in		number budget	Default value is DEFAULT_CPU_BUDGET.
	@out	nil
*/
int MOAISim::_setCpuBudget ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAISim::Get ().mCpuBudget = state.GetValue < u32 >( 1, DEFAULT_CPU_BUDGET );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setHistogramEnabled
	@text	Enable tracking of every MOAILuaObject so that an object count
			histogram may be generated.
 
	@opt	bool enable		Default value is false.
	@out	nil
*/
int MOAISim::_setHistogramEnabled ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAILuaRuntime::Get ().EnableHistogram ( state.GetValue < bool >( 1, false ));
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
	MOAILuaState state ( L );
	MOAILuaRuntime::Get ().EnableLeakTracking ( state.GetValue < bool >( 1, false ));
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLongDelayThreshold
	@text	Sets the long delay threshold. If the sim step falls behind
			the given threshold, the deficit will be dropped: sim will
			neither spin nor boost to catch up.

	@opt	number longDelayThreshold		Default value is DEFAULT_LONG_DELAY_THRESHOLD.
	@out	nil
*/
int MOAISim::_setLongDelayThreshold ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAISim::Get ().mLongDelayThreshold = state.GetValue < double >( 1, DEFAULT_LONG_DELAY_THRESHOLD );
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
			
			Three presets are provided: MOAISim.LOOP_FLAGS_DEFAULT, MOAISim.LOOP_FLAGS_FIXED,
			and MOAISim.LOOP_FLAGS_MULTISTEP.

	@opt	number flags		Mask or a combination of MOAISim.SIM_LOOP_FORCE_STEP, MOAISim.SIM_LOOP_ALLOW_BOOST,
								MOAISim.SIM_LOOP_ALLOW_SPIN, MOAISim.SIM_LOOP_NO_DEFICIT, MOAISim.SIM_LOOP_NO_SURPLUS,
								MOAISim.SIM_LOOP_RESET_CLOCK. Default value is 0.
	@out	nil	
*/
int MOAISim::_setLoopFlags ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAISim::Get ().mLoopFlags |= state.GetValue < u32 >( 1, 0 );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLuaAllocLogEnabled
	@text	Toggles log messages from Lua allocator.

	@opt	boolean enable			Default value is 'false.'
	@out	nil
*/
int MOAISim::_setLuaAllocLogEnabled ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAILuaRuntime::Get ().SetAllocLogEnabled ( state.GetValue < bool >( 1, false ));
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setStep
	@text	Sets the size of each simulation step (in seconds).
	
	@in		number step		The step size. Default value is 1 / DEFAULT_STEPS_PER_SECOND.
	@out	nil
*/
int MOAISim::_setStep ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAISim::Get ().SetStep ( state.GetValue < double >( 1, 1.0 / ( double )DEFAULT_STEPS_PER_SECOND ));
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setStepMultiplier
	@text	Runs the simulation multiple times per step (but with a fixed
			step size). This is used to speed up the simulation without
			providing a larger step size (which could destabilize physics
			simulation).
	
	@in		number count		Default value is DEFAULT_STEP_MULTIPLIER.
	@out	nil
*/
int MOAISim::_setStepMultiplier ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAISim::Get ().mStepMultiplier = state.GetValue < u32 >( 1, DEFAULT_STEP_MULTIPLIER );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTimerError
	@text	Sets the tolerance for timer error. This is a multiplier of step.
			Timer error tolerance is step * timerError.
	
	@in		number timerError		Default value is 0.0.
	@out	nil
*/
int MOAISim::_setTimerError ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAISim::Get ().mTimerError = state.GetValue < double >( 1, 0.0 );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTraceback
	@text	Sets the function to call when a traceback occurs in lua
 
	@in		function callback		Function to execute when the traceback occurs
	@out	nil
*/
int MOAISim::_setTraceback ( lua_State* L ) {
	UNUSED ( L );
	
	MOAILuaRuntime::Get ().GetCustomTraceback().SetStrongRef ( MOAILuaRuntime::Get ().GetMainState(), 1 );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	timeToFrames
	@text	Converts the number of time passed in seconds to frames.

	@in		number time			The number of seconds.
	@out	number frames		The equivilant number of frames for the specified number of seconds.
*/
int MOAISim::_timeToFrames ( lua_State* L ) {

	MOAILuaState state ( L );
	if ( !state.CheckParams ( 1, "N" )) return 0;
	
	float time = state.GetValue < float >( 1, 0.0f );
	
	MOAISim& device = MOAISim::Get ();
	lua_pushnumber ( state, time / device.mStep );
	
	return 1;
}

//================================================================//
// DOXYGEN
//================================================================//

#ifdef DOXYGEN

	//----------------------------------------------------------------//
	/**	@name	clearRenderStack
		@text	Alias for MOAIRenderMgr.clearRenderStack (). THIS METHOD
				IS DEPRECATED AND WILL BE REMOVED IN A FUTURE RELEASE.

		@out	nil
	*/
	int MOAISim::_clearRenderStack ( lua_State* L ) {
	}
	
	//----------------------------------------------------------------//
	/**	@name	popRenderPass
		@text	Alias for MOAIRenderMgr.popRenderPass (). THIS METHOD
				IS DEPRECATED AND WILL BE REMOVED IN A FUTURE RELEASE.

		@out	nil
	*/
	int MOAISim::_popRenderPass ( lua_State* L ) {
	}
	
	//----------------------------------------------------------------//
	/**	@name	pushRenderPass
		@text	Alias for MOAIRenderMgr.pushRenderPass (). THIS METHOD
				IS DEPRECATED AND WILL BE REMOVED IN A FUTURE RELEASE.

		@in		MOAIRenderable renderable
		@out	nil
	*/
	int MOAISim::_pushRenderPass ( lua_State* L ) {
	}
	
	//----------------------------------------------------------------//
	/**	@name	removeRenderPass
		@text	Alias for MOAIRenderMgr.removeRenderPass (). THIS METHOD
				IS DEPRECATED AND WILL BE REMOVED IN A FUTURE RELEASE.

		@in		MOAIRenderable renderable
		@out	nil
	*/
	int MOAISim::_removeRenderPass ( lua_State* L ) {
	}

#endif

//================================================================//
// MOAISim
//================================================================//

//----------------------------------------------------------------//
MOAISim::MOAISim () :
	mLoopState ( START ),
	mStep ( 1.0 / ( double )DEFAULT_STEPS_PER_SECOND ),
	mSimTime ( 0.0 ),
	mRealTime ( 0.0 ),
	mFrameTime ( 0.0 ),
	mFrameRate ( 0.0f ),
	mFrameRateIdx ( 0 ),
	mLoopFlags ( LOOP_FLAGS_DEFAULT ),
	mBoostThreshold ( DEFAULT_BOOST_THRESHOLD ),
	mLongDelayThreshold ( DEFAULT_LONG_DELAY_THRESHOLD ),
	mCpuBudget ( DEFAULT_CPU_BUDGET ),
	mStepMultiplier ( DEFAULT_STEP_MULTIPLIER ),
	mTimerError ( 0.0 ) {
	
	RTTI_SINGLE ( MOAIGlobalEventSource )
	
	for ( u32 i = 0; i < FPS_BUFFER_SIZE; ++i ) {
		this->mFrameRateBuffer [ i ] = 0.0f;
	}
	
	this->mFrameTime = USDeviceTime::GetTimeInSeconds ();
	
	this->mTaskSubscriber.Set ( *this, new MOAITaskSubscriber ());
}

//----------------------------------------------------------------//
MOAISim::~MOAISim () {

	this->mTaskSubscriber.Set ( *this, 0 );
}

//----------------------------------------------------------------//
double MOAISim::MeasureFrameRate () {

	double frameTime = USDeviceTime::GetTimeInSeconds ();
	double delay = frameTime - this->mFrameTime;
	this->mFrameTime = frameTime;
	
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
	
	return delay;
}

//----------------------------------------------------------------//
void MOAISim::OnGlobalsFinalize () {
	this->SendFinalizeEvent ();
}

//----------------------------------------------------------------//
void MOAISim::OnGlobalsRestore () {
}

//----------------------------------------------------------------//
void MOAISim::OnGlobalsRetire () {
}

//----------------------------------------------------------------//
void MOAISim::PauseMOAI () {

	this->SendPauseEvent();
	this->mLoopState = PAUSED;
}

//----------------------------------------------------------------//
void MOAISim::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIGlobalEventSource::RegisterLuaClass ( state );

	state.SetField ( -1, "EVENT_FINALIZE", ( u32 )EVENT_FINALIZE );
	state.SetField ( -1, "EVENT_PAUSE", ( u32 )EVENT_PAUSE );
	state.SetField ( -1, "EVENT_RESUME", ( u32 )EVENT_RESUME );

	state.SetField ( -1, "SIM_LOOP_FORCE_STEP", ( u32 )SIM_LOOP_FORCE_STEP );
	state.SetField ( -1, "SIM_LOOP_ALLOW_BOOST", ( u32 )SIM_LOOP_ALLOW_BOOST );
	state.SetField ( -1, "SIM_LOOP_ALLOW_SPIN", ( u32 )SIM_LOOP_ALLOW_SPIN );
	state.SetField ( -1, "SIM_LOOP_NO_DEFICIT", ( u32 )SIM_LOOP_NO_DEFICIT );
	state.SetField ( -1, "SIM_LOOP_NO_SURPLUS", ( u32 )SIM_LOOP_NO_SURPLUS );
	state.SetField ( -1, "SIM_LOOP_RESET_CLOCK", ( u32 )SIM_LOOP_RESET_CLOCK );
	state.SetField ( -1, "SIM_LOOP_ALLOW_SOAK", ( u32 )SIM_LOOP_ALLOW_SOAK );

	state.SetField ( -1, "LOOP_FLAGS_DEFAULT", ( u32 )LOOP_FLAGS_DEFAULT );
	state.SetField ( -1, "LOOP_FLAGS_FIXED", ( u32 )LOOP_FLAGS_FIXED );
	state.SetField ( -1, "LOOP_FLAGS_MULTISTEP", ( u32 )LOOP_FLAGS_MULTISTEP );
	state.SetField ( -1, "LOOP_FLAGS_SOAK", ( u32 )LOOP_FLAGS_SOAK );

	state.SetField ( -1, "DEFAULT_STEPS_PER_SECOND", ( u32 )DEFAULT_STEPS_PER_SECOND );
	state.SetField ( -1, "DEFAULT_BOOST_THRESHOLD", ( u32 )DEFAULT_BOOST_THRESHOLD );
	state.SetField ( -1, "DEFAULT_LONG_DELAY_THRESHOLD", ( u32 )DEFAULT_LONG_DELAY_THRESHOLD );
	state.SetField ( -1, "DEFAULT_CPU_BUDGET", ( u32 )DEFAULT_CPU_BUDGET );
	state.SetField ( -1, "DEFAULT_STEP_MULTIPLIER", ( u32 )DEFAULT_STEP_MULTIPLIER );

	luaL_Reg regTable [] = {
		{ "clearLoopFlags",				_clearLoopFlags },
		{ "crash",						_crash },
		{ "enterFullscreenMode",		_enterFullscreenMode },
		{ "exitFullscreenMode",			_exitFullscreenMode },
		{ "forceGarbageCollection",		_forceGarbageCollection },
		{ "framesToTime",				_framesToTime },
		{ "getDeviceTime",				_getDeviceTime },
		{ "getElapsedFrames",			_getElapsedFrames },
		{ "getElapsedTime",				_getElapsedTime },
		{ "getHistogram",				_getHistogram },
		{ "getLoopFlags",				_getLoopFlags },
		{ "getLuaObjectCount",			_getLuaObjectCount },
		{ "getMemoryUsage",				_getMemoryUsage },
		{ "getPerformance",				_getPerformance },
		{ "getStep",					_getStep },
		{ "getTaskSubscriber",			_getTaskSubscriber },
		{ "openWindow",					_openWindow },
		{ "pauseTimer",					_pauseTimer },
		{ "reportHistogram",			_reportHistogram },
		{ "reportLeaks",				_reportLeaks },
		{ "setBoostThreshold",			_setBoostThreshold },
		{ "setCpuBudget",				_setCpuBudget},
		{ "setHistogramEnabled",		_setHistogramEnabled },
		{ "setLeakTrackingEnabled",		_setLeakTrackingEnabled },
		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAISim > },
		{ "setLongDelayThreshold",		_setLongDelayThreshold },
		{ "setLoopFlags",				_setLoopFlags },
		{ "setLuaAllocLogEnabled",		_setLuaAllocLogEnabled },
		{ "setStep",					_setStep },
		{ "setStepMultiplier",			_setStepMultiplier },
		{ "setTimerError",				_setTimerError },
		{ "setTraceback",				_setTraceback },
		{ "timeToFrames",				_timeToFrames },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAISim::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAISim::ResumeMOAI() {

	if ( this->mLoopState == PAUSED ) {
		this->SendResumeEvent();
		this->mLoopState = START;
	}
}

//----------------------------------------------------------------//
void MOAISim::SendFinalizeEvent () {

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListener ( EVENT_FINALIZE, state )) {
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAISim::SendPauseEvent () {

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListener ( EVENT_PAUSE, state )) {
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAISim::SendResumeEvent () {

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	if ( this->PushListener ( EVENT_RESUME, state )) {
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAISim::SetStep ( double step ) {

	if ( this->mStep != step ) {

		this->mStep = step;
		AKUSetSimStepFunc setSimStep = AKUGetFunc_SetSimStep ();
		if ( setSimStep ) {
			setSimStep ( step );
		}
	}
}

//----------------------------------------------------------------//
double MOAISim::StepSim ( double step, u32 multiplier ) {

	double time = USDeviceTime::GetTimeInSeconds ();

	for ( u32 s = 0; s < multiplier; ++s ) {
			
		MOAIInputMgr::Get ().Update ();
		MOAIActionMgr::Get ().Update (( float )step );		
		MOAINodeMgr::Get ().Update ();
		this->mSimTime += step;
	}

	return USDeviceTime::GetTimeInSeconds () - time;
}

//----------------------------------------------------------------//
void MOAISim::Update () {

	double interval = this->MeasureFrameRate ();

	#if USE_CURL
		MOAIUrlMgrCurl::Get ().Process ();
	#endif
	
	#if MOAI_OS_NACL
		MOAIUrlMgrNaCl::Get ().Process ();
	#endif
	
	this->mTaskSubscriber->Publish ();
	
	// try to account for timer error
	if ( this->mTimerError != 0.0 ) {
		
		double steps = interval / this->mStep;
		double integer = floor ( steps );
		double decimal = steps - integer;
		
		if ( decimal <= this->mTimerError ) {
			interval = this->mStep * integer;
		}
		else if ( decimal >= ( 1.0 - this->mTimerError )) {
			interval = this->mStep * ( integer + 1.0 );
		}
	}
	
	// actual device time elapsed since starting or restarting the sim
	this->mRealTime += interval;
	
	// bail if we're paused
	if ( this->mLoopState == PAUSED ) {
		return;
	}
	
	// the reset clock flag warps the sim time ahead to match real time just once, then autoclears
	// this means there will be no time deficit or attempted catch-up
	// if spinning is not allowed, also clear prevent any time deficit
	if ( this->mLoopFlags & SIM_LOOP_RESET_CLOCK ) {
	
		this->mLoopState = START;
		this->mLoopFlags &= ~SIM_LOOP_RESET_CLOCK;
	}
	
	// 'budget' will be used to measure the actual CPU time each sim step takes to proces
	// initialize budget to limit time spent updating when the sim has fallen behind realtime
	// this prevents a scenario where the sim falls behind but loops forever when attempting to catch up due to
	// the update itself taking too long and increading the gap between real and target time
	double budget = this->mStep * this->mCpuBudget;

	// reset sim time on start
	if ( this->mLoopState == START ) {
		
		this->mRealTime = this->mSimTime;
		this->mLoopState = RUNNING;
		
		// perform an empty step to initialize the sim
		// subtract the elapsed CPU time from the budget
		budget -= this->StepSim ( 0.0, 1 );
	}

	// 'gap' is the time left to make up between sim time and real time
	// i.e. the time deficit
	double gap = this->mRealTime - this->mSimTime;

	// long delay lets us ignore gaps bigger than a certain threshold
	if (( this->mLoopFlags & SIM_LOOP_LONG_DELAY ) && ( gap > ( this->mStep * this->mLongDelayThreshold ))) {
		budget -= this->StepSim ( this->mStep, 1 );
		gap = 0.0f;
		this->mRealTime = this->mSimTime;
	}

	// boost mode allows the sim to perform a large, variable-sized step to
	// make up the entire time deficit - but only if the sim has fallen behind
	// by a certain threshold (given in number of frames)
	// we only boost if we've fallen behind the number of steps given by boost threshold
	if (( this->mLoopFlags & SIM_LOOP_ALLOW_BOOST ) && ( gap > ( this->mStep * this->mBoostThreshold ))) {
		budget -= this->StepSim ( gap, 1 );
		gap = 0.0f;
	}
	else {
	
		// we didn't boost, so process steps normally...
	
		// perform a single step only if the time deficit is greater than step time
		// in other words, at least one interval of step time has elapsed in real time
		// so we need to catch up
		if (( this->mLoopFlags & SIM_LOOP_FORCE_STEP ) || (( this->mStep <= gap ) && ( budget > 0.0 ))) {
			budget -= this->StepSim ( this->mStep, this->mStepMultiplier );
			gap -= this->mStep * ( double )this->mStepMultiplier;
		}
		
		// spin mode allows us to attempt to close the time deficit by using our
		// budget to run additional sim steps
		// of course, if the sim takes an excessively long time to process
		// we may never catch up...
		if ( this->mLoopFlags & SIM_LOOP_ALLOW_SPIN ) {
			while (( this->mStep <= gap ) && ( budget > 0.0 )) {
				budget -= this->StepSim ( this->mStep, this->mStepMultiplier );
				gap -= this->mStep * ( double )this->mStepMultiplier;
				
			}
		}

		// Will use up the remaining 'frame' budget, e.g if step size 1 / 30, it will
		// spin/sleep until this time has passed inside this update
		if ( this->mLoopFlags & SIM_LOOP_ALLOW_SOAK ) {
			
			double startTime = USDeviceTime::GetTimeInSeconds ();
			double remainingTime = budget - ( this->mStep * ( DEFAULT_CPU_BUDGET - 1 ) );
			
			// using 2ms buffer zone for sleeps
			while ( ( remainingTime - ( USDeviceTime::GetTimeInSeconds() - startTime ) > 0.002 )) {

				#ifndef MOAI_OS_WINDOWS
					usleep ( 1000 );
				#else
					// WARNING: sleep on windows is not quite as precise
					Sleep ( 1 );
				#endif
			}
		}
	}

	// if real time is more than a step ahead of sim time (for whatever reason), wait up
	if (( this->mLoopFlags & SIM_LOOP_NO_DEFICIT ) && (( this->mRealTime - this->mSimTime ) >= this->mStep )) {
		this->mRealTime = this->mSimTime;
	}

	// if real time is behind sim time (for whatever reason), catch up
	if (( this->mLoopFlags & SIM_LOOP_NO_SURPLUS ) && ( this->mRealTime < this->mSimTime )) {
		this->mRealTime = this->mSimTime;
	}
}
