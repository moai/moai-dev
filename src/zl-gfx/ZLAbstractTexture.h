// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLABSTRACTTEXTURE_H
#define	ZLABSTRACTTEXTURE_H

//================================================================//
// ZLAbstractTexture
//================================================================//
class ZLAbstractTexture :
	public virtual RTTIBase,
	public virtual ZLRefCountedObject {
protected:

	//----------------------------------------------------------------//
	virtual u32		ZLAbstractTexture_GetHeight		() const = 0;
	virtual u32		ZLAbstractTexture_GetWidth		() const = 0;

public:

	//----------------------------------------------------------------//
	u32				GetHeight						() const;
	u32				GetWidth						() const;
					ZLAbstractTexture				();
					~ZLAbstractTexture				();
};

#endif
