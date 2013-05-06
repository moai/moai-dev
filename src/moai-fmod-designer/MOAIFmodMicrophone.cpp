// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-fmod-designer/MOAIFmodMicrophone.h>
#include <fmod.hpp>
#include <moai-fmod-designer/Source/EventManager.h>

#ifdef MOAI_OS_IPHONE
	#include <fmodiphone.h>
#endif

#ifdef MOAI_OS_NACL
#include <NaClFileSystem.h>
#include <moai_nacl.h>
#endif

//================================================================//
// MOAIFmodMicrophone
//================================================================//

//----------------------------------------------------------------//
MOAIFmodMicrophone::MOAIFmodMicrophone () {
    RTTI_BEGIN
        RTTI_EXTEND ( MOAITransform )        
    RTTI_END
}

//----------------------------------------------------------------//
MOAIFmodMicrophone::~MOAIFmodMicrophone () {
}

//----------------------------------------------------------------//
void MOAIFmodMicrophone::RegisterLuaClass ( MOAILuaState& state ) {

    MOAITransform::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIFmodMicrophone::RegisterLuaFuncs ( MOAILuaState& state ) {

    MOAITransform::RegisterLuaFuncs ( state );

    luaL_Reg regTable [] = {
        { "new",            MOAILogMessages::_alertNewIsUnsupported },
        { NULL, NULL }
    };

    luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAIFmodMicrophone::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {
    
    bool bReturn = MOAITransform::ApplyAttrOp ( attrID, attrOp, op );    

    if ( MOAIFmodMicrophoneAttr::Check ( attrID ) )
    {   
        ZLVec3D vPos = GetLocalToWorldMtx().GetTranslation();
        SetMicLoc ( vPos );
    }

    return bReturn;
}

//----------------------------------------------------------------//
void MOAIFmodMicrophone::OnDepNodeUpdate () {

    MOAITransform::OnDepNodeUpdate ();

    ZLVec3D vPos = GetLocalToWorldMtx().GetTranslation();
	SetMicLoc ( vPos );
}

//----------------------------------------------------------------//

void MOAIFmodMicrophone::SetMicLoc ( ZLVec3D& vPos )
{    
    FMODDesigner::tEventManager.SetMicrophoneTransform( vPos );
}