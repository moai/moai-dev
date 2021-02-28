// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAISim.h>

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
/**	@lua	crash
	@text	Crashes Moai with a null pointer dereference.
 
	@out	nil
*/
int MOAISim::_crash ( lua_State* L ) {
	UNUSED ( L );
	
	int *p = NULL;
	( *p ) = 0;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	enterFullscreenMode
	@text	Enters fullscreen mode on the device if possible.

	@out	nil
*/
int MOAISim::_enterFullscreenMode ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAISim, "" )

	EnterFullscreenModeFunc func = self->GetEnterFullscreenModeFunc ();
	if ( func ) {
		func.mFunc ( func.mUserdata );
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	exitFullscreenMode
	@text	Exits fullscreen mode on the device if possible.

	@out	nil
*/
int MOAISim::_exitFullscreenMode ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAISim, "" )

	ExitFullscreenModeFunc func = self->GetExitFullscreenModeFunc ();
	if ( func ) {
		func.mFunc ( func.mUserdata );
	}

	return 0;
}

//----------------------------------------------------------------//
/** @lua	getMemoryUsage
	@text	Get the current amount of memory used by MOAI and its subsystems. This will
			attempt to return reasonable estimates where exact values cannot be obtained.
			Some fields represent informational fields (i.e. are not double counted in the
			total, but present to assist debugging) and may be only available on certain
			platforms (e.g. Windows, etc). These fields begin with a '_' character.
 
	@out	table	usage		The breakdown of each subsystem's memory usage, in bytes. There is also a "total" field that contains the summed value.
*/
int MOAISim::_getMemoryUsage ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAISim, "" )
	
	MOAILuaRuntime& runtime = self->Get < MOAILuaRuntime >();
	MOAIGfxMgr& gfxMgr = self->Get < MOAIGfxMgr >();
	
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
	
	count = runtime.GetMemoryUsage ();
	lua_pushnumber(L, count / divisor);
	lua_setfield(L, -2, "lua");
	total += count;

	// This is informational only (i.e. don't double count with the previous field).
	// see: http://pgl.yoyo.org/luai/i/lua_gc
	int luabytes = lua_gc ( L, LUA_GCCOUNT, 0 ) * 1024 + lua_gc ( L, LUA_GCCOUNTB, 0 );
	lua_pushnumber ( L, luabytes / divisor  );
	lua_setfield ( L, -2, "_luagc_count" );
	
	count = gfxMgr.GetTextureMemoryUsage ();
	lua_pushnumber ( L, count / divisor );
	lua_setfield ( L, -2, "texture" );
	total += count;
	
	#if defined(_WIN32)
	
		PROCESS_MEMORY_COUNTERS pmc;

		// Print the process identifier.
		if ( GetProcessMemoryInfo ( GetCurrentProcess (), &pmc, sizeof ( pmc ))) {
			lua_pushnumber ( L, pmc.PagefileUsage / divisor );
			lua_setfield ( L, -2, "_sys_vs" );
			lua_pushnumber ( L, pmc.WorkingSetSize / divisor );
			lua_setfield ( L, -2, "_sys_rss" );
		}
		
	#elif defined(__APPLE__) //&& defined(TARGET_IPHONE_SIMULATOR)
	
		// Tricky undocumented mach polling of memory
		struct task_basic_info t_info;
		mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
	
		kern_return_t kr = task_info (
			mach_task_self (),
			TASK_BASIC_INFO,
			reinterpret_cast < task_info_t >( &t_info ),
			&t_info_count
		);
	
		// Most likely cause for failure: |task| is a zombie.
		if( kr == KERN_SUCCESS ) {
			lua_pushnumber ( L, t_info.virtual_size / divisor );
			lua_setfield ( L, -2, "_sys_vs" );
			lua_pushnumber ( L, t_info.resident_size / divisor );
			lua_setfield ( L, -2, "_sys_rss" );
		}
	#endif
	
	lua_pushnumber ( L, total / divisor );
	lua_setfield ( L, -2, "total" );
	
	return 1;
}

//----------------------------------------------------------------//
/** @lua	getMemoryUsagePlain
	@text	Returns lua and texture memory usage measured by MOAI subsystems.
			This function tries to avoid allocations to minimize skewing the results.
			Suitable for realtime memory monitoring.
	
	@out	number lua memory usage in bytes
	@out	number texture memory usage in bytes
*/
int MOAISim::_getMemoryUsagePlain ( lua_State *L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAISim, "" )
	
	size_t lua = self->Get < MOAILuaRuntime >().GetMemoryUsage ();
	size_t tex = self->Get < MOAIGfxMgr >().GetTextureMemoryUsage ();
	
	lua_pushnumber ( L, ( lua_Number )lua );
	lua_pushnumber ( L, ( lua_Number )tex );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	getPerformance
	@text	Returns an estimated frames per second and other performance counters 
			based on measurements taken at every render.

	@out	number fps		Estimated frames per second.
	@out	number seconds	Last ActionTree update duration
	@out	number seconds  Last NodeMgr update duration
	@out	number seconds  Last sim duration
	@out	number seconds  Last render duration
