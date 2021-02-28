// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAIActionMgr.h>
#include <moai-core/MOAIActionTree.h>
#include <moai-core/MOAINodeMgr.h>
#include <moai-core/MOAITestMgr.h>
#include <moai-core/MOAIUpdateMgr.h>

#if defined(_WIN32)
	#include <windows.h>
	#include <Psapi.h>
#elif defined(__APPLE__) //&& defined(TARGET_IPHONE_SIMULATOR)
	// Not sure if using mach API is disallowed in the app store. :/
	#include <mach/mach.h>
	#include <unistd.h>
#elif defined (__QNX__)
	#include <unistd.h>
#elif defined (__EMSCRIPTEN__)
  #include <unistd.h>
#elif defined (ANDROID)
  #include <unistd.h>
#endif

#define LUA_GC_FUNC_NAME "collectgarbage"

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	clearLoopFlags
	@text	Uses the mask provided to clear the loop flags.

	@opt	number mask		Default value is 0xffffffff.
	@out	nil
*/
int MOAIUpdateMgr::_clearLoopFlags ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIUpdateMgr, "" )
	self->mLoopFlags &= ~state.GetValue < u32 >( 1, 0xffffffff );
	return 0;
}

//----------------------------------------------------------------//
int MOAIUpdateMgr::_collectgarbage ( lua_State* L ) {
	UNUSED ( L );
	printf ( "WARNING: 'collectgarbage' replaced by MOAIUpdateMgr. Use MOAIUpdateMgr's 'forceGC', 'setGCStep' and 'setGCActive' instead.\n" );
	return 0;
}

//----------------------------------------------------------------//
// TODO: deprecate
int MOAIUpdateMgr::_forceGC ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIUpdateMgr, "" )
	self->Get < MOAILuaRuntime >().ForceGarbageCollection ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	framesToTime
	@text	Converts the number of frames to time passed in seconds.

	@in		number frames		The number of frames.
	@out	number time			The equivalent number of seconds for the specified number of frames.
*/
int MOAIUpdateMgr::_framesToTime ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIUpdateMgr, "N" )

	float frames = state.GetValue < float >( 1, 0.0f );
	lua_pushnumber ( state, frames * self->mStep );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getDeviceTime
	@text	Gets the raw device clock. This is a replacement for Lua's os.time ().

	@out	number time			The device clock time in seconds.
*/
int MOAIUpdateMgr::_getDeviceTime ( lua_State* L ) {
	
	lua_pushnumber ( L, ZLDeviceTime::GetTimeInSeconds ());
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getElapsedTime
	@text	Gets the number of seconds elapsed since the application was started.

	@out	number time			The number of elapsed seconds.
*/
int MOAIUpdateMgr::_getElapsedTime ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIUpdateMgr, "" )
	lua_pushnumber ( L, self->mSimTime );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getLoopFlags
	@text	Returns the current loop flags.

	@out	number mask
*/
int MOAIUpdateMgr::_getLoopFlags ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIUpdateMgr, "" )
	lua_pushnumber ( L, self->mLoopFlags );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getStep
	@text	Gets the amount of time (in seconds) that it takes for one frame to pass.

	@out	number size			The size of the frame; the time it takes for one frame to pass.
*/
int MOAIUpdateMgr::_getStep ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIUpdateMgr, "" )
	lua_pushnumber ( L, self->GetStep ());
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getStepCount
	@text	Gets the number of times the sim was stepped since the application was started.

	@out	number steps		The number of times the sim was stepped.
*/
int MOAIUpdateMgr::_getStepCount ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIUpdateMgr, "" )
	lua_pushnumber ( L, self->mStepCount );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	pauseTimer
	@text	Pauses or unpauses the device timer, preventing any visual updates (rendering) while paused.

	@in		boolean pause		Whether the device timer should be paused.
	@out	nil
