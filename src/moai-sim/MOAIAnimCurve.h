// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIAnimCurve_H
#define	MOAIAnimCurve_H

#include <moai-sim/MOAINode.h>

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
// MOAIAnimKeySpan
//================================================================//
class MOAIAnimKeySpan {
public:

	u32		mKeyID;		// ID of first key in span
	float	mTime;		// Local time (within span)
	float	mCycle;		// Total number of times to add curve delta (repeat cycle)
};

//================================================================//
// MOAIAnimCurve
//================================================================//
/**	@lua	MOAIAnimCurve
	@text	Piecewise animation function with one input (time) and one output (value). This
			is the base class for typed animation curves (float, quaternion, etc.).
	
	@attr	ATTR_TIME
	@attr	ATTR_VALUE
	
	@attr  CLAMP
	@attr  WRAP
	@attr  MIRROR
	@attr  APPEND
*/
class MOAIAnimCurve :
	public virtual MOAINode {
protected:

	static const u32 NULL_KEY_ID = ( u32 )-1;

	ZLLeanArray < MOAIAnimKey > mKeys;

	float	mTime;
	u32		mWrapMode;

	//----------------------------------------------------------------//
	static int			_getLength			( lua_State* L );
	static int			_reserveKeys		( lua_State* L );
	static int			_setTime			( lua_State* L );
	static int			_setWrapMode		( lua_State* L );

	//----------------------------------------------------------------//
	virtual void		MOAIAnimCurve_ApplyValueAttrOp		( MOAIAttribute& attr, u32 op ) = 0;
	virtual void		MOAIAnimCurve_Draw					( u32 resolution ) const;
	virtual void		MOAIAnimCurve_GetDelta				( MOAIAttribute& attr, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const = 0;
	virtual float		MOAIAnimCurve_GetFloatForTime		( float t ) const;
	virtual void		MOAIAnimCurve_GetValue				( MOAIAttribute& attr, const MOAIAnimKeySpan& span ) const = 0;
	virtual void		MOAIAnimCurve_GetZero				( MOAIAttribute& attr ) const = 0;
	virtual void		MOAIAnimCurve_ReserveSamples		( u32 total ) = 0;

	//----------------------------------------------------------------//
	bool				MOAINode_ApplyAttrOp	( u32 attrID, MOAIAttribute& attr, u32 op );

public:
	
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
	void				Clear					();
	void				Draw					( u32 resolution ) const;
	u32					FindKeyID				( float time ) const;
	void				GetDelta				( MOAIAttribute& attr, float t0, float t1 );
	const MOAIAnimKey&	GetKey					( ZLIndex idx ) const;
	float				GetLength				() const;
	MOAIAnimKeySpan		GetSpan					( float time ) const;
	void				GetValue				( MOAIAttribute& attr, float time );
						MOAIAnimCurve			();
						~MOAIAnimCurve			();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				ReserveKeys				( u32 total );
	void				SetKey					( ZLIndex idx, float time, u32 mode, float weight = 1.0f );
	u32					Size					() const;
	float				WrapTime				( float t, float &repeat ) const;
};

#endif
