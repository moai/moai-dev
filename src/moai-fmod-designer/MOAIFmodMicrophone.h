// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFMODMICROPHONE_H
#define	MOAIFMODMICROPHONE_H

#include <moai-sim/headers.h>

//================================================================//
// MOAIFmodMicrophone
//================================================================//
/**	@lua	MOAIFmodMicrophone
	@text	The in-game Microphone, with respect to which
           all the sounds are heard in the game. The Event Manager
           (MOAIFmodEventManager) must be initialized before the
           Microphone can be accessed. Should only be grabbed from
           MOAIFmodEventMgr
*/
class MOAIFmodMicrophone :
	public MOAITransform {
private:

	//----------------------------------------------------------------//    

public:

    DECL_LUA_FACTORY ( MOAIFmodMicrophone )
    DECL_ATTR_HELPER ( MOAIFmodMicrophone )

	//----------------------------------------------------------------//
                MOAIFmodMicrophone	();                
	virtual		~MOAIFmodMicrophone	();
    bool		ApplyAttrOp				( u32 attrID, MOAIAttrOp& attrOp, u32 op );
    void        OnDepNodeUpdate         ();
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
    void	    SetMicLoc				( ZLVec3D& vPos );
};

#endif