*/
int MOAISim::_getPerformance ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAISim, "" )

	MOAIGfxMgr& gfxMgr = self->Get < MOAIGfxMgr >();

	lua_pushnumber ( L, self->mFrameRate );
	lua_pushnumber ( L, self->mLastActionTreeTime );
	lua_pushnumber ( L, self->mLastNodeMgrTime );
	lua_pushnumber ( L, self->mSimDuration );
	lua_pushnumber ( L, gfxMgr.GetRenderDuration ());

	return 5;
}

//----------------------------------------------------------------//
/**	@lua	hideCursor
	@text	Hides system cursor.

	@out	nil
*/
int MOAISim::_hideCursor ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAISim, "" )

	HideCursorFunc func =self->GetHideCursorFunc ();
	if ( func ) {
		func.mFunc ( func.mUserdata );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	openWindow
	@text	Opens a new window for the application to render on.  This must be called before any rendering can be done, and it must only be called once.

	@in		string title		The title of the window.
	@in		number width		The width of the window in pixels.
	@in		number height		The height of the window in pixels.
	@out	nil
*/
int MOAISim::_openWindow ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAISim, "SNN" )
	
	MOAIGfxMgr& gfxMgr = self->Get < MOAIGfxMgr >();

	cc8* title = lua_tostring ( state, 1 );
	u32 width = state.GetValue < u32 >( 2, 320 );
	u32 height = state.GetValue < u32 >( 3, 480 );

	OpenWindowFunc openWindow = self->GetOpenWindowFunc ();
	if ( openWindow ) {
		MOAIFrameBuffer* buffer = gfxMgr.GetDefaultFrameBuffer ();
		assert ( buffer );
		buffer->SetFrameSize ( width, height );
		openWindow.mFunc ( title, width, height, openWindow.mUserdata );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setStep
	@text	Sets the size of each simulation step (in seconds).
	
	@in		number step		The step size. Default value is 1 / DEFAULT_STEPS_PER_SECOND.
	@out	nil
*/
int MOAISim::_setStep ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAISim, "" )
	self->SetStep ( state.GetValue < double >( 1, 1.0 / ( double )DEFAULT_STEPS_PER_SECOND ));
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setTextInputRect
	@text	Sets text input rect.
 
	@out	nil
 */
int MOAISim::_setTextInputRect ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAISim, "" )
	
	ZLIntRect rect = state.GetRect< int >( 1 );
	rect.Bless();
	
	SetTextInputRectFunc func = self->GetSetTextInputRectFunc ();
	if ( func ) {
		func.mFunc ( rect.mXMin, rect.mYMin, rect.mXMax, rect.mYMax, func.mUserdata );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	showCursor
	@text	Shows system cursor.

	@out	nil
*/
int MOAISim::_showCursor ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAISim, "" )

	ShowCursorFunc func = self->GetShowCursorFunc ();
	if ( func ) {
		func.mFunc ( func.mUserdata );
	}
	return 0;
}

//================================================================//
// MOAISim
//================================================================//

//----------------------------------------------------------------//
MOAISim::MOAISim ( ZLContext& context ) :
	ZLHasContext ( context ),
	ZLContextClass ( context ),
	MOAILuaObject ( context ),
	MOAIEventSource ( context ),
	MOAIGlobalEventSource ( context ),
	MOAIUpdateMgr ( context ),
	mEnterFullscreenModeFunc ( NULL, NULL ),
	mExitFullscreenModeFunc ( NULL, NULL ),
	mOpenWindowFunc ( NULL, NULL ),
	mSetSimStepFunc ( NULL, NULL ),
	mShowCursorFunc ( NULL, NULL ),
	mHideCursorFunc ( NULL, NULL ) {
		
	RTTI_BEGIN ( MOAISim )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAISim >)
		RTTI_EXTEND ( MOAIUpdateMgr )
	RTTI_END
}

//----------------------------------------------------------------//
MOAISim::~MOAISim () {
}

//----------------------------------------------------------------//
void MOAISim::SetStep ( double step ) {

	if ( this->mStep != step ) {
		this->mStep = step;
		if ( this->mSetSimStepFunc ) {
			this->mSetSimStepFunc.mFunc ( step, this->mSetSimStepFunc.mUserdata );
		}
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAISim::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "crash",						_crash },
		{ "enterFullscreenMode",		_enterFullscreenMode },
		{ "exitFullscreenMode",			_exitFullscreenMode },
		{ "getMemoryUsage",				_getMemoryUsage },
		{ "getMemoryUsagePlain",		_getMemoryUsagePlain },
		{ "getPerformance",				_getPerformance },
		{ "hideCursor",					_hideCursor },
		{ "openWindow",					_openWindow },
		{ "setStep",					_setStep },
		{ "setTextInputRect",			_setTextInputRect },
		{ "showCursor",					_showCursor },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAISim::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAISim::MOAIUpdateMgr_DidResume ( double skip ) {

	this->Get < MOAIInputMgr >().FlushEvents ( skip );
}

//----------------------------------------------------------------//
void MOAISim::MOAIUpdateMgr_WillStep ( double step ) {

	this->Get < MOAIInputMgr >().Update ( step );
}

//----------------------------------------------------------------//
void MOAISim::MOAIUpdateMgr_WillUpdate () {

	this->Get < MOAIMainThreadTaskSubscriber >().Publish ();
}
