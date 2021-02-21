// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLGFXDEVICE_H
#define ZLGFXDEVICE_H

#include <zl-gfx/ZLGfxEnum.h>

//================================================================//
// ZLGfxDevice
//================================================================//
class ZLGfxDevice {
public:

	//----------------------------------------------------------------//
	static u32				GetCap							( ZLGfxEnum::_ cap );
	static ZLGfxEnum::_		GetError						();
	static cc8*				GetErrorString					( ZLGfxEnum::_ error );
	static cc8*				GetFrameBufferStatusString		( GLenum status );
	static cc8*				GetString						( ZLGfxEnum::_ stringID );
	static void				Initialize						();
};

#endif
