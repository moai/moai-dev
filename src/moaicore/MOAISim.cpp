// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
//#include <moaicore/MOAICollisionMgr.h>
#include <moaicore/MOAIDebugLines.h>
#include <moaicore/MOAIInputMgr.h>
#include <moaicore/MOAINodeMgr.h>
#include <moaicore/MOAIPrim.h>
#include <moaicore/MOAISim.h>
#include <moaicore/MOAIFmod.h>
#include <aku/AKU.h>

#define USERPREFS_FILENAME "userprefs.lua"

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>clearRenderStack ()</tt>\n
\n
	Clears the render stack.
*/
int MOAISim::_clearRenderStack ( lua_State* L ) {

	USLuaState state ( L );
	
	MOAISim& device = MOAISim::Get ();
	device.Clear ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
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
/**	@brief <tt>( returns ) func ( self )</tt>\n
\n
	Description of method Coming Soon(tm).
	@param self (in)
	@param y (out)
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
/**	@brief <tt>framesToTime ( frames )</tt>\n
\n
	Converts frame counts to time.
	@param frames Count of frames to be converted.
	@return Time in ?? (seconds?)
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
/**	@brief <tt>getDeviceIDString ( )</tt>\n
\n
	Gets a unique string that identifies the device.
	@return The identifier string.
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
/**	@brief <tt>getElapsedFrames ( )</tt>\n
\n
	Returns the number of frames elapsed since...?
	@return Frames elapsed.
*/
int MOAISim::_getElapsedFrames ( lua_State* L ) {
	
	MOAISim& device = MOAISim::Get ();
	lua_pushnumber ( L, device.mTime / device.mStep );
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>getElapsedTime ( )</tt>\n
\n
	Returns the amount of time elapsed since...?
	@return Time elapsed.
*/
int MOAISim::_getElapsedTime ( lua_State* L ) {
	
	lua_pushnumber ( L, MOAISim::Get ().mTime );
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>getFrameSize ( )</tt>\n
\n
	Returns the size of a frame?
	@return Frame size.
*/
int MOAISim::_getFrameSize ( lua_State* L ) {
	
	MOAISim& device = MOAISim::Get ();
	lua_pushnumber ( L, device.mStep );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>_getNetworkStatus ( )</tt>\n
\n
	Returns the status of the current ability to reach the web.
	@return Boolean representing current reachability status.
*/
int	MOAISim::_getNetworkStatus ( lua_State* L ) {

	MOAISim& device = MOAISim::Get ();
	lua_pushboolean( L, device.mHasNetwork );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>getUserInput ( keyboardType )</tt>\n
\n
	Enables the user to input text. Resulting text will be sent via
	callback set with _setTextInputCallback.
	@param keyboardType The keyboard type to use for text input.
*/
int MOAISim::_getUserInput ( lua_State* L ) {

	USLuaState state ( L );
	
	int keyboardType = state.GetValue < int >( 1, MOAISim::KEYBOARD_TYPE_DEFAULT );
	
	bool result = MOAISim::Get ().ShowTextPrompt ( keyboardType );
	
	state.Push ( result );
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>getUserPrefs ( )</tt>\n
\n
	Returns a table of variables that was saved within the project at runtime, presumably to change user settings.
	@return A lua table of previously-set variables.
*/
int MOAISim::_getUserPrefs ( lua_State* L ) {
	
	USLuaState state ( L );
	
	cc8* prefPath = MOAISim::Get().mPrefsPath;
	
	if ( !USFileSys::CheckFileExists ( prefPath )) return 0;
	
	int status;
	
	status = luaL_loadfile ( state, prefPath );
	if ( state.PrintErrors ( status )) return 0;
	
	state.DebugCall ( 0, 1 );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@brief <tt>openWindow ( title, width, height )</tt>\n
\n
	Opens a window in the device to render to.
	@param title Title to be displayed at the top of the window.
	@param width Width of the window.
	@param height Height of the window.
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
/**	@brief <tt>pauseTimer ( pause )</tt>\n
\n
	Pauses the device timer, not displaying any visual updates made while paused.
	@param pause A boolean value.  True will pause the timer, false will unpause it.
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
/**	@brief <tt>popRenderPass ( )</tt>\n
\n
	Pops the rendering prim off the stack?
*/
int MOAISim::_popRenderPass ( lua_State* L ) {
	UNUSED ( L );
	
	MOAISim& device = MOAISim::Get ();
	device.PopRenderPass ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>pushRenderPass ( prim )</tt>\n
\n
	Pushes the specified prim onto the render stack?
	@param prim The viewport of the render prim?
*/
int MOAISim::_pushRenderPass ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;
	
	MOAIPrim* prim = state.GetLuaData < MOAIPrim >( 1 );
	if ( !prim ) return 0;
	
	MOAISim& device = MOAISim::Get ();
	device.PushRenderPass ( prim );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>serializeToString ( data )</tt>\n
\n
	Converts a lua data type to a string.  Useful for sending script to a server.
	@param data The data to be sent.  Can be a table.
	@return The string of converted script.
*/
int MOAISim::_serializeToString ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;
	if ( !( state.IsType ( 1, LUA_TTABLE ) || state.IsType ( 1, LUA_TUSERDATA ))) return 0;

	USLuaSerializer serializer;
	serializer.Affirm ( state, 1 );
	serializer.AddLuaReturn ( state, 1 );
	STLString result = serializer.SerializeToString ();

	lua_pushstring ( state, result );

	return 1;
}

//----------------------------------------------------------------//
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
/**	@brief <tt>setFrameSize ( size )</tt>\n
\n
	Sets the frame size(?)
	@param size The frame size to be set.
*/
int MOAISim::_setFrameSize ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "N" )) return 0;
	
	MOAISim& device = MOAISim::Get ();
	device.mStep = state.GetValue < double >( 1, device.mStep );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>_setLoading ( ref )</tt>\n
\n
	Sets whether the Loading screen should be displayed or hidden.
	@param bool Whether to show or hide the loading screen.
*/
int  MOAISim::_setLoading ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "B" )) return 0;
	
	bool loadingScreenStatus = state.GetValue < bool >( 1, false );

	bool result = MOAISim::Get ().SetLoadingScreenStatus ( loadingScreenStatus );
	
	state.Push ( result );
	return 0;

}

//----------------------------------------------------------------//
/**	@brief <tt>setTextInputCallback ( ref )</tt>\n
\n
	Sets the callback function for when user has entered text.
	@param ref Function to call.
*/
int MOAISim::_setTextInputCallback ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "F" )) return 0;
	
	MOAISim::Get ().mOnTextEntry.SetRef ( state, 1, false );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setTextInputCallback ( ref )</tt>\n
