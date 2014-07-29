// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-fmod-designer/MOAIFmodEventInstance.h>
#include <fmod.hpp>
#include <moai-fmod-designer/Source/EventHandle.h>
#include <moai-fmod-designer/Source/EventInstance.h>
#include <moai-fmod-designer/Source/EventParameter.h>

#ifdef MOAI_OS_IPHONE
	#include <fmodiphone.h>
#endif

#ifdef MOAI_OS_NACL
#include <NaClFileSystem.h>
#include <moai_nacl.h>
#endif

//----------------------------------------------------------------//
/**	@lua	isValid
	@text	Checks to see if the instance is valid (i.e., currently playing)
	
	@in		MOAIFmodEventInstance self

	@out	boolean valid				True if the instance is currently playing, false otherwise
*/
int	MOAIFmodEventInstance::_isValid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodEventInstance, "U" )
	
    bool bReturnVal = false;

    FMODDesigner::EventInstance* pInstance = *( self->mEventHandle );
    if ( pInstance && pInstance->IsValid() ) {
        bReturnVal = true;
    }    
	
    lua_pushboolean( L, bReturnVal );
	
    return 1;
}

//----------------------------------------------------------------//
/**	@lua   stop
	@text   Stops the Event Instance from playing.

	@in     MOAIFmodEventInstance    self

	@out    nil
*/
int	MOAIFmodEventInstance::_stop ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodEventInstance, "U" )
	
    FMODDesigner::EventInstance* pInstance = *( self->mEventHandle );
    if ( pInstance ) {
        pInstance->Stop();
    }    
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua   pause
	@text   Pauses the Event Instance currently playing.

	@in     MOAIFmodEventInstance    self
    @opt    boolean                   setting    Whether to pause or unpause. Defaults to true (pause).

	@out    nil
*/
int	MOAIFmodEventInstance::_pause ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodEventInstance, "U" )
	
    FMODDesigner::EventInstance* pInstance = *( self->mEventHandle );
    if ( pInstance ) {
        bool bSetting = state.GetValue < bool > ( 2, true );
        pInstance->Pause ( bSetting );
    }    
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua   mute
	@text   Mutes the Event Instance currently playing.

	@in     MOAIFmodEventInstance    self
    @opt    boolean                   setting    Whether to mute or unmute. Defaults to true (mute).

	@out    nil
*/
int	MOAIFmodEventInstance::_mute ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodEventInstance, "U" )
	
    FMODDesigner::EventInstance* pInstance = *( self->mEventHandle );
    if ( pInstance ) {
        bool bSetting = state.GetValue < bool > ( 2, true );
        pInstance->Mute ( bSetting );
    }    
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setVolume
	@text	Sets the volume of the Event Instance.
	
	@in		MOAIFmodEventInstance self
	@in	    number                volume     Volume is a number between 0 and 1 (where 1 is at full volume).

    @out	nil
*/
int MOAIFmodEventInstance::_setVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodEventInstance, "UN" )
	
    FMODDesigner::EventInstance* pInstance = *( self->mEventHandle );
    if ( pInstance )
    {
        float fVolume = state.GetValue < float > ( 2, 0.0f );
        pInstance->SetVolume ( fVolume );
    } 
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getVolume
	@text	Gets the volume of the Event Instance.
	
	@in		MOAIFmodEventInstance self	
	
	@out	number volume					volume of this Event Instance
*/
int MOAIFmodEventInstance::_getVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodEventInstance, "U" )
	
    FMODDesigner::EventInstance* pInstance = *( self->mEventHandle );
    if ( pInstance )
    {        
        float fVolume = pInstance->GetVolume();
        lua_pushnumber( L, fVolume );
        
        return 1;
    } 
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setPitch
	@text	Sets the pitch of the Event Instance.
	
	@in		MOAIFmodEventInstance self
	@in	    number                pitch     A pitch level, from 0.0 to 10.0 inclusive. 0.5 = half pitch, 2.0  = double pitch. Default = 1.0

    @out	nil
*/
int MOAIFmodEventInstance::_setPitch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodEventInstance, "UN" )
	
    FMODDesigner::EventInstance* pInstance = *( self->mEventHandle );
    if ( pInstance )
    {
        float fPitch = state.GetValue < float > ( 2, 1.0f );
        pInstance->SetPitch ( fPitch );
    } 
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getPitch
	@text	Gets the pitch of the Event Instance.
	
	@in		MOAIFmodEventInstance self	

	@out	number pitch					pitch of this Event Instance
*/
int MOAIFmodEventInstance::_getPitch ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodEventInstance, "U" )
	
    FMODDesigner::EventInstance* pInstance = *( self->mEventHandle );
    if ( pInstance )
    {        
        float fPitch = pInstance->GetPitch();
        lua_pushnumber( L, fPitch );
        
        return 1;
    } 
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getParameter
	@text	Gets the value (a number) of an Event parameter
	
	@in		MOAIFmodEventInstance self
	@in	    string                parameterName     The name of the Event Parameter

    @out	number                paramValue        The value of the Event Parameter
