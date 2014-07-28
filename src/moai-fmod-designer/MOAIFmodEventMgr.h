// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFMODEVENTMGR_H
#define	MOAIFMODEVENTMGR_H

#include <moai-sim/headers.h>

class MOAIFmodMicrophone;

namespace FMODDesigner {
    class Event;
}


//================================================================//
// MOAIFmodEx
//================================================================//
/**	@lua	MOAIFmodEventMgr
	@text	Event Manager singleton that provides an interface to all implemented FMOD Designer features. 
*/
class MOAIFmodEventMgr :
	public MOAIGlobalClass < MOAIFmodEventMgr, MOAILuaObject > {
private:
	MOAIFmodMicrophone*                         mMic;
    STLMap< STLString, FMODDesigner::Event >    mEventList;

	//----------------------------------------------------------------//
	static int	_getMemoryStats		        ( lua_State* L );
	static int	_init				        ( lua_State* L );
    static int _isEnabled                  ( lua_State* L );
    
    // project interface
    static int _loadProject                ( lua_State* L );
    static int _unloadProject              ( lua_State* L );
    static int _loadVoiceProject           ( lua_State* L );
    static int _unloadAllVoiceProjects     ( lua_State* L );

    // group interface
    static int _loadGroup                  ( lua_State* L );
    static int _unloadGroup                ( lua_State* L );
    static int _unloadPendingUnloads       ( lua_State* L );    

    // sound category interface
    static int _setSoundCategoryVolume     ( lua_State* L );
    static int _getSoundCategoryVolume     ( lua_State* L );
    static int _muteSoundCategory	        ( lua_State* L );
    static int _isSoundCategoryMuted       ( lua_State* L );
    static int _pauseSoundCategory         ( lua_State* L );
    static int _isSoundCategoryPaused      ( lua_State* L );

    // event interface
    static int _playEvent3D                ( lua_State* L );
    static int _playEvent2D                ( lua_State* L );    
    static int _playVoiceLine              ( lua_State* L );    
    static int _unloadEvent                ( lua_State* L );
    static int _stopAllEvents              ( lua_State* L );
    static int _muteAllEvents              ( lua_State* L );
    static int _getEventDuration           ( lua_State* L );

    // voice line interface
    static int _preloadVoiceLine           ( lua_State* L );

    // game mic interface
    static int _getMicrophone              ( lua_State* L );

    // reverb/global processing interface
    static int _setDefaultReverb           ( lua_State* L );
    static int _setDistantOcclusion        ( lua_State* L );
    static int _setNear2DBlend             ( lua_State* L );    

public:

	DECL_LUA_SINGLETON ( MOAIFmodEventMgr )

	//----------------------------------------------------------------//
					                MOAIFmodEventMgr	();
	virtual			                ~MOAIFmodEventMgr	();
    void			                RegisterLuaClass	( MOAILuaState& state );
    void                            RegisterLuaFuncs	( MOAILuaState& state );
    MOAIFmodMicrophone*		        AffirmMic			();    
    const FMODDesigner::Event*     GetEvent            ( STLString eventName );
    void                            AddEvent            ( STLString eventName, FMODDesigner::Event& actualEvent );

};

#endif