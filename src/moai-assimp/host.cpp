// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-assimp/host.h>
#include <moai-assimp/headers.h>
#include <moai-assimp/MOAIAssimpMesh.h>
#include <moai-assimp/MOAIAssimpCamera.h>

//================================================================//
// AKUBulletPhysics
//================================================================//

//----------------------------------------------------------------//
void AKUAssimpAppFinalize () {
}

//----------------------------------------------------------------//
void AKUAssimpAppInitialize () {
}

//----------------------------------------------------------------//
void AKUAssimpContextInitialize () {

//    REGISTER_LUA_CLASS ( MOAIImporterFactory )
//    REGISTER_LUA_CLASS ( MOAIImporterSingleton )


    REGISTER_LUA_CLASS ( MOAIAssimpScene  )
    REGISTER_LUA_CLASS ( MOAIAssimpMesh   )
    REGISTER_LUA_CLASS ( MOAIAssimpCamera )

}
