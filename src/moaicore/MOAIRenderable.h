// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIRENDERABLE_H
#define	MOAIRENDERABLE_H

#include <moaicore/MOAILua.h>

//================================================================//
// MOAIRenderable
//================================================================//
/**	@name	MOAIRenderable
	@text	Abstract base class for objects that can be rendered by MOAIRenderMgr.
*/
class MOAIRenderable :
	public virtual MOAILuaObject {
public:

	//----------------------------------------------------------------//
						MOAIRenderable			();
	virtual				~MOAIRenderable			();
	virtual void		Render					() = 0;
};

#endif
