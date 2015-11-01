// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-image-pvr/host.h>
#include <moai-image-pvr/MOAIImageFormatPvr.h>

//================================================================//
// host
//================================================================//

//----------------------------------------------------------------//
void AKUImagePvrContextInitialize () {

	MOAIImageFormatMgr::Get ().RegisterFormat < MOAIImageFormatPvr >( "pvr" );
}
