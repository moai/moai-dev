// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLGFXDEVICE_H
#define ZLGFXDEVICE_H

//================================================================//
// ZLGfxDevice
//================================================================//
class ZLGfxDevice {
public:

	//----------------------------------------------------------------//
	static void				Begin					();
	static void				End						();
	static void				Finalize				();
	static u32				GetCap					( u32 cap );
	ZLGfxHandle*			GetCurrentFramebuffer	();
	static u32				GetError				();
	static cc8*				GetErrorString			( u32 error );
	static cc8*				GetString				( u32 stringID );
	static void				Initialize				();
};

#endif
