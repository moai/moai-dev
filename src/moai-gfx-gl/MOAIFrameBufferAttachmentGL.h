// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFERATTACHMENTGL_H
#define	MOAIFRAMEBUFFERATTACHMENTGL_H

#include <moai-gfx-gl/MOAIGfxResourceGL.h>

//================================================================//
// MOAIFrameBufferAttachmentGL
//================================================================//
class MOAIFrameBufferAttachmentGL :
	public virtual MOAIGfxResourceGL {
private:
	
	//----------------------------------------------------------------//
	virtual bool		MOAIFrameBufferAttachmentGL_Attach			( ZLGfx& gfx, ZLGfxEnum::_ target, ZLGfxEnum::_ attachment, s32 level, s32 layer ) = 0;
	
	//----------------------------------------------------------------//
//	void				_RegisterLuaClass							( RTTIVisitorHistory& history, MOAILuaState& state );
//	void				_RegisterLuaFuncs							( RTTIVisitorHistory& history, MOAILuaState& state );

public:
		
	//----------------------------------------------------------------//
	bool				Attach								( ZLGfx& gfx, ZLGfxEnum::_ target, ZLGfxEnum::_ attachment, s32 level, s32 layer );
						MOAIFrameBufferAttachmentGL			();
						~MOAIFrameBufferAttachmentGL		();
};

#endif
