// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFERATTACHABLEGL_H
#define	MOAIFRAMEBUFFERATTACHABLEGL_H

#include <moai-gfx-gl/MOAIGfxResourceGL.h>

//================================================================//
// MOAIFrameBufferAttachableGL
//================================================================//
class MOAIFrameBufferAttachableGL :
	public virtual MOAIGfxResourceGL {
private:
	
	//----------------------------------------------------------------//
	virtual bool		MOAIFrameBufferAttachableGL_Attach			( ZLGfx& gfx, ZLGfxEnum::_ attachment, ZLGfxEnum::_ target, s32 level, s32 layer ) = 0;
	
	//----------------------------------------------------------------//
//	void				_RegisterLuaClass							( RTTIVisitorHistory& history, MOAILuaState& state );
//	void				_RegisterLuaFuncs							( RTTIVisitorHistory& history, MOAILuaState& state );

public:
		
	//----------------------------------------------------------------//
	bool				Attach								( ZLGfx& gfx, ZLGfxEnum::_ target, ZLGfxEnum::_ attachment, s32 level, s32 layer );
						MOAIFrameBufferAttachableGL			();
						~MOAIFrameBufferAttachableGL		();
};

#endif
