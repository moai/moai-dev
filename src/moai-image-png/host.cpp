// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-image-png/host.h>
#include <moai-image-png/MOAIImageFormatPng.h>

//================================================================//
// host
//================================================================//

//----------------------------------------------------------------//
void AKUImagePngContextInitialize () {

	MOAIImageFormatMgr::Get().RegisterFormat < MOAIImageFormatPng >( "png" );
}