*/
int MOAIUpdateMgr::_pauseTimer ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIUpdateMgr, "" )
	
	bool pause = state.GetValue < bool >( 1, true );
	
	if ( pause ) {
		self->Pause ();
	}
	else {
		self->Resume ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setBoostThreshold
	@text	Sets the boost threshold, a scalar applied to step. If the gap
			between simulation time and device time is greater than the step
			size multiplied by the boost threshold and MOAIUpdateMgr.SIM_LOOP_ALLOW_BOOST
			is set in the loop flags, then the simulation is updated once with a
			large, variable step to make up the entire gap.

	@opt	number boostThreshold		Default value is DEFAULT_BOOST_THRESHOLD.
	@out	nil
*/
int MOAIUpdateMgr::_setBoostThreshold ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIUpdateMgr, "" )
	self->mBoostThreshold = state.GetValue < double >( 1, DEFAULT_BOOST_THRESHOLD );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setCpuBudget
	@text	Sets the amount of time (given in simulation steps) to allow
			for updating the simulation.
	
	@in		number budget	Default value is DEFAULT_CPU_BUDGET.
	@out	nil
*/
int MOAIUpdateMgr::_setCpuBudget ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIUpdateMgr, "" )
	self->mCpuBudget = state.GetValue < u32 >( 1, DEFAULT_CPU_BUDGET );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setGCActive
	@text	Enable incremental garbage collection.
	
	@in		boolean active			Default value is false.
	@out	nil
*/
int MOAIUpdateMgr::_setGCActive ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIUpdateMgr, "" )
	self->mGCActive = state.GetValue < bool >( 1, false );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setGCStep
	@text	Sets a step to use when running the incremental gc each frame.
	
	@in		number step
	@out	nil
*/
int MOAIUpdateMgr::_setGCStep ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIUpdateMgr, "" )
	self->mGCStep = state.GetValue < u32 >( 1, 0 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setLongDelayThreshold
	@text	Sets the long delay threshold. If the simulation step falls behind
			the given threshold, the deficit will be dropped: the simulation will
			neither spin nor boost to catch up.

	@opt	number longDelayThreshold		Default value is DEFAULT_LONG_DELAY_THRESHOLD.
	@out	nil
*/
int MOAIUpdateMgr::_setLongDelayThreshold ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIUpdateMgr, "" )
	self->mLongDelayThreshold = state.GetValue < double >( 1, DEFAULT_LONG_DELAY_THRESHOLD );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setLoopFlags
	@text	Fine tune behavior of the simulation loop. MOAIUpdateMgr.SIM_LOOP_ALLOW_SPIN
			will allow the simulation step to run multiple times per update to try
			and catch up with device time, but will abort if processing the simulation
			exceeds the configured step time. MOAIUpdateMgr.SIM_LOOP_ALLOW_BOOST will permit
			a *variable* update step if simulation time falls too far behind
			device time (based on the boost threshold). Be warned: this can wreak
			havoc with physics and stepwise animation or game AI.
			
			Three presets are provided: MOAIUpdateMgr.LOOP_FLAGS_DEFAULT, MOAIUpdateMgr.LOOP_FLAGS_FIXED,
			and MOAIUpdateMgr.LOOP_FLAGS_MULTISTEP.

	@opt	number flags		Mask or a combination of MOAIUpdateMgr.SIM_LOOP_FORCE_STEP, MOAIUpdateMgr.SIM_LOOP_ALLOW_BOOST,
								MOAIUpdateMgr.SIM_LOOP_ALLOW_SPIN, MOAIUpdateMgr.SIM_LOOP_NO_DEFICIT, MOAIUpdateMgr.SIM_LOOP_NO_SURPLUS,
								MOAIUpdateMgr.SIM_LOOP_RESET_CLOCK. Default value is 0.
	@out	nil	
*/
int MOAIUpdateMgr::_setLoopFlags ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIUpdateMgr, "" )
	self->mLoopFlags |= state.GetValue < u32 >( 1, 0 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setStepMultiplier
	@text	Runs the simulation multiple times per step (but with a fixed
			step size). This is used to speed up the simulation without
			providing a larger step size (which could destabilize physics
			simulation).
	
	@in		number count		Default value is DEFAULT_STEP_MULTIPLIER.
	@out	nil
*/
int MOAIUpdateMgr::_setStepMultiplier ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIUpdateMgr, "" )
	self->mStepMultiplier = state.GetValue < u32 >( 1, DEFAULT_STEP_MULTIPLIER );
	return 0;
}

