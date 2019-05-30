// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTURE_H
#define	MOAITEXTURE_H

//================================================================//
// MOAITexture
//================================================================//
class MOAITexture :
	public virtual MOAILuaObject {
protected:

	//----------------------------------------------------------------//
	virtual u32		MOAITexture_GetHeight		() const = 0;
	virtual u32		MOAITexture_GetWidth		() const = 0;

public:

	//----------------------------------------------------------------//
	u32				GetHeight					() const;
	u32				GetWidth					() const;
					MOAITexture					();
					~MOAITexture				();
};

#endif
