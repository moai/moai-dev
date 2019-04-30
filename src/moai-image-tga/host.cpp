// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-image-tga/host.h>
#include <moai-image-tga/MOAIImageFormatTga.h>

//================================================================//
// host
//================================================================//

//----------------------------------------------------------------//
void AKUImageTgaContextInitialize () {

	ZLImageFormatMgr::Get().RegisterFormat < MOAIImageFormatTga >( "tga" );
}
