// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMCURVE_H
#define	MOAIANIMCURVE_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAINode.h>
#include <moaicore/MOAIWeakPtr.h>

//================================================================//
// MOAIAnimKey
//================================================================//
class MOAIAnimKey {
public:

	float		mTime;
	float		mValue;
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
*/
class MOAIAnimCurve :
	public virtual MOAINode,
	public USLeanArray < MOAIAnimKey > {
private:

	float	mTime;
	float	mValue;
	u32		mWrapMode;

	//----------------------------------------------------------------//
	static int	_getLength		( lua_State* L );
	static int  _getValueAtTime ( lua_State* L );
	static int	_reserveKeys	( lua_State* L );
	static int	_setKey			( lua_State* L );
	static int	_setWrapMode	( lua_State* L );

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
	bool			ApplyAttrOp			( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	u32				FindKeyID			( float time ) const;
	bool			GetBoolValue		( float time ) const;
	float			GetFloatDelta		( float t0, float t1 ) const;
	float			GetFloatValue		( float time ) const;
	u32				GetIndexValue		( float time ) const;
	int				GetIntValue			( float time ) const;
	float			GetLength			() const;
					MOAIAnimCurve		();
					~MOAIAnimCurve		();
	void			OnDepNodeUpdate		();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			SetKey				( u32 id, float time, float value, u32 mode, float weight = 1.0f );
	float			WrapTimeValue		( float t, float &repeat ) const;
};

#endif