//----------------------------------------------------------------//
/**
	@lua	setStepSmoothing
	@text	Average delta time over N last frames. This is useful to
			filter out momentary single-frame spikes. 
			Can make difference even in fixed step setup (helps to avoids double steps).

	@in		number count		Number of frames. Default is 0 (no smoothing).
	@out	nil
*/
int MOAIUpdateMgr::_setStepSmoothing ( lua_State *L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIUpdateMgr, "" )

	u32 size = state.GetValue < u32 >( 1, 0 );
	
	self->mSmoothBuffer.Init ( size );
	self->mSmoothBuffer.Fill ( self->mStep );
	self->mSmoothIdx = 0;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setTimerError
	@text	Sets the tolerance for timer error. This is a multiplier of step.
			Timer error tolerance is step * timerError.
	
	@in		number timerError		Default value is 0.0.
	@out	nil
*/
int MOAIUpdateMgr::_setTimerError ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIUpdateMgr, "" )
	self->mTimerError = state.GetValue < double >( 1, 0.0 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	timeToFrames
	@text	Converts the number of time passed in seconds to frames.

	@in		number time			The number of seconds.
	@out	number frames		The equivalent number of frames for the specified number of seconds.
*/
int MOAIUpdateMgr::_timeToFrames ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIUpdateMgr, "N" )
	
	float time = state.GetValue < float >( 1, 0.0f );
	lua_pushnumber ( state, time / self->mStep );
	
	return 1;
}

//================================================================//
// MOAIUpdateMgr
//================================================================//

//----------------------------------------------------------------//
MOAIUpdateMgr::MOAIUpdateMgr ( ZLContext& context ) :
	ZLHasContext ( context ),
	ZLContextClass ( context ),
	MOAILuaObject ( context ),
	MOAIEventSource ( context ),
	MOAIGlobalEventSource ( context ),
	mLoopState ( START ),
	mStep ( 1.0 / ( double )DEFAULT_STEPS_PER_SECOND ),
	mSimTime ( 0.0 ),
	mRealTime ( 0.0 ),
	mFrameTime ( 0.0 ),
	mPauseTime ( 0.0 ),
	mStepCount ( 0 ),
	mFrameRate ( 0.0f ),
	mFrameRateIdx ( 0 ),
	mNodeMgrTime ( 0.0 ),
	mActionTreeTime ( 0.0 ),
	mLastNodeMgrTime ( 0.0 ),
	mLastActionTreeTime ( 0.0 ),
	mLoopFlags ( LOOP_FLAGS_DEFAULT ),
	mBoostThreshold ( DEFAULT_BOOST_THRESHOLD ),
	mLongDelayThreshold ( DEFAULT_LONG_DELAY_THRESHOLD ),
	mCpuBudget ( DEFAULT_CPU_BUDGET ),
	mStepMultiplier ( DEFAULT_STEP_MULTIPLIER ),
	mTimerError ( 0.0 ),
	mSimDuration ( 1.0 / 60.0 ),
	mSmoothIdx ( 0 ),
	mGCActive ( true ),
	mGCStep ( 0 ) {
		
	RTTI_BEGIN ( MOAIUpdateMgr )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIUpdateMgr >)
		RTTI_EXTEND ( MOAIGlobalEventSource )
	RTTI_END
	
	for ( u32 i = 0; i < FPS_BUFFER_SIZE; ++i ) {
		this->mFrameRateBuffer [ i ] = 0.0f;
	}
	
	this->mFrameTime = ZLDeviceTime::GetTimeInSeconds ();
}

//----------------------------------------------------------------//
MOAIUpdateMgr::~MOAIUpdateMgr () {
}

