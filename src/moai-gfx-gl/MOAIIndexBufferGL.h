// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIINDEXBUFFERGL_H
#define	MOAIINDEXBUFFERGL_H

#include <moai-gfx-gl/MOAIGfxBufferGL.h>

//================================================================//
// MOAIIndexBufferGL
//================================================================//
/**	@lua	MOAIIndexBufferGL
	@text	Buffer for vertex indices.
*/
class MOAIIndexBufferGL :
	public virtual MOAIIndexBuffer,
	public virtual MOAIGfxBufferGL {
protected:

	//----------------------------------------------------------------//
	
public:
	
	friend class MOAIGfxMgrGL_DisplayListClerkGL;
	
	DECL_LUA_FACTORY ( MOAIIndexBufferGL )
	
	//----------------------------------------------------------------//
					MOAIIndexBufferGL		();
					~MOAIIndexBufferGL		();
};

#endif
