// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITRAITED_H
#define	MOAITRAITED_H

#include <moaicore/MOAINode.h>

class MOAIPartition;
class MOAIShader;

//================================================================//
// MOAITraits
//================================================================//
/**	@name	MOAITraits
	@text	Abstracts heritable prop traits.
	
	@const INHERIT_LOC
	@const INHERIT_TRANSFORM
	@const INHERIT_COLOR
	@const INHERIT_FRAME
	@const INHERIT_PARTITION
	@const INHERIT_SHADER	
*/
class MOAITraits :
	public virtual MOAINode {
protected:
	
	USWeak < MOAITraits >		mTraitSource;
	u32							mTraitMask;

	//----------------------------------------------------------------//
	static int	_setTraitMask		( lua_State* L );
	static int	_setTraitSource		( lua_State* L );

public:
	
	DECL_LUA_FACTORY ( MOAITraits )
	
	enum {
		INHERIT_LOC				= 0x00000001,
		INHERIT_TRANSFORM		= 0x00000002,
		INHERIT_COLOR			= 0x00000004,
		INHERIT_FRAME			= 0x00000008,
		INHERIT_PARTITION		= 0x00000010,
		INHERIT_SHADER			= 0x00000020,
	};
	
	static const u32 DEFAULT_MASK = 0x00000007;
	
	//----------------------------------------------------------------//
	virtual USColorVec			GetColorTrait			();
	virtual USRect*				GetFrameTrait			();
	virtual MOAIPartition*		GetPartitionTrait		();
	virtual MOAIShader*			GetShaderTrait			();
	virtual const USAffine2D*	GetTransformTrait		();
								MOAITraits				();
								~MOAITraits				();
	void						RegisterLuaClass		( USLuaState& state );
	void						RegisterLuaFuncs		( USLuaState& state );
	void						SetTraitSource			( MOAITraits* traitSource );
	STLString					ToString				();
};

#endif
