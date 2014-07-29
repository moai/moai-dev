// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFMODEVENTINSTANCE_H
#define	MOAIFMODEVENTINSTANCE_H

#include <moai-sim/headers.h>
#include <moai-fmod-designer/Source/EventHandle.h>

//================================================================//
// MOAIFmodEventInstance
//================================================================//
/**	@lua	MOAIFmodEventInstance
	@text	An instance of an FMOD Event
           Not to be confused with an Event, MOAIFmodEvent.           
*/
class MOAIFmodEventInstance :
	public MOAITransform {
private:

    FMODDesigner::EventHandle mEventHandle;

	//----------------------------------------------------------------//
    static int _isValid                ( lua_State* L );
    
    static int _stop                   ( lua_State* L );
    static int	_pause          	    ( lua_State* L );
    static int	_mute          		    ( lua_State* L );    

    static int _getVolume              ( lua_State* L );
    static int _setVolume			    ( lua_State* L );
    static int _getPitch               ( lua_State* L );
    static int _setPitch               ( lua_State* L );    
    
    static int	_getParameter		    ( lua_State* L );
    static int	_setParameter		    ( lua_State* L );
    static int _keyOff                 ( lua_State* L );
    
    static int	_unloadOnSilence	    ( lua_State* L );  
    
    static int	_getName        	    ( lua_State* L );
    static int	_getNumChannels         ( lua_State* L );

    static int _getTime                ( lua_State* L );
    static int _getDominantFrequency   ( lua_State* L );
    
    // music API    
    static int _getTempo               ( lua_State* L );
    static int _getBeatFraction        ( lua_State* L );
    static int _getMeasureFraction     ( lua_State* L );
    

public:

	DECL_LUA_FACTORY ( MOAIFmodEventInstance )
    DECL_ATTR_HELPER ( MOAIFmodEventInstance )

	//----------------------------------------------------------------//
				MOAIFmodEventInstance	();
	virtual		~MOAIFmodEventInstance	();
    bool		ApplyAttrOp				( u32 attrID, MOAIAttrOp& attrOp, u32 op );
    void        OnDepNodeUpdate         ();
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
    void        SetEventInstance        ( FMODDesigner::EventHandle hEventHandle ) { mEventHandle = hEventHandle; }
    void	    SetInstanceLoc		    ( ZLVec3D& vPos );
};

#endif
