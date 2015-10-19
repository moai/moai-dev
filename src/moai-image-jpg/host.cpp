// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-image-jpg/host.h>
#include <moai-image-jpg/MOAIImageFormatJpg.h>

//================================================================//
// host
//================================================================//

//----------------------------------------------------------------//
void AKUImageJpgContextInitialize () {

	MOAIImageFormatMgr::Get ().RegisterFormat < MOAIImageFormatJpg >( "jpg" );
}
