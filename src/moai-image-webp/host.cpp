// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-image-webp/host.h>
#include <moai-image-webp/MOAIImageFormatWebP.h>

//================================================================//
// host
//================================================================//

//----------------------------------------------------------------//
void AKUImageWebPContextInitialize () {

	MOAIImageFormatMgr::Get ().RegisterFormat < MOAIImageFormatWebP >( "webp" );
}
