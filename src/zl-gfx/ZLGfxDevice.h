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
	static u32				GetCap					( ZGLEnum cap );
	static ZGLEnum			GetError				();
	static cc8*				GetErrorString			( ZGLEnum error );
	static cc8*				GetString				( ZGLEnum stringID );
	static void				Initialize				();
};

#endif