\n
	Sets the cancel callback function for when user has canceled entering text.
	@param ref Function to call.
*/
int MOAISim::_setTextInputCancelCallback ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "F" )) return 0;
	
	MOAISim::Get ().mOnCancelTextEntry.SetRef ( state, 1, false );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>setUserPrefs ( userprefs )</tt>\n
\n
	Saves a table of variables during runtime so it can be accessed later, presumably for user settings.
	@param userprefs A lua table of variables to save for later.
*/
int MOAISim::_setUserPrefs ( lua_State* L ) {
	LUA_SETUP ( MOAISim, "T" )
	
	cc8* prefPath = MOAISim::Get().mPrefsPath;
	
	USLuaSerializer serializer;
	serializer.Affirm ( state, 1 );
	serializer.AddLuaReturn ( state, 1 );
	serializer.SerializeToFile ( prefPath );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@brief <tt>timeToFrames ( time )</tt>\n
\n
	Converts time to a frame count.
	@param time Time to be converted.
	@return Frames after conversion.
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
	
	RTTI_SINGLE ( USLuaData )
	
	#ifdef _WIN32 
		this->mPrefsPath = USERPREFS_FILENAME;
	#endif

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
void MOAISim::OnTextInputCallback ( cc8* text ) {

	USLuaStateHandle state = this->GetSelf ();

	if ( this->mOnTextEntry ) {
		this->mOnTextEntry.PushRef ( state );
		lua_pushstring ( state, text );
		state.DebugCall ( 1, 0 );
	}

	return;
}

//----------------------------------------------------------------//
void MOAISim::OnTextInputCancelCallback () {

	USLuaStateHandle state = this->GetSelf ();

	if ( this->mOnCancelTextEntry ) {
		this->mOnCancelTextEntry.PushRef ( state );
		state.DebugCall ( 1, 0 );
	}

	return;
}

//----------------------------------------------------------------//
void MOAISim::PauseMOAI () {

	this->mTimerState = PAUSED;
}

//----------------------------------------------------------------//
void MOAISim::PopRenderPass () {

	if ( this->mRenderPasses.Count ()) {
		MOAIPrim* prim = this->mRenderPasses.Back ();
		this->mRenderPasses.PopBack ();
		prim->Release ();
	}
}

//----------------------------------------------------------------//
void MOAISim::PushRenderPass ( MOAIPrim* prim ) {

	if ( prim ) {
		if ( !this->mRenderPasses.Contains ( prim )) {
			prim->Retain ();
			this->mRenderPasses.PushBack ( prim );
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
		{ "getElapsedFrames",			_getElapsedFrames },
		{ "getElapsedTime",				_getElapsedTime },
		{ "getFrameSize",				_getFrameSize },
		{ "getNetworkStatus",			_getNetworkStatus },
		{ "getUserInput",				_getUserInput },
		{ "getUserPrefs",				_getUserPrefs },
		{ "openWindow",					_openWindow },
		{ "pauseTimer",					_pauseTimer },
		{ "popRenderPass",				_popRenderPass },
		{ "pushRenderPass",				_pushRenderPass },
		{ "serializeToString",			_serializeToString },
		{ "setClearColor",				_setClearColor },
		{ "setClearDepth",				_setClearDepth },
		{ "setFrameSize",				_setFrameSize },
		{ "setLoading",					_setLoading },
		{ "setTextInputCallback",		_setTextInputCallback },
		{ "setTextInputCancelCallback",	_setTextInputCancelCallback },
		{ "setUserPrefs",				_setUserPrefs },
		{ "timeToFrames",				_timeToFrames },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
	
	state.SetField ( -1, "KEYBOARD_TYPE_DEFAULT",			( u32 )KEYBOARD_TYPE_DEFAULT );
	state.SetField ( -1, "KEYBOARD_TYPE_ASCII_CAPABLE",		( u32 )KEYBOARD_TYPE_ASCII_CAPABLE );
	state.SetField ( -1, "KEYBOARD_TYPE_NUMBERS_AND_PUNC",	( u32 )KEYBOARD_TYPE_NUMBERS_AND_PUNC );
	state.SetField ( -1, "KEYBOARD_TYPE_URL",				( u32 )KEYBOARD_TYPE_URL );
	state.SetField ( -1, "KEYBOARD_TYPE_NUMBER_PAD",		( u32 )KEYBOARD_TYPE_NUMBER_PAD );
	state.SetField ( -1, "KEYBOARD_TYPE_PHONE_PAD",			( u32 )KEYBOARD_TYPE_PHONE_PAD );
	state.SetField ( -1, "KEYBOARD_TYPE_NAME_PHONE_PAD",	( u32 )KEYBOARD_TYPE_NAME_PHONE_PAD );
	state.SetField ( -1, "KEYBOARD_TYPE_EMAIL_ADDR",		( u32 )KEYBOARD_TYPE_EMAIL_ADDR );
	state.SetField ( -1, "KEYBOARD_TYPE_ALPHABET",			( u32 )KEYBOARD_TYPE_ALPHABET );
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
		MOAIPrim* renderPass = passIt->Data ();
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
void MOAISim::SetPrefsPath ( cc8* path ) {

	mPrefsPath = path;
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
bool MOAISim::SetLoadingScreenStatus ( bool showLoadingScreen ) {

	if ( this->mLoadingScreenCallback ) {
		this->mLoadingScreenCallback.Call ( showLoadingScreen );
		return true;
	}
	return false;
} 

//----------------------------------------------------------------//
bool MOAISim::ShowTextPrompt ( int keyboardType ) {

	if ( this->mTextInputCallback ) {
		this->mTextInputCallback.Call ( keyboardType );
		return true;
	}
	return false;
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
			//MOAICollisionMgr::Get ().Update ();
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
		//MOAICollisionMgr::Get ().Update ();
		
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
	PRETTY_PRINT ( repr, mPrefsPath )
	PRETTY_PRINT ( repr, mUniqueID )

	return repr;
}