// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISIM_H
#define	MOAISIM_H

#include <moai-sim/MOAIInputMgr.h>
#include <moai-core/MOAITaskQueue.h>
#include <moai-core/MOAITaskSubscriber.h>

class MOAIProp;

//================================================================//
// MOAISim
//================================================================//
// TODO: doxygen
class MOAISim :
	public virtual MOAIUpdateMgr {
public:

	typedef ZLCallbackWithUserdata < void ( * )( void* userdata )>	EnterFullscreenModeFunc;
	typedef ZLCallbackWithUserdata < void ( * )( void* userdata )>	ExitFullscreenModeFunc;
	typedef ZLCallbackWithUserdata < void ( * )( void* userdata )>	ShowCursorFunc;
	typedef ZLCallbackWithUserdata < void ( * )( void* userdata )>	HideCursorFunc;
	typedef ZLCallbackWithUserdata < void ( * )( const char* title, int width, int height, void* userdata )>	OpenWindowFunc;
	typedef ZLCallbackWithUserdata < void ( * )( double step, void* userdata )>									SetSimStepFunc;
	typedef ZLCallbackWithUserdata < void ( * )( int xMin, int yMin, int xMax, int yMax, void* userdata )>		SetTextInputRectFunc;

private:
	
	EnterFullscreenModeFunc		mEnterFullscreenModeFunc;
	ExitFullscreenModeFunc		mExitFullscreenModeFunc;
	OpenWindowFunc				mOpenWindowFunc;
	SetSimStepFunc				mSetSimStepFunc;
	SetTextInputRectFunc		mSetTextInputRectFunc;
	ShowCursorFunc				mShowCursorFunc;
	HideCursorFunc				mHideCursorFunc;
	
	//----------------------------------------------------------------//
	static int			_crash						( lua_State* L );
	static int			_enterFullscreenMode		( lua_State* L );
	static int			_exitFullscreenMode			( lua_State* L );
	static int			_getMemoryUsage				( lua_State* L );
	static int			_getMemoryUsagePlain		( lua_State* L );
	static int			_getPerformance				( lua_State* L );
	static int			_hideCursor					( lua_State* L );
	static int			_openWindow					( lua_State* L );
	static int			_setStep					( lua_State* L );
	static int			_setTextInputRect			( lua_State* L );
	static int			_showCursor					( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs			( RTTIVisitorHistory& history, MOAILuaState& state );
	void				MOAIUpdateMgr_DidResume		( double skip );
	void				MOAIUpdateMgr_WillStep		( double step );
	void				MOAIUpdateMgr_WillUpdate	();

public:
	
	DECL_LUA_SINGLETON ( MOAISim )
	
	GET_SET ( EnterFullscreenModeFunc, EnterFullscreenModeFunc, mEnterFullscreenModeFunc );
	GET_SET ( ExitFullscreenModeFunc, ExitFullscreenModeFunc, mExitFullscreenModeFunc );
	GET_SET ( HideCursorFunc, HideCursorFunc, mHideCursorFunc );
	GET_SET ( OpenWindowFunc, OpenWindowFunc, mOpenWindowFunc );
	GET_SET ( SetSimStepFunc, SetSimStepFunc, mSetSimStepFunc );
	GET_SET ( ShowCursorFunc, ShowCursorFunc, mShowCursorFunc );
	GET_SET ( SetTextInputRectFunc, SetTextInputRectFunc, mSetTextInputRectFunc );
	
	//----------------------------------------------------------------//
					MOAISim						( ZLContext& context );
					~MOAISim					();
	void			SetStep						( double step );
};

#endif
