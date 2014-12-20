// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-core/MOAITrace.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
void MOAITrace::_callback ( lua_State *L, lua_Debug* ar ) {

	MOAITrace::Get ().Callback ( L, ar );
}

//----------------------------------------------------------------//
int MOAITrace::_reportTrace ( lua_State* L ) {
	UNUSED ( L );

	MOAITrace::Get ().ReportTrace ();
	return 0;
}

//----------------------------------------------------------------//
int MOAITrace::_run ( lua_State* L ) {
	UNUSED ( L );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAITrace::_setVerbose ( lua_State* L ) {
	MOAILuaState state ( L );

	MOAITrace::Get ().SetVerbose ( state.GetValue < bool >( 1, true ));
	return 0;
}

//----------------------------------------------------------------//
int MOAITrace::_start ( lua_State* L ) {
	UNUSED ( L );

	MOAITrace::Get ().Start ();
	return 0;
}

//================================================================//
// MOAITrace
//================================================================//

//----------------------------------------------------------------//
void MOAITrace::Callback ( lua_State* L, lua_Debug *ar ) {

	double currentTime = ZLDeviceTime::GetTimeInSeconds ();
	this->mElapsedRunTime += currentTime - this->mLastRunAt;

	if ( this->mVerbose ) {
		switch ( ar->event ) {
		
			case LUA_HOOKCALL:
				printf ( "CALL <%p>: %d\n", L, ar->currentline );
				break;
				
			case LUA_HOOKLINE:
				printf ( "LINE <%p>: %d\n", L, ar->currentline );
				break;
				
			case LUA_HOOKRET:
			case LUA_HOOKTAILRET:
				printf ( "RTRN <%p>: %d\n", L, ar->currentline );
				break;
		}
	}

	if ( this->mListener ) {
		this->mListener->OnHook ( L, ar );
	}
	
	this->HandleTrace ( L, ar );
	
	this->mLastRunAt = ZLDeviceTime::GetTimeInSeconds ();
}

//----------------------------------------------------------------//
void MOAITrace::HandleTrace ( lua_State* L, lua_Debug *ar ) {

	if ( !this->mEnableTrace ) return;
	
	bool didStartOrResume = this->mCurrentThread != L ;
	if ( didStartOrResume ) {
		
		// if we're changing threads, we could be *starting* a new coroutine or
		// we could be returning to a coroutine that was already running. either way,
		// we can't tell if the thread we left was the thread that triggered the change
		// as the new through could have been created and run by the engine.
	
		// to try and profile more accurately, we need to stop billing the thread we
		// are leaving for time and start or resume billing the new thread. to do this
		// we just track the amount of time the thread has been asleep. however: we deduct
		// the sleep time *only if* the event is a return (meaning the sleep was triggered
		// from a yield within the thread). otherwise, the sleep time is discarded.
	
		if ( this->mCurrentThreadTrace ) {
			this->mCurrentThreadTrace->mLastSleptAt = this->mElapsedRunTime;
			//printf ( "SLEPT %p: %f\n", this->mCurrentThreadTrace->mLastSleptAt * 1000000 );
		}
	
		//printf ( "\n" );
		//printf ( "//----------------------------------------------------------------//\n" );
	}
	
	this->mCurrentThread = L;
	MOAIThreadTrace& threadTrace = this->mThreadTrace [ this->mCurrentThread ];
	this->mCurrentThreadTrace = &threadTrace;
	
	if ( !threadTrace.mThread ) {
		threadTrace.mThread = L;
		threadTrace.mLastSleptAt = this->mElapsedRunTime;
		//printf ( "STARTED %p: %f\n", L, this->mElapsedRunTime * 1000000 );
		return;
	}
	
	double totalTimeAsleep = 0;
	if ( didStartOrResume ) {
		totalTimeAsleep = this->mElapsedRunTime - threadTrace.mLastSleptAt;
		//printf ( "RESUMED %p: %f (after %f)\n", L, this->mElapsedRunTime * 1000000, totalTimeAsleep * 1000000 );
	}
	
	switch ( ar->event ) {
	
		case LUA_HOOKCALL: {
		
			lua_getinfo ( L, "n", ar );
		
			lua_getinfo ( L, "f", ar );
			const void* func = lua_topointer ( L, -1 );
			lua_pop ( L, 1 );
			
			MOAIFuncTraceStackFrame& frame = threadTrace.mStack.Push ();
			
			frame.mFunc = func;
			frame.mLastRunAt = this->mElapsedRunTime;

			break;
		}
		
		case LUA_HOOKRET:
		case LUA_HOOKTAILRET: {
			
			lua_getinfo ( L, "n", ar );
			
			//printf ( "RETURN: %p %s\n", L, funcName );

			// there's a chance we can get a return without a call if the profiler is started in this thread
			if ( threadTrace.mStack.GetTop ()) {
			
				lua_getinfo ( L, "f", ar );
				const void* func = lua_topointer ( L, -1 );
				lua_pop ( L, 1 );
				
				MOAIFuncTraceStackFrame& frame = threadTrace.mStack.Pop ();
				assert ( frame.mFunc == func );
				
				MOAIFuncTrace& record = this->mFuncTrace [ func ];
				
				if ( record.mCount == 0 ) {
					lua_getinfo ( L, "nlS", ar );
					
					record.mName			= ar->name;
					record.mNameWhat		= ar->namewhat;
					record.mWhat			= ar->what;
					
					record.mSource			= ar->source;
					record.mShortSource		= ar->short_src;

					record.mFirstLine		= ar->linedefined;
					record.mLastLine		= ar->lastlinedefined;
				}
				
				assert ( frame.mFunc == func );
				
				double elapsed = this->mElapsedRunTime - frame.mLastRunAt;
				assert ( totalTimeAsleep < elapsed );

				record.mElapsedRunTime += elapsed - totalTimeAsleep;
				
				//printf ( "elapsed: %f asleep: %f\n", elapsed * 1000000, totalTimeAsleep * 1000000 );
				record.mCount++;
			}
			break;
		}
	}
}

//----------------------------------------------------------------//
MOAITrace::MOAITrace () :
	mIsActive ( false ),
	mEnableTrace ( false ),
	mElapsedRunTime ( 0 ),
	mLastRunAt ( 0 ),
	mCurrentThread ( 0 ),
	mCurrentThreadTrace ( 0 ),
	mVerbose ( false ) {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAITrace::~MOAITrace () {
}

//----------------------------------------------------------------//
void MOAITrace::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "reportTrace",		_reportTrace },
		{ "run",				_run },
		{ "setVerbose",			_setVerbose },
		{ "start",				_start },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITrace::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAITrace::ReportTrace () {

	FILE* file	= MOAILogMgr::Get ().GetFile ();

	typedef STLSortPair < double, MOAIFuncTrace* > SortPair;
	STLList < SortPair > sortList;

	double traceTime = 0;

	FuncTraceIt funcTraceIt = this->mFuncTrace.begin ();
	for ( ; funcTraceIt != this->mFuncTrace.end (); ++funcTraceIt ) {
		MOAIFuncTrace& funcTrace = funcTraceIt->second;
		
		if ( funcTrace.mCount > 0 ) {

			traceTime += funcTrace.mElapsedRunTime;

			SortPair sortPair;
			sortPair.mKey = funcTrace.mElapsedRunTime;
			sortPair.mValue = &funcTrace;
			
			sortList.push_back ( sortPair );
		}
	}

	sortList.sort ( SortPair::greater );

	ZLLog::LogF ( file, "trace time: %fms\n", traceTime * 1000000 );

	STLList < SortPair >::iterator sortListIt = sortList.begin ();
	for ( ; sortListIt != sortList.end (); ++sortListIt ) {
		MOAIFuncTrace& funcTrace = *( sortListIt->mValue );
		ZLLog::LogF (
			file,
			"what:%s name:%s (%s) count:%d time:%3.2f%%\n",
			( cc8* )funcTrace.mWhat,
			( cc8* )funcTrace.mName,
			( cc8* )funcTrace.mNameWhat,
			funcTrace.mCount,
			( funcTrace.mElapsedRunTime / traceTime ) * 100
		);
	}
}

//----------------------------------------------------------------//
void MOAITrace::Start () {

	if ( this->mIsActive ) return;
	this->mIsActive = true;

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	lua_sethook ( state, MOAITrace::_callback, LUA_MASKCALL | LUA_MASKRET | LUA_MASKLINE, 0 );
	
	this->mLastRunAt = ZLDeviceTime::GetTimeInSeconds ();
}

//----------------------------------------------------------------//
void MOAITrace::Start ( MOAILuaHookListener* listener ) {

	this->Start ();
	this->mListener = listener;
}
