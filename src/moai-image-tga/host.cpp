// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-image-tga/host.h>
#include <moai-image-tga/MOAIImageFormatTga.h>

//================================================================//
// host
//================================================================//

//----------------------------------------------------------------//
void AKUImageTgaContextInitialize () {

	MOAIImageFormatMgr::Get().RegisterFormat < MOAIImageFormatTga >( "tga" );
}
