// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-fmod-designer/MOAIFmodEventMgr.h>
#include <moai-fmod-designer/MOAIFmodEventInstance.h>
#include <moai-fmod-designer/MOAIFmodMicrophone.h>
#include <fmod.hpp>
#include <moai-fmod-designer/Source/EventManager.h>
#include <moai-fmod-designer/Source/EventProperties.h>
#include <moai-fmod-designer/Source/SoundInitParams.h>

#ifdef MOAI_OS_NACL
#include <fmodnacl.h>
#include "moai_nacl.h"
#include "ppapi/c/ppb.h"
#include "ppapi/cpp/instance.h"
#endif

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getMemoryStats
	@text	Get memory usage.

	@opt	boolean blocking		Default value is 'false.'
	
	@out	number currentAlloc
	@out	number maxAlloc
*/
int	MOAIFmodEventMgr::_getMemoryStats ( lua_State* L ) {
	MOAILuaState state ( L );
	
	bool blocking = state.GetValue < bool > ( 1, false );
	
	int currentAlloc;
	int maxAlloc;
	
	FMOD_Memory_GetStats ( &currentAlloc, &maxAlloc, blocking );
	
	lua_pushnumber ( state, currentAlloc );
	lua_pushnumber ( state, maxAlloc );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	init
	@text	Initializes the sound system.

	@out	boolean enabled
*/
int MOAIFmodEventMgr::_init ( lua_State* L ) {
    MOAILuaState state ( L );
	
#ifdef MOAI_OS_NACL
	printf ( "Cannot initialize FMOD Designer on background thread\n" );
	return 0;
#endif

    FMODDesigner::SoundInitParams params = FMODDesigner::SoundInitParams();

    params.m_soundMemoryMB = state.GetField < u32 >( -1, "soundMemoryMB", 32 );
    params.m_rsxMemoryMB = state.GetField < u32 > ( -1, "rsxMemoryMB", 0 );
    params.m_voiceLRUBufferMB = state.GetField < u32 > ( -1, "voiceLRUBufferMB", 0 );
    params.m_voiceLRUMaxMB = state.GetField < u32 > ( -1, "voiceLRUMaxMB", 0 );
    params.m_nVirtualChannels = state.GetField < u32 > ( -1, "nVirtualChannels", 256 );
    params.m_nRealChannels = state.GetField < u32 > ( -1, "nRealChannels", 32 );
    params.m_nPCMCodecs = state.GetField < u32 > ( -1, "nPCMCodecs", 16 );    
    params.m_nADPCMCodecs = state.GetField < u32 > ( -1, "nADPCMCodecs", 32 );
    params.m_nCompressedCodecs = state.GetField < u32 > ( -1, "nCompressedCodecs", 32 );
    params.m_nMaxInputChannels = state.GetField < u32 > ( -1, "nMaxInputChannels", 6 );
    params.m_enableSoundSystem = state.GetField < bool > ( -1, "enableSoundSystem", true );
    params.m_enableDistantLowpass = state.GetField < bool > ( -1, "enableDistantLowpass", false );
    params.m_enableEnvironmentalReverb = state.GetField < bool > ( -1, "enableEnvironmentalReverb", true );
    params.m_enableNear2DBlend = state.GetField < bool > ( -1, "enableNear2DBlend", false );
    params.m_enableAuditioning = state.GetField < bool > ( -1, "enableAuditioning", false );
    params.m_enableProfiling = state.GetField < bool > ( -1, "enableProfiling", false );
    params.m_enableFsCallbacks = state.GetField < bool > ( -1, "enableFsCallbacks", false );
    params.m_disableSound = state.GetField < bool > ( -1, "disableSound", false );
    params.m_dopplerScale = state.GetField < float > ( -1, "dopplerScale", 0.f );

    const bool b = FMODDesigner::tEventManager.Init ( params );
    lua_pushboolean ( L, b );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	isEnabled
	@text	Returns true if the Event Manager is enabled (active).

	@out	boolean enabled    True if the Event Manager is active, false otherwise
*/

int MOAIFmodEventMgr::_isEnabled ( lua_State* L ) {
    MOAILuaState state ( L );

    bool bEnabled = FMODDesigner::tEventManager.IsEnabled();
    lua_pushboolean ( L, bEnabled );

    return 1;
}

//----------------------------------------------------------------//
/**	@lua	loadProject
	@text	Loads a project from disk, but does not load any wav or instance data. 
           Projects must be loaded before sounds can be played from them.
           For special voice projects, use loadVoiceProject() 
    
    @in    string  projectName   The name of the .fev file (path should be relative from project root)  

	@out	boolean loaded        True if the project successfully loaded, false otherwise
*/
int MOAIFmodEventMgr::_loadProject ( lua_State* L ) {
	MOAILuaState state ( L );
	
    cc8* projectName = state.GetValue < cc8* > ( 1, "" );
    if ( projectName[0] != '\0' )
    {
        bool bLoaded = FMODDesigner::tEventManager.LoadProject( projectName );

        lua_pushboolean ( L, bLoaded );
	    return 1;
    }

    return 0;
}

//----------------------------------------------------------------//
/**	@lua	unloadProject
    @text	Unloads all data associated with a particular project.
           Completely flushes all memory associated with the project.
           For special voice projects, use unloadAllVoiceProjects() 
    
    @in    string  projectName   The name of the .fev file (path should be relative from project root)  

	@out	boolean unloaded      True if the project is no longer loaded after this call, false otherwise.
*/
int MOAIFmodEventMgr::_unloadProject ( lua_State* L ) {
	MOAILuaState state ( L );
	
    cc8* projectName = state.GetValue < cc8* > ( 1, "" );
    if ( projectName[0] != '\0' )
    {
        bool bLoaded = FMODDesigner::tEventManager.UnloadProject( projectName );

        lua_pushboolean ( L, bLoaded );
        return 1;
    }

    return 0;
}

//----------------------------------------------------------------//
/**	@lua	loadVoiceProject
	@text	Calls LoadProject and does all attendant special voice load stuff, as well.
           For regular projects, use loadProject() 
    
    @in    string  projectName   The name of the .fsb file (path should be relative from project root)
	@out	boolean loaded        True if the voice project successfully loaded, false otherwise
*/
int MOAIFmodEventMgr::_loadVoiceProject ( lua_State* L ) {
	MOAILuaState state ( L );
	
    cc8* projectName = state.GetValue < cc8* > ( 1, "" );
    if ( projectName[0] != '\0' )
    {
		// Try to load the cached line information (e.g. normalized code and line length)
		typedef std::pair<cc8*, float> LineCodeInfo;
		std::vector<LineCodeInfo> lineCodeInfo;
		if ( lua_istable ( state, 2 ) ) {

			u16 numLinesCodes = lua_objlen ( state, 2 );
			for ( u16 i = 0; i < numLinesCodes; i++ ) {

				lua_pushinteger ( state, i + 1 );
				lua_gettable ( state, -2 );
		
				if ( lua_istable ( state, -1 ) ) {

					assert ( lua_objlen ( state, -1 ) == 2 );

					cc8* lineCodeName = 0;
					float lineLength = 0;

					for ( u16 j = 0; j < 2; j++ ) {
					
						lua_pushinteger ( state, j + 1 );
						lua_gettable ( state, -2 );

						if ( j == 0 ) {
							assert ( lua_isstring ( state, -1 ) );
							lineCodeName = lua_tostring ( state, -1 );
						}
						else {
							assert ( lua_isnumber ( state, -1 ) );
							lineLength = (float) lua_tonumber ( state, -1 );
						}

						lua_pop ( state, 1 );
					}
				
					lineCodeInfo.push_back ( LineCodeInfo ( lineCodeName, lineLength ) );
				}

				lua_pop ( state, 1 );
			}
		}

		bool bLoaded = FMODDesigner::tEventManager.LoadVoiceProject( projectName, lineCodeInfo.size () > 0 ? &lineCodeInfo : 0 );

        lua_pushboolean ( L, bLoaded );
        return 1;
    }

    return 0;
}

//----------------------------------------------------------------//
/**	@lua	unloadAllVoiceProjects
    @text	Calls UnloadProject and does all attendant special voice unload stuff, as well.
           For regular projects, use unloadProject()
    
	@out	boolean unloaded   True if voice projects are no longer loaded after this call, false otherwise.
*/
int MOAIFmodEventMgr::_unloadAllVoiceProjects ( lua_State* L ) {
	MOAILuaState state ( L );
	
    bool bLoaded = FMODDesigner::tEventManager.UnloadVoiceProjects();

    lua_pushboolean ( L, bLoaded );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	loadGroup
    @text	Loads the wave data and instance data associated with a particular group.
           Groups are reference counted internally, with each call to LoadGroup incrementing the ref count.           
    
    @in    string  groupPath     Should be of the form ProjectName/GroupName
    @in    boolean persistent    Passing true means that this group is expected to be around for the entire
                                 duration of the app
    @in    boolean blockOnLoad   Passing true means that the main thread will block while loading wav data
                                 for this Group.

	@out	boolean loaded        True if the Group successfully loaded after this call, false otherwise
*/

int MOAIFmodEventMgr::_loadGroup ( lua_State* L ) {
    MOAILuaState state ( L );

    cc8* groupName = state.GetValue < cc8* > ( 1, "" );
    if ( groupName[0] != '\0' )
    {
        bool bPersistent = state.GetValue ( 2, false );
        bool bBlockOnLoad = state.GetValue ( 3, false );

        bool bLoaded = FMODDesigner::tEventManager.LoadGroup ( STLString( groupName ), bPersistent, bBlockOnLoad );
         
        lua_pushboolean ( L, bLoaded );

        return 1;
    }

    return 0;
}

//----------------------------------------------------------------//
/**	@lua	unloadGroup
    @text	Unloads the wave data and instance data associated with a particular group.
           Groups might not be unloaded immediately either because their reference count is not zero,
           or because the sound system is not ready to release the group.            
    
    @in    string  groupPath            Should be of the form ProjectName/GroupName
    @in    boolean unloadImmediately    If true is passed in, it'll try to unload now, but won't block. 
                                        Passing false will allow it to wait before trying to unload

	@out	boolean unloaded             True if the Group is no longer loaded after this call, false otherwise
*/

int MOAIFmodEventMgr::_unloadGroup ( lua_State* L ) {
    MOAILuaState state ( L );

    cc8* groupName = state.GetValue < cc8* > ( 1, "" );
    if ( groupName[0] != '\0' )
    {
        bool bUnloadImmediately = state.GetValue ( 2, false );    

        bool bUnloaded = FMODDesigner::tEventManager.UnloadGroup ( STLString( groupName ), bUnloadImmediately );

        lua_pushboolean ( L, bUnloaded );

        return 1;

    }

    return 0;
}

//----------------------------------------------------------------//
/**	@lua	unloadPendingUnloads
	@text	Unloads all the pending unload groups. Use between act changes in the game.

	@opt	boolean blockOnUnload		Passing true means that the main thread will block while unloading
	@out	nil
*/

int MOAIFmodEventMgr::_unloadPendingUnloads ( lua_State* L ) {
    MOAILuaState state ( L );

    bool bBlockOnUnload = state.GetValue ( 1, false );
    FMODDesigner::tEventManager.UnloadPendingUnloads( bBlockOnUnload );
     
    return 0;
}

//----------------------------------------------------------------//
/**	@lua	setSoundCategoryVolume
    @text	Sets the volume for a sound category           
    
    @in    string  categoryName  Name of the category whose volume you wanna modify
    @in    number  newVolume     New volume to set for the category

	@out	nil
*/

int MOAIFmodEventMgr::_setSoundCategoryVolume ( lua_State* L ) {
    MOAILuaState state ( L );

    cc8* categoryName = state.GetValue < cc8* > ( 1, "" );
    if ( categoryName[0] != '\0' )
    {
        float fVolume = state.GetValue < float > ( 2, 0.f );    

        FMODDesigner::tEventManager.SetSoundCategoryVolume( STLString( categoryName ), fVolume );    
    }

    return 0;
}

//----------------------------------------------------------------//
/**	@lua	getSoundCategoryVolume
    @text	Gets the volume for a sound category           
    
    @in    string  categoryName      Name of the category whose volume you wanna know    
    
	@out	number  categoryVolume    The volume of the category
*/

int MOAIFmodEventMgr::_getSoundCategoryVolume ( lua_State* L ) {
    MOAILuaState state ( L );

    cc8* categoryName = state.GetValue < cc8* > ( 1, "" );
    if ( categoryName[0] != '\0' )
    {
        float fVolume = FMODDesigner::tEventManager.GetSoundCategoryVolume( STLString( categoryName ) );    
        lua_pushnumber( L, fVolume );

        return 1;
    }

    return 0;
}

//----------------------------------------------------------------//
/**	@lua	muteSoundCategory
    @text	Mute a sound category           
    
    @in    string  categoryName  Name of the category whose volume you wanna modify
    @in    boolean muteSetting   Whether to mute the category or not (Default: true)

	@out	nil
*/

int MOAIFmodEventMgr::_muteSoundCategory ( lua_State* L ) {
    MOAILuaState state ( L );

    cc8* categoryName = state.GetValue < cc8* > ( 1, "" );
    if ( categoryName[0] != '\0' )
    {
        bool bSetting = state.GetValue < bool > ( 2, true );    

        FMODDesigner::tEventManager.MuteSoundCategory( STLString( categoryName ), bSetting );    
    }

    return 0;
}

//----------------------------------------------------------------//
/**	@lua	isSoundCategoryMuted
    @text	Checks to see whether a sound category is muted           
    
    @in    string  categoryName      Name of the category whose volume you wanna know    
    
	@out	boolean isMuted           Returns whether the sound category is muted
*/

int MOAIFmodEventMgr::_isSoundCategoryMuted ( lua_State* L ) {
    MOAILuaState state ( L );

    cc8* categoryName = state.GetValue < cc8* > ( 1, "" );
    if ( categoryName[0] != '\0' )
    {
        bool bMuted = FMODDesigner::tEventManager.IsSoundCategoryMuted( STLString( categoryName ) );    
        lua_pushboolean( L, bMuted );

        return 1;
    }

    return 0;
}

//----------------------------------------------------------------//
/**	@lua	pauseSoundCategory
    @text	Mute a sound category           
    
    @in    string  categoryName      Name of the category whose volume you wanna modify
    @in    boolean pauseSetting      Whether to mute the category or not (Default: true)

	@out	nil
*/

int MOAIFmodEventMgr::_pauseSoundCategory ( lua_State* L ) {
    MOAILuaState state ( L );

    cc8* categoryName = state.GetValue < cc8* > ( 1, "" );
    if ( categoryName[0] != '\0' )
    {
        bool bSetting = state.GetValue < bool > ( 2, true );    

        FMODDesigner::tEventManager.PauseSoundCategory( STLString( categoryName ), bSetting );    
    }

    return 0;
}

//----------------------------------------------------------------//
/**	@lua	isSoundCategoryPaused
    @text	Checks to see whether a sound category is muted           
    
    @in    string  categoryName      Name of the category whose volume you wanna know    
    
	@out	boolean isMuted           Returns whether the sound category is muted
*/

int MOAIFmodEventMgr::_isSoundCategoryPaused ( lua_State* L ) {
    MOAILuaState state ( L );

    cc8* categoryName = state.GetValue < cc8* > ( 1, "" );
    if ( categoryName[0] != '\0' )
    {
        bool bPaused = FMODDesigner::tEventManager.IsSoundCategoryPaused( STLString( categoryName ) );    
        lua_pushboolean( L, bPaused );

        return 1;
    }

    return 0;
}

//----------------------------------------------------------------//
/**	@lua	playEvent2D
    @text	Plays an FMOD Event in 2D. Calling this function on 3D Events is undefined.
    
    @in    string                    eventName  Event to play
    @opt   boolean                   loopSound  Will force the Event to loop even if it does not in the data.
    
    @out   MOAIFmodEventInstance     eventInst  The Event instance
*/

int MOAIFmodEventMgr::_playEvent2D ( lua_State* L ) {
    MOAILuaState state ( L );

    cc8* eventName = state.GetValue < cc8* > ( 1, "" );
    if ( eventName[0] != '\0' ) {
        
        const FMODDesigner::Event* pEvent = MOAIFmodEventMgr::Get().GetEvent( eventName );
        if ( !pEvent )
        {
            FMODDesigner::Event actualEvent = FMODDesigner::Event( STLString( eventName ) );
            MOAIFmodEventMgr::Get().AddEvent( eventName, actualEvent );
            pEvent = MOAIFmodEventMgr::Get().GetEvent( eventName );
        }
        
        
        if ( pEvent->IsValid() ) {
            bool bLoop = state.GetValue( 2, false );
            
            // Try to play the sound
            FMODDesigner::EventHandle hEventHandle = FMODDesigner::tEventManager.PlayEvent2D( *pEvent, bLoop );

            // Create a MOAI Event Instance and push it out
            MOAIFmodEventInstance* pEventInstance = new MOAIFmodEventInstance();
            pEventInstance->SetEventInstance( hEventHandle );   

            pEventInstance->PushLuaUserdata ( state );

            return 1;
        }        
    }
     
    return 0;
}

//----------------------------------------------------------------//
/**	@lua	playEvent3D
    @text	Plays an FMOD Event 3D. Calling this function on 2D Events is harmless, but not advised.
    
    @in    string                    eventName  Event to play
    @opt   number                    x          x position of this sound                 
    @opt   number                    y          y position of this sound                 
    @opt   number                    z          z position of this sound                 
    @opt   boolean                   loopSound  Will force the Event to loop even if it does not in the data.
    
    @out   MOAIFmodEventInstance     eventInst  The Event instance
*/

int MOAIFmodEventMgr::_playEvent3D ( lua_State* L ) {
    MOAILuaState state ( L );

    cc8* eventName = state.GetValue < cc8* > ( 1, "" );
    if ( eventName[0] != '\0' ) {
        
        const FMODDesigner::Event* pEvent = MOAIFmodEventMgr::Get().GetEvent( eventName );
        if ( !pEvent )
        {
            FMODDesigner::Event actualEvent = FMODDesigner::Event( STLString( eventName ) );
            MOAIFmodEventMgr::Get().AddEvent( eventName, actualEvent );
            pEvent = MOAIFmodEventMgr::Get().GetEvent( eventName );
        }

        if ( pEvent->IsValid() ) {
            
            float x = state.GetValue < float > ( 2, 0.f );
            float y = state.GetValue < float > ( 3, 0.f );
            float z = state.GetValue < float > ( 4, 0.f );

            bool bLoop = state.GetValue( 5, false );
            
            // Try to play the sound
            FMODDesigner::EventHandle hEventHandle = FMODDesigner::tEventManager.PlayEvent3D( *pEvent, ZLVec3D( x, y, z ), bLoop );
            
            // Create a MOAI Event Instance and push it out
            MOAIFmodEventInstance* pEventInstance = new MOAIFmodEventInstance();
            pEventInstance->SetEventInstance( hEventHandle );                
            pEventInstance->PushLuaUserdata ( state );

            return 1; 
        }
    }
     
    return 0;
}

//----------------------------------------------------------------//
/**	@lua	playVoiceLine
    @text	Plays a voice line that exists in a loaded voice project.  
           Will play it 2D or 3D based on Event settings.  
           Uses a unique identifier for the line that is not the name
           of the Event -- although the event serves as a template
           for how the line will play.    
    
    @in    string         linename   Unique identifier for a voice line
    @in    string         eventName  The Event template to use for playing this line      
    @opt   number         x          x position of this sound                 
    @opt   number         y          y position of this sound                 
    @opt   number         z          z position of this sound               
    
    @out   MOAIFmodEventInstance eventInst  The Event instance
*/

int MOAIFmodEventMgr::_playVoiceLine ( lua_State* L ) {
    MOAILuaState state ( L );

    cc8* linename	= state.GetValue < cc8* >( 1, "" );
    if ( linename[0] != '\0' ) {

        FMODDesigner::LineCode lineCode = FMODDesigner::LineCode( linename );

        cc8* eventName = state.GetValue < cc8* > ( 2, "" );
        if ( eventName[0] != '\0' ) {

            const FMODDesigner::Event* pEvent = MOAIFmodEventMgr::Get().GetEvent( eventName );
            if ( !pEvent )
            {
                FMODDesigner::Event actualEvent = FMODDesigner::Event( STLString( eventName ) );
                MOAIFmodEventMgr::Get().AddEvent( eventName, actualEvent );
                pEvent = MOAIFmodEventMgr::Get().GetEvent( eventName );
            }

            if ( pEvent->IsValid() ) {            
                const FMODDesigner::EventProperties* pProperties = FMODDesigner::tEventManager.GetEventProperties( *pEvent );
                if( pProperties )
                {
                    FMODDesigner::EventHandle hEventHandle;

                    if ( !pProperties->m_is3D ) {
                        // Play the line, which should be the same as the event, at full volume
                        hEventHandle = FMODDesigner::tEventManager.PlayEvent2D( *pEvent, false, &lineCode );
                    }
                    else if ( pProperties->m_headRelative ) {
                        // Head relative sounds are basically 2D, except played as 3D w/ a zero position.
                        hEventHandle = FMODDesigner::tEventManager.PlayEvent3D( *pEvent, ZLVec3D( 0.f, 0.f, 0.f ), false, ZLVec3D( 0.f, 0.f, 0.f ), &lineCode );
                    }
                    else {
                        float x = state.GetValue < float > ( 3, 0.f );
                        float y = state.GetValue < float > ( 4, 0.f );
                        float z = state.GetValue < float > ( 5, 0.f );

                        hEventHandle = FMODDesigner::tEventManager.PlayEvent3D( *pEvent, ZLVec3D( x, y, z ), false, ZLVec3D( 0.f, 0.f, 0.f ), &lineCode );
                    }

                    if ( *hEventHandle )
                    {                    
                        MOAIFmodEventInstance* pEventInstance = new MOAIFmodEventInstance();
                        pEventInstance->SetEventInstance( hEventHandle );                
                        pEventInstance->PushLuaUserdata ( state );

                        return 1;
                    }
                }
            }
        }  
    }
     
    return 0;
}

//----------------------------------------------------------------//
/**	@lua	unloadEvent
	@text	Unloads the data associated with an Event. 
			All instances of this Event will be stopped when this call is made.
			Returns true if the Event is no longer loaded after this call.

	@in		string eventName		The name of the Event
	@opt	boolean blockOnUnload	Passing true means that the main thread will block while unloading
	
	@out	boolean eventUnloaded	True if the Event is no longer loaded after this call.
*/

int MOAIFmodEventMgr::_unloadEvent ( lua_State* L ) {
    MOAILuaState state ( L );

    cc8* eventName = state.GetValue < cc8* > ( 1, "" );
    if ( eventName[0] != '\0' ) {
        bool bBlockOnUnload = state.GetValue < bool > ( 2, false );
        FMODDesigner::Event actualEvent = FMODDesigner::Event ( eventName );
        bool bUnloaded = FMODDesigner::tEventManager.UnloadEvent ( actualEvent, bBlockOnUnload );

        lua_pushboolean ( L, bUnloaded );
        
        return 1;
    }
     
    return 0;
}

//----------------------------------------------------------------//
/**	@lua	stopAllEvents
    @text	Stops all events/sounds from playing.

	@out	nil
*/

int MOAIFmodEventMgr::_stopAllEvents ( lua_State* L ) {
    MOAILuaState state ( L );

    FMODDesigner::tEventManager.StopAllEvents();
     
    return 0;
}

//----------------------------------------------------------------//
/**	@lua	muteAllEvents
    @text	Stops all events/sounds from playing.

    @in    boolean muteSetting   Whether to mute (true) or unmute (false)
	@out	nil
*/

int MOAIFmodEventMgr::_muteAllEvents ( lua_State* L ) {
    MOAILuaState state ( L );

    bool bSetting = state.GetValue < bool > ( 1, true );

    FMODDesigner::tEventManager.MuteAllEvents( bSetting );
     
    return 0;
}

//----------------------------------------------------------------//
/**	@lua	preloadVoiceLine
    @text	Preload a high demand voice line

    @in    string eventName      The Event template to use for this line
    @in    string lineName       The unique ID of the line to preload

	@out	nil
*/

int MOAIFmodEventMgr::_preloadVoiceLine ( lua_State* L ) {
    MOAILuaState state ( L );

    cc8* eventName = state.GetValue < cc8* > ( 1, "" );
    if ( eventName[0] != '\0' ) {
        cc8* lineName = state.GetValue < cc8* > ( 2, "" );
        if ( lineName[0] != '\0' )
        {
            FMODDesigner::Event actualEvent = FMODDesigner::Event ( eventName );
            FMODDesigner::LineCode lineCode = FMODDesigner::LineCode ( lineName );

            FMODDesigner::tEventManager.AddLineToLRU ( actualEvent, lineCode );
        }        
    }
     
    return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDefaultReverb
    @text	Set the default regional Reverb

    @in    string reverbName      Name of the Reverb (defined in Designer)

	@out	nil
*/

int MOAIFmodEventMgr::_setDefaultReverb ( lua_State* L ) {
    MOAILuaState state ( L );

    cc8* reverbName = state.GetValue < cc8* > ( 1, "" );
    if ( reverbName[0] != '\0' ) {
        FMODDesigner::tEventManager.SetDefaultReverb ( STLString( reverbName ) );
    }
     
    return 0;
}

//----------------------------------------------------------------//
/**	@lua	setDistantOcclusion
    @text	Sets a lowpass filter on distant sounds -- a filter added to 
           everything greater than a certain distance (minRange to maxRange) 
           from the microphone to make it sound muffled/far away.                            

    @in    number minRange              Minimum distance from mic
    @in    number maxRange              Maximum distance from mic
    @in    number maxOcclusion          Maximum occlusion value

	@out	nil
*/

int MOAIFmodEventMgr::_setDistantOcclusion ( lua_State* L ) {
    MOAILuaState state ( L );

    float fMinRange = state.GetValue < float > ( 1, 0.f );
    float fMaxRange = state.GetValue < float > ( 2, 0.f );
    float fMaxOcclusion = state.GetValue < float > ( 3, 0.f );
    FMODDesigner::tEventManager.SetDistantOcclusion ( fMinRange, fMaxRange, fMaxOcclusion );
     
    return 0;
}

//----------------------------------------------------------------//
/**	@lua	setNear2DBlend
    @text	Blend sounds near the microphone to 2D sounds. 
           When 3D Events are playing near the microphone, their
           positioning becomes distracting rather than helpful/interesting,
           so this is a way to blend them together as if they were all
           taking place just at the mic.

    @in    number minRange              Minimum distance from mic
    @in    number maxRange              Maximum distance from mic
    @in    number maxLevel              Maximum pan level

	@out	nil
*/

int MOAIFmodEventMgr::_setNear2DBlend ( lua_State* L ) {
    MOAILuaState state ( L );

    float fMinRange = state.GetValue < float > ( 1, 0.f );
    float fMaxRange = state.GetValue < float > ( 2, 0.f );
    float fMaxPanLevel = state.GetValue < float > ( 3, 0.f );
	FMODDesigner::tEventManager.SetNear2DBlend( fMinRange, fMaxRange, fMaxPanLevel );
     
    return 0;
}

//----------------------------------------------------------------//
/**	@lua	getEventDuration
    @text	Returns the duration of an Event.  Although multiple sounds
           can potentially be played from 1 Event, to support determinism,
           a consistent duration will be returned for a given Event.
    
    @in    string  eventName  The name of the Event  
    @opt   string  lineName   Pass this is if you want the duration of a voice line
    
    @out   number  duration   Duration of the Event, nil if Event is invalid 
*/

int MOAIFmodEventMgr::_getEventDuration ( lua_State* L ) {
    MOAILuaState state ( L );

    cc8* eventName = state.GetValue < cc8* > ( 1, "" );
    if ( eventName[0] != '\0' ) {

        const FMODDesigner::Event* pEvent = MOAIFmodEventMgr::Get().GetEvent( eventName );
        if ( !pEvent )
        {
            FMODDesigner::Event actualEvent = FMODDesigner::Event( STLString( eventName ) );
            MOAIFmodEventMgr::Get().AddEvent( eventName, actualEvent );
            pEvent = MOAIFmodEventMgr::Get().GetEvent( eventName );
        }

        cc8* lineName = state.GetValue < cc8* > ( 2, "" );

        float fDuration = 0.f;
        if ( lineName[0] != '\0' ) {
            FMODDesigner::LineCode lineCode = FMODDesigner::LineCode( lineName );
            fDuration = pEvent->GetDuration( &lineCode );
        }
        else {
            fDuration = pEvent->GetDuration();
        }        
        
        lua_pushnumber( L, fDuration );
        
        return 1;
    }
     
    return 0;
}

//----------------------------------------------------------------//
/**	@lua	getMicrophone
    @text	Returns the game microphone.    
        
    @out   MOAIFmodMicrophone mic  The game microphone
*/

int MOAIFmodEventMgr::_getMicrophone ( lua_State* L ) {
    MOAILuaState state ( L );

    MOAIFmodMicrophone* pMic = MOAIFmodEventMgr::Get().AffirmMic ();
    pMic->PushLuaUserdata ( state );

    return 1;
}


//================================================================//
// MOAIFmodEventMgr
//================================================================//

MOAIFmodEventMgr::MOAIFmodEventMgr () : mMic ( 0 ) {
    RTTI_SINGLE ( MOAIFmodEventMgr )
}

//----------------------------------------------------------------//
MOAIFmodEventMgr::~MOAIFmodEventMgr () {
}

//----------------------------------------------------------------//
MOAIFmodMicrophone* MOAIFmodEventMgr::AffirmMic () {

    if ( !this->mMic ) {
        this->mMic = new MOAIFmodMicrophone ();
        this->LuaRetain ( this->mMic );
    }
    return this->mMic;
}

//----------------------------------------------------------------//
const FMODDesigner::Event* MOAIFmodEventMgr::GetEvent ( STLString eventName ) {

    STLMap < STLString, FMODDesigner::Event >::iterator it = mEventList.find ( eventName );
    if ( it != mEventList.end() )
    {
        return &it->second;
    }

    return NULL;
}

//----------------------------------------------------------------//
void MOAIFmodEventMgr::AddEvent ( STLString eventName, FMODDesigner::Event& actualEvent ) {

    if ( !mEventList.contains ( eventName ) )
    {
        mEventList.insert ( pair < STLString, FMODDesigner::Event > ( eventName, actualEvent ) );
    }
}

//----------------------------------------------------------------//
void MOAIFmodEventMgr::RegisterLuaClass ( MOAILuaState& state ) {
    UNUSED ( state );
	
	luaL_Reg regTable [] = {
		{ "getMemoryStats",		                _getMemoryStats },
		{ "init",				                _init },
        { "isEnabled",                          _isEnabled },

        { "loadProject",                        _loadProject },
		{ "unloadProject",                      _unloadProject },
        { "loadVoiceProject",                   _loadVoiceProject },
        { "unloadAllVoiceProjects",             _unloadAllVoiceProjects },

        { "loadGroup",                          _loadGroup },
        { "unloadGroup",                        _unloadGroup },
        { "unloadPendingUnloads",               _unloadPendingUnloads },                
        
        { "setSoundCategoryVolume",             _setSoundCategoryVolume },        
        { "getSoundCategoryVolume",             _getSoundCategoryVolume },        
        { "muteSoundCategory",                  _muteSoundCategory },        
        { "isSoundCategoryMuted",               _isSoundCategoryMuted },        
        { "pauseSoundCategory",                 _pauseSoundCategory },        
        { "isSoundCategoryPaused",              _isSoundCategoryPaused },        
        
        { "playEvent3D",                        _playEvent3D },        
        { "playEvent2D",                        _playEvent2D },     
        { "playVoiceLine",                      _playVoiceLine },
        { "unloadEvent",                        _unloadEvent },        
        { "stopAllEvents",                      _stopAllEvents },  
        { "muteAllEvents",                      _muteAllEvents },  
        { "getEventDuration",                   _getEventDuration },
        
        { "preloadVoiceLine",                   _preloadVoiceLine },        

        { "setDefaultReverb",                   _setDefaultReverb },                

        { "setDistantOcclusion",                _setDistantOcclusion },                
        { "setNear2DBlend",                     _setNear2DBlend },         
        
        { "getMicrophone",                      _getMicrophone },         

		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFmodEventMgr::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