*/
int MOAIFmodEventInstance::_getParameter ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodEventInstance, "US" )
	
    FMODDesigner::EventInstance* pInstance = *( self->mEventHandle );
    if ( pInstance ) {

        cc8* paramName = state.GetValue < cc8* > ( 2, "" );
        if ( paramName[0] != '\0' ) {
        
            const FMODDesigner::Event actualEvent = pInstance->GetSourceEvent();
            FMODDesigner::EventParameter eventParam;
            actualEvent.GetParameterIndex ( eventParam, paramName );
            if ( eventParam.IsValid() )
            {
                float fReturn = pInstance->GetParameter ( eventParam );
                lua_pushnumber ( L, fReturn );

                return 1;
            }    

        }  

    } 
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setParameter
	@text	Sets the value (a number) of an Event parameter
	
	@in		MOAIFmodEventInstance self
	@in	    string                parameterName     The name of the Event Parameter
    @in	    number                paramValue        New value of the Event Parameter

    @out   nil
*/
int MOAIFmodEventInstance::_setParameter ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodEventInstance, "USN" )
	
    FMODDesigner::EventInstance* pInstance = *( self->mEventHandle );
    if ( pInstance ) {

        cc8* paramName = state.GetValue < cc8* > ( 2, "" );
        if ( paramName[0] != '\0' ) {
        
            const FMODDesigner::Event actualEvent = pInstance->GetSourceEvent();
            FMODDesigner::EventParameter eventParam;
            actualEvent.GetParameterIndex ( eventParam, paramName );
            if ( eventParam.IsValid() )
            {
                float fValue = state.GetValue < float > ( 3, 0.f );
                pInstance->SetParameter ( eventParam, fValue );                
            }    

        }  

    } 
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	keyOff
	@text	Sets the Event Instance to key off based on the passed in Event Parameter
	
	@in		MOAIFmodEventInstance self
	@in	    string                parameterName     The name of the Event Parameter

    @out   nil
*/
int MOAIFmodEventInstance::_keyOff ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodEventInstance, "US" )
	
    FMODDesigner::EventInstance* pInstance = *( self->mEventHandle );
    if ( pInstance ) {

        cc8* paramName = state.GetValue < cc8* > ( 2, "" );
        if ( paramName[0] != '\0' ) {
        
            const FMODDesigner::Event actualEvent = pInstance->GetSourceEvent();
            FMODDesigner::EventParameter eventParam;
            actualEvent.GetParameterIndex ( eventParam, paramName );
            if ( eventParam.IsValid() )
            {
                pInstance->KeyOff ( eventParam );                
            }    

        }  

    } 
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	unloadOnSilence
	@text	For streaming sounds -- unloads the Event from memory on silence.
           (A good guideline to use is: for sounds that don't repeat often)
	
	@in		MOAIFmodEventInstance self
    @opt   boolean               setting    Whether to unload on silence or not (Default: true)

    @out   nil
*/
int MOAIFmodEventInstance::_unloadOnSilence ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodEventInstance, "U" )
	
    FMODDesigner::EventInstance* pInstance = *( self->mEventHandle );
    if ( pInstance ) {

        bool bSetting = state.GetValue < bool > ( 2, true );
        pInstance->UnloadOnSilence ( bSetting );
    } 
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getName
	@text	Get the name of the Event
	
	@in		MOAIFmodEventInstance self    

    @out   string                name       Name of the event
*/
int MOAIFmodEventInstance::_getName ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodEventInstance, "U" )
	
    FMODDesigner::EventInstance* pInstance = *( self->mEventHandle );
    if ( pInstance ) {

        const STLString sName = pInstance->GetName();
        lua_pushstring ( L, sName.c_str() );
        
        return 1;
    } 
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getNumChannels
	@text	Get the number of Channels in this Event's Channel Group
	
	@in		MOAIFmodEventInstance self    

    @out   number                channels   Number of channels in Event Instance's Channel Group
*/
int MOAIFmodEventInstance::_getNumChannels ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodEventInstance, "U" )
	
    FMODDesigner::EventInstance* pInstance = *( self->mEventHandle );
    if ( pInstance ) {

        int channels = pInstance->GetNumChannels();
        lua_pushnumber ( L, channels );

        return 1;
    } 
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getTime
    @text	Returns time within the Event, or if useSubsoundTime, will return the time
           within the *the first subsound only*
	
	@in		MOAIFmodEventInstance self    
    @opt   boolean               useSubsoundTime   If true, will return the time within the first subsound only (Default: false)

    @out   number                time              Time within the Event
