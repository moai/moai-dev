// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTTEXTURE_H
#define	MOAIABSTRACTTEXTURE_H

//================================================================//
// MOAIAbstractTexture
//================================================================//
class MOAIAbstractTexture :
	public virtual MOAILuaObject {
protected:

	//----------------------------------------------------------------//
	virtual u32		ZLAbstractTexture_GetHeight		() const = 0;
	virtual u32		ZLAbstractTexture_GetWidth		() const = 0;

public:

	//----------------------------------------------------------------//
	u32				GetHeight						() const;
	u32				GetWidth						() const;
					MOAIAbstractTexture				();
					~MOAIAbstractTexture			();
};

#endif
