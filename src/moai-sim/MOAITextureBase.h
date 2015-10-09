// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTUREBASE_H
#define	MOAITEXTUREBASE_H

class MOAISingleTexture;

//================================================================//
// MOAITextureBase
//================================================================//
class MOAITextureBase :
	public virtual MOAILuaObject {
public:

	//----------------------------------------------------------------//
	virtual u32					CountActiveUnits		() = 0;
	virtual MOAISingleTexture*	GetTextureForUnit		( u32 unit ) = 0;
								MOAITextureBase			() {}
	virtual						~MOAITextureBase		() {}
};

#endif
