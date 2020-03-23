// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISHADER_H
#define	MOAISHADER_H

#include <moai-gfx/MOAIHasGfxScript.h>
#include <moai-gfx/MOAIUniformHandle.h>

// uniform buffer - attach statically (like a texture)
// uniform buffer - assemble dynamically (from lights)
// attach to shader at binding point
// light - fragment of uniform buffer (?)
// light - complete uniform buffer (?)
// light - trigger multiple redraws (?)
// bind in render pass (for object)

// shader has an implicit light (which is a collection of uniforms)

//================================================================//
// MOAIShader
//================================================================//
class MOAIShader :
	public virtual MOAIHasGfxScript {
protected:

	//----------------------------------------------------------------//
	virtual MOAIUniformHandle		MOAIShader_GetUniformHandle		( ZLIndex uniformID, ZLIndex index ) = 0;
	virtual bool					MOAIShader_IsReadyForUse		() const = 0;
	
public:

	//----------------------------------------------------------------//
	MOAIUniformHandle 		GetUniformHandle 		( ZLIndex uniformID, ZLIndex index );
	bool					HasProgram				() const;
	bool					IsReadyForUse			() const;
							MOAIShader				();
							~MOAIShader				();
};

#endif