*/
int MOAIFmodEventInstance::_getTime ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodEventInstance, "U" )
	
    FMODDesigner::EventInstance* pInstance = *( self->mEventHandle );
    if ( pInstance ) {

        bool bUseSubsoundTime = state.GetValue < bool > ( 2, false );
        float fTime = pInstance->GetTime ( bUseSubsoundTime );
        lua_pushnumber ( L, fTime );

        return 1;
    } 
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getDominantFrequency
    @text	Returns the fundamental frequency of this Event Instance.
	
	@in		MOAIFmodEventInstance self    

    @out   number                frequency  Dominant frequency of this Event Instance
*/
int MOAIFmodEventInstance::_getDominantFrequency ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodEventInstance, "U" )
	
    FMODDesigner::EventInstance* pInstance = *( self->mEventHandle );
    if ( pInstance ) {

        float fFrequency = pInstance->GetDominantFrequency();
        lua_pushnumber ( L, fFrequency );

        return 1;
    } 
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getTempo
    @text	Returns the tempo of this Event Instance (useful for music)
	
	@in		MOAIFmodEventInstance self    

    @out   number                tempo  Tempo of this Event Instance
*/
int MOAIFmodEventInstance::_getTempo ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodEventInstance, "U" )
	
    FMODDesigner::EventInstance* pInstance = *( self->mEventHandle );
    if ( pInstance ) {

        float fTempo = pInstance->GetTempo();
        lua_pushnumber ( L, fTempo );

        return 1;
    } 
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getBeatFraction
    @text	Returns the beat fraction of this Event Instance (useful for music)
	
	@in		MOAIFmodEventInstance self    

    @out   number                beatFraction  Beat Fraction of this Event Instance
*/
int MOAIFmodEventInstance::_getBeatFraction ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodEventInstance, "U" )
	
    FMODDesigner::EventInstance* pInstance = *( self->mEventHandle );
    if ( pInstance ) {

        float fBeatFraction = pInstance->GetBeatFraction();
        lua_pushnumber ( L, fBeatFraction );

        return 1;
    } 
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getMeasureFraction
    @text	Returns the measure fraction of this Event Instance (useful for music)
	
	@in		MOAIFmodEventInstance self    

    @out   number                measureFraction  Measure Fraction of this Event Instance
*/
int MOAIFmodEventInstance::_getMeasureFraction ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodEventInstance, "U" )
	
    FMODDesigner::EventInstance* pInstance = *( self->mEventHandle );
    if ( pInstance ) {

        float fFraction = pInstance->GetMeasureFraction();
        lua_pushnumber ( L, fFraction );

        return 1;
    } 
	
	return 0;
}

//================================================================//
// MOAIFmodEventInstance
//================================================================//

//----------------------------------------------------------------//
MOAIFmodEventInstance::MOAIFmodEventInstance () {
    RTTI_BEGIN
        RTTI_EXTEND ( MOAITransform )        
    RTTI_END
}

//----------------------------------------------------------------//
MOAIFmodEventInstance::~MOAIFmodEventInstance () {
}

//----------------------------------------------------------------//
void MOAIFmodEventInstance::RegisterLuaClass ( MOAILuaState& state ) {

    MOAITransform::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIFmodEventInstance::RegisterLuaFuncs ( MOAILuaState& state ) {

    MOAITransform::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
        { "isValid",			    _isValid },

		{ "stop",			        _stop },
		{ "pause",		            _pause },
		{ "mute",	                _mute },

		{ "setLoc",                 _setLoc },

        { "setVolume",              _setVolume },
        { "setPitch",               _setPitch },
        { "getVolume",              _getVolume },
        { "getPitch",               _getPitch },

        { "getParameter",           _getParameter },
        { "setParameter",           _setParameter },
        { "keyOff",                 _keyOff },

        { "unloadOnSilence",        _unloadOnSilence },

        { "getName",                _getName },
        { "getNumChannels",         _getNumChannels },

        { "getTime",                _getTime },
        { "getDominantFrequency",   _getDominantFrequency },

        { "getTempo",               _getTempo },
        { "getBeatFraction",        _getBeatFraction },
        { "getMeasureFraction",     _getMeasureFraction },

		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//

void MOAIFmodEventInstance::SetInstanceLoc ( ZLVec3D& vPos )
{
    FMODDesigner::EventInstance* pInstance = *mEventHandle;
    if ( pInstance )
    {
        pInstance->SetPosition ( vPos );
    }    
}

//----------------------------------------------------------------//
bool MOAIFmodEventInstance::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {
    
    bool bReturn = MOAITransform::ApplyAttrOp ( attrID, attrOp, op );    

    if ( MOAIFmodEventInstanceAttr::Check ( attrID ) )
    {
		ZLVec3D vPos = GetLocalToWorldMtx().GetTranslation();
        SetInstanceLoc ( vPos );
    }

    return bReturn;
}

//----------------------------------------------------------------//
void MOAIFmodEventInstance::OnDepNodeUpdate () {

    MOAITransform::OnDepNodeUpdate ();

    ZLVec3D vPos = GetLocalToWorldMtx().GetTranslation();
	SetInstanceLoc ( vPos );
}