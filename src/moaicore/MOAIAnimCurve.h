// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMCURVE_H
#define	MOAIANIMCURVE_H

#include <moaicore/MOAIAnimSample.h>
#include <moaicore/MOAILua.h>
#include <moaicore/MOAINode.h>
#include <moaicore/MOAIWeakPtr.h>

//================================================================//
// MOAIAnimKey
//================================================================//
class MOAIAnimKey {
public:

	float		mTime;
	u32			mMode;
	float		mWeight;
	
	bool operator < ( const MOAIAnimKey &rhs ) const {
		return mTime < rhs.mTime;
	}
	
	bool operator > ( const MOAIAnimKey &rhs ) const {
		return mTime > rhs.mTime;
	}
};

//================================================================//
// MOAIAnimCurve
//================================================================//
/**	@name	MOAIAnimCurve
	@text	Piecewise animation function with one input (time) and one output (value).
	
	@attr	ATTR_TIME
	@attr	ATTR_VALUE
	
	@attr  CLAMP
	@attr  WRAP
	@attr  MIRROR
	@attr  APPEND
	
	@attr	TYPE_BOOL
	@attr	TYPE_FLOAT
	@attr	TYPE_INDEX
	@attr	TYPE_INT
	@attr	TYPE_QUATERNION
	@attr	TYPE_TRANSFORM_2D
	@attr	TYPE_TRANSFORM_3D
	@attr	TYPE_VEC2
	@attr	TYPE_VEC3
*/
class MOAIAnimCurve :
	public virtual MOAINode {
private:

	USLeanArray < MOAIAnimKey > mKeys;

	float	mTime;
	u32		mWrapMode;
	
	u32		mSampleType;
	size_t	mSampleSize;
	void*	mBuffer;

	MOAIAnimSample mValue;

	//----------------------------------------------------------------//
	static int		_getLength			( lua_State* L );
	static int		_getValueAtTime		( lua_State* L );
	static int		_reserveKeys		( lua_State* L );
	static int		_setKey				( lua_State* L );
	static int		_setSample			( lua_State* L );
	static int		_setWrapMode		( lua_State* L );

	//----------------------------------------------------------------//
	inline void* GetBufferForKey ( u32 keyID ) {
		return ( void* )(( size_t )this->mBuffer + ( keyID * this->mSampleSize ));
	}

public:
	
	DECL_LUA_FACTORY ( MOAIAnimCurve )
	DECL_ATTR_HELPER ( MOAIAnimCurve )
	
	enum {
		ATTR_TIME,
		ATTR_VALUE,
		TOTAL_ATTR,
	};

	enum {
		CLAMP,
		WRAP,
		MIRROR,
		APPEND,
	};
	
	//----------------------------------------------------------------//
	bool			ApplyAttrOp				( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	void			Clear					();
	void			Draw					( u32 resolution ) const;
	u32				FindKeyID				( float time ) const;
	void			GetCurveDelta			( MOAIAnimSample& sample );
	void			GetDelta				( MOAIAttrOp& attrOp, float t0, float t1 );
	float			GetLength				() const;
	void			GetSampleForKey			( MOAIAnimSample& sample, u32 keyID );
	void			GetSampleForTime		( MOAIAnimSample& sample, float time );
	void			GetValue				( MOAIAttrOp& attrOp, float time );
	void			Init					( u32 total, u32 type );
					MOAIAnimCurve			();
					~MOAIAnimCurve			();
	void			OnDepNodeUpdate			();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SetKey					( u32 id, float time, u32 mode, float weight = 1.0f );
	void			SetSample				( u32 id, const MOAIAnimSample& sample );
	float			WrapTimeValue			( float t, float &repeat ) const;
};

#endif
