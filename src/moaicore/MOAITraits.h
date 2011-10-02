// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITRAITED_H
#define	MOAITRAITED_H

#include <moaicore/MOAIBlendMode.h>
#include <moaicore/MOAINode.h>

class MOAIPartition;
class MOAIShader;

//================================================================//
// MOAITraitLink
//================================================================//
class MOAITraitLink {
private:

	friend class MOAITraits;
	
	USWeak < MOAITraits >		mSource;
	u32							mMask;
	MOAITraitLink*				mNext;
};

//================================================================//
// MOAITraitsBuffer
//================================================================//
class MOAITraitsBuffer {
private:

	friend class MOAITraits;

	enum {
		SOURCE_LOC,
		SOURCE_TRANSFORM,
		SOURCE_COLOR,
		SOURCE_FRAME,
		SOURCE_PARTITION,
		SOURCE_SHADER,
		SOURCE_VISIBLE,
		SOURCE_BLEND_MODE,
		TOTAL_SOURCES,
	};

	MOAITraits* mSources [ TOTAL_SOURCES ];
	u32 mMask;

public:

	//----------------------------------------------------------------//
	MOAIBlendMode		GetBlendModeTrait		();
	USColorVec			GetColorTrait			();
	USRect*				GetFrameTrait			();
	const USAffine2D*	GetLocTrait				();
	MOAIPartition*		GetPartitionTrait		();
	MOAIShader*			GetShaderTrait			();
	const USAffine2D*	GetTransformTrait		();
	bool				GetVisibleTrait			();
	bool				HasTrait				( u32 mask );
	bool				HasTraits				();
};

//================================================================//
// MOAITraits
//================================================================//
/**	@name	MOAITraits
	@text	Abstracts heritable prop traits.
	
	@const INHERIT_ALL
	@const INHERIT_LOC
	@const INHERIT_TRANSFORM
	@const INHERIT_COLOR
	@const INHERIT_FRAME
	@const INHERIT_PARTITION
	@const INHERIT_SHADER
	@const INHERIT_VISIBLE
	@const INHERIT_BLEND_MODE
*/
class MOAITraits :
	public virtual MOAINode {
private:
	
	friend class MOAITraitsBuffer;
	
	MOAITraitLink* mSourceList; // set of sources
	
	//----------------------------------------------------------------//
	static int	_setTraitSource		( lua_State* L );

	//----------------------------------------------------------------//
	virtual MOAIBlendMode		GetBlendModeTrait		();
	virtual USColorVec			GetColorTrait			();
	virtual USRect*				GetFrameTrait			();
	virtual const USAffine2D*	GetLocTrait				();
	virtual MOAIPartition*		GetPartitionTrait		();
	virtual MOAIShader*			GetShaderTrait			();
	virtual const USAffine2D*	GetTransformTrait		();
	virtual bool				GetVisibleTrait			();

protected:

	//----------------------------------------------------------------//
	void						AccumulateSources		( MOAITraitsBuffer& buffer );
	bool						HasTraitsSource			();

public:
	
	DECL_LUA_FACTORY ( MOAITraits )
	
	enum {
		INHERIT_LOC				= 0x00000001,
		INHERIT_TRANSFORM		= 0x00000002,
		INHERIT_COLOR			= 0x00000004,
		INHERIT_FRAME			= 0x00000008,
		INHERIT_PARTITION		= 0x00000010,
		INHERIT_SHADER			= 0x00000020,
		INHERIT_VISIBLE			= 0x00000040,
		INHERIT_BLEND_MODE		= 0x00000080,
	};
	
	static const u32 DEFAULT_MASK	= 0x00000046;
	static const u32 ALL_TRAITS		= 0xffffffff;
	
	//----------------------------------------------------------------//
	void						ClearTraitSources		();
	MOAITraits*					GetTraitSource			( u32 mask );
								MOAITraits				();
								~MOAITraits				();
	void						RegisterLuaClass		( USLuaState& state );
	void						RegisterLuaFuncs		( USLuaState& state );
	void						SetTraitSource			( MOAITraits* source, u32 mask );
	STLString					ToString				();
};

#endif