//----------------------------------------------------------------//
double MOAIUpdateMgr::MeasureFrameRate () {

	double frameTime = ZLDeviceTime::GetTimeInSeconds ();
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
void MOAIUpdateMgr::ResetPerformanceTimers () {

	this->mLastActionTreeTime = this->mActionTreeTime;
	this->mLastNodeMgrTime = this->mNodeMgrTime;
	this->mNodeMgrTime = 0.0;
	this->mActionTreeTime = 0.0;
}

//----------------------------------------------------------------//
void MOAIUpdateMgr::Pause () {

	if ( this->mLoopState != PAUSED ) {
		this->InvokeListener ( EVENT_PAUSE );
		this->mLoopState = PAUSED;
		this->mPauseTime = ZLDeviceTime::GetTimeInSeconds ();
	}
}

//----------------------------------------------------------------//
void MOAIUpdateMgr::Resume () {

	if ( this->mLoopState == PAUSED ) {
	
		double skip = ZLDeviceTime::GetTimeInSeconds () - this->mPauseTime;
		this->MOAIUpdateMgr_DidResume ( skip );
	
		this->InvokeListener ( EVENT_RESUME );
		this->mLoopState = START;
	}
}

//----------------------------------------------------------------//
double MOAIUpdateMgr::SmoothStep ( double step ) {
	
	if ( this->mSmoothBuffer.Size () == 0 ) {
		return step;
	}
	
	ZLSize size = this->mSmoothBuffer.Size ();
	
	this->mSmoothBuffer [ this->mSmoothIdx++ ] = step;
	this->mSmoothIdx =  ZLIndexOp::Wrap ( this->mSmoothIdx, size );
	
	u32 count = 0;
	double sum = 0.0;
	for ( ZLIndex i = 0; i < size; ++i ) {
		double dt = this->mSmoothBuffer [ i ];
		
		// Ignore long delay steps
		if ( !( this->mLoopFlags & SIM_LOOP_LONG_DELAY ) || ( dt < this->mStep * this->mLongDelayThreshold )) {
			count++;
			sum += dt;
		}
	}
	return ( count > 0 ) ? sum / count : step;
}

//----------------------------------------------------------------//
double MOAIUpdateMgr::StepSim ( double step, u32 multiplier ) {

	double time = ZLDeviceTime::GetTimeInSeconds ();

	MOAIScopedLuaState state = this->Get < MOAILuaRuntime >().State ();

	for ( u32 s = 0; s < multiplier; ++s ) {
		
		lua_gc ( state, LUA_GCSTOP, 0 );
		
//		MOAITestMgr::Get ().Step ();
		this->MOAIUpdateMgr_WillStep ( step );
		
		this->InvokeListener ( EVENT_STEP );
		
		double t = ZLDeviceTime::GetTimeInSeconds ();
		this->Get < MOAIActionMgr >().GetActionTree ().Update ( step );
		this->mActionTreeTime = this->mActionTreeTime + ZLDeviceTime::GetTimeInSeconds () - t;
		
		t = ZLDeviceTime::GetTimeInSeconds ();
		this->Get < MOAINodeMgr >().Update ();
		this->mNodeMgrTime = this->mNodeMgrTime + ZLDeviceTime::GetTimeInSeconds () - t;
		
		this->mSimTime += step;
		this->mStepCount++;
		
		if ( this->mGCActive ) {
			
			// empty the userdata cache
			this->Get < MOAILuaRuntime >().PurgeUserdataCache ();
		
			// crank the garbage collector
			lua_gc ( state, LUA_GCSTEP, this->mGCStep );
		}
	}
	return ZLDeviceTime::GetTimeInSeconds () - time;
}

//----------------------------------------------------------------//
void MOAIUpdateMgr::Update () {

	MOAIScopedLuaState state = this->Get < MOAILuaRuntime >().State ();

	if ( !this->mLuaGCFunc ) {
	
		lua_getglobal ( state, LUA_GC_FUNC_NAME );
		this->mLuaGCFunc.SetRef ( *this, state, -1 );
		lua_pop ( state, 1 );
		
		lua_pushcfunction ( state, _collectgarbage );
		lua_setglobal ( state, LUA_GC_FUNC_NAME );
	}

	// Measure performance
	double simStartTime = ZLDeviceTime::GetTimeInSeconds ();

	double interval = this->MeasureFrameRate ();
	this->ResetPerformanceTimers ();
	
	this->MOAIUpdateMgr_WillUpdate ();
	
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

	interval = this->SmoothStep ( interval );
	
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
	if ( !(( this->mLoopFlags & SIM_LOOP_ALLOW_BOOST ) && ( gap > ( this->mStep * this->mBoostThreshold )))) {
//		budget -= this->StepSim ( gap, 1 );
//		gap = 0.0f;
//	}
//	else {
	
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
			
			double startTime = ZLDeviceTime::GetTimeInSeconds ();
			double remainingTime = budget - ( this->mStep * ( DEFAULT_CPU_BUDGET - 1 ) );
			
			// using 2ms buffer zone for sleeps
			while ( ( remainingTime - ( ZLDeviceTime::GetTimeInSeconds() - startTime ) > 0.002 )) {

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
	
	// Measure performance
	double simEndTime = ZLDeviceTime::GetTimeInSeconds ();
	this->mSimDuration = simEndTime - simStartTime;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIUpdateMgr::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	state.SetField ( -1, "EVENT_FINALIZE",	( u32 )EVENT_FINALIZE );
	state.SetField ( -1, "EVENT_PAUSE",		( u32 )EVENT_PAUSE );
	state.SetField ( -1, "EVENT_RESUME",	( u32 )EVENT_RESUME );
	state.SetField ( -1, "EVENT_STEP",		( u32 )EVENT_STEP );

	state.SetField ( -1, "SIM_LOOP_FORCE_STEP",		( u32 )SIM_LOOP_FORCE_STEP );
	state.SetField ( -1, "SIM_LOOP_ALLOW_BOOST",	( u32 )SIM_LOOP_ALLOW_BOOST );
	state.SetField ( -1, "SIM_LOOP_ALLOW_SPIN",		( u32 )SIM_LOOP_ALLOW_SPIN );
	state.SetField ( -1, "SIM_LOOP_NO_DEFICIT",		( u32 )SIM_LOOP_NO_DEFICIT );
	state.SetField ( -1, "SIM_LOOP_NO_SURPLUS",		( u32 )SIM_LOOP_NO_SURPLUS );
	state.SetField ( -1, "SIM_LOOP_RESET_CLOCK",	( u32 )SIM_LOOP_RESET_CLOCK );
	state.SetField ( -1, "SIM_LOOP_ALLOW_SOAK",		( u32 )SIM_LOOP_ALLOW_SOAK );

	state.SetField ( -1, "LOOP_FLAGS_DEFAULT",			( u32 )LOOP_FLAGS_DEFAULT );
	state.SetField ( -1, "LOOP_FLAGS_FIXED",			( u32 )LOOP_FLAGS_FIXED );
	state.SetField ( -1, "LOOP_FLAGS_MULTISTEP",		( u32 )LOOP_FLAGS_MULTISTEP );
	state.SetField ( -1, "LOOP_FLAGS_SOAK",				( u32 )LOOP_FLAGS_SOAK );

	state.SetField ( -1, "DEFAULT_STEPS_PER_SECOND",		( u32 )DEFAULT_STEPS_PER_SECOND );
	state.SetField ( -1, "DEFAULT_BOOST_THRESHOLD",			( u32 )DEFAULT_BOOST_THRESHOLD );
	state.SetField ( -1, "DEFAULT_LONG_DELAY_THRESHOLD",	( u32 )DEFAULT_LONG_DELAY_THRESHOLD );
	state.SetField ( -1, "DEFAULT_CPU_BUDGET",				( u32 )DEFAULT_CPU_BUDGET );
	state.SetField ( -1, "DEFAULT_STEP_MULTIPLIER",			( u32 )DEFAULT_STEP_MULTIPLIER );

	luaL_Reg regTable [] = {
		{ "clearLoopFlags",				_clearLoopFlags },
		{ "forceGC",					_forceGC },
		{ "framesToTime",				_framesToTime },
		{ "getDeviceTime",				_getDeviceTime },
		{ "getElapsedTime",				_getElapsedTime },
		{ "getListener",				&MOAIGlobalEventSource::_getListener < MOAIUpdateMgr >},
		{ "getLoopFlags",				_getLoopFlags },
		{ "getStep",					_getStep },
		{ "getStepCount",				_getStepCount },
		{ "pauseTimer",					_pauseTimer },
		{ "setBoostThreshold",			_setBoostThreshold },
		{ "setCpuBudget",				_setCpuBudget},
		{ "setGCActive",				_setGCActive },
		{ "setGCStep",					_setGCStep },
		{ "setListener",				&MOAIGlobalEventSource::_setListener < MOAIUpdateMgr >},
		{ "setLongDelayThreshold",		_setLongDelayThreshold },
		{ "setLoopFlags",				_setLoopFlags },
		{ "setStepMultiplier",			_setStepMultiplier },
		{ "setStepSmoothing",			_setStepSmoothing },
		{ "setTimerError",				_setTimerError },
		{ "timeToFrames",				_timeToFrames },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIUpdateMgr::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIUpdateMgr::ZLContextClass_Finalize () {

	this->InvokeListener ( EVENT_FINALIZE );
}
