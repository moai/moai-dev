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
int MOAITrace::_start ( lua_State* L ) {

	MOAITrace::Get ().Start ( L );

//	// Read the message off of the top of the stack
//	json_t* message = MOAITrace::ConvertStackIndexToJSON ( L, lua_gettop ( L ));
//
//	// Send the message back to the IDE.
//	json_t* msg = json_object ();
//	json_object_set_new ( msg, "ID", json_string ( "message" ));
//	json_object_set_new ( msg, "Value", message );
//	char* data = json_dumps ( msg, 0 );
//	MOAITrace::SendData ( std::string ( data ));
//	free ( data );

	// Done!
	return 0;
}

//================================================================//
// MOAITrace
//================================================================//

//----------------------------------------------------------------//
void MOAITrace::Callback ( lua_State* L, lua_Debug *ar ) {

	double currentTime = ZLDeviceTime::GetTimeInSeconds ();
	this->mElapsedRunTime += currentTime - this->mLastRunAt;

	this->OnHook ( L, ar );
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
	mEnginePaused ( false ),
	mEnableTrace ( false ),
	mElapsedRunTime ( 0 ),
	mLastRunAt ( 0 ),
	mCurrentThread ( 0 ),
	mCurrentThreadTrace ( 0 ) {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAITrace::~MOAITrace () {
}

//----------------------------------------------------------------//
void MOAITrace::OnHook ( lua_State* L, lua_Debug* ar ) {
	UNUSED ( L );
	UNUSED ( ar );
}

//----------------------------------------------------------------//
void MOAITrace::OnStart () {
}

//----------------------------------------------------------------//
void MOAITrace::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "reportTrace",		_reportTrace },
		{ "run",				_run },
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
void MOAITrace::Start ( lua_State* L ) {

	MOAITrace::mEnginePaused = false;
	lua_sethook ( L, MOAITrace::_callback, LUA_MASKCALL | LUA_MASKRET | LUA_MASKLINE, 0 );
	
	this->mEnableTrace = true;
	
	this->OnStart ();
	
	this->mLastRunAt = ZLDeviceTime::GetTimeInSeconds ();
}
