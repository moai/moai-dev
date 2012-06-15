// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMCURVEBASE_H
#define	MOAIANIMCURVEBASE_H

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
// MOAIAnimKeySpan
//================================================================//
class MOAIAnimKeySpan {
public:

	u32		mKeyID;		// ID of first key in span
	float	mTime;		// Local time (within span)
	float	mCycle;		// Total number of times to add curve delta (repeat cycle)
};

//================================================================//
// MOAIAnimCurveBase
//================================================================//
/**	@name	MOAIAnimCurveBase
	@text	Piecewise animation function with one input (time) and one output (value). This
			is the base class for typed anim curves (float, quaternion, etc.).
	
	@attr	ATTR_TIME
	@attr	ATTR_VALUE
	
	@attr  CLAMP
	@attr  WRAP
	@attr  MIRROR
	@attr  APPEND
*/
class MOAIAnimCurveBase :
	public virtual MOAINode {
protected:

	static const u32 NULL_KEY_ID = ( u32 )-1;

	USLeanArray < MOAIAnimKey > mKeys;

	float	mTime;
	u32		mWrapMode;

	//----------------------------------------------------------------//
	static int			_getLength			( lua_State* L );
	static int			_reserveKeys		( lua_State* L );
	static int			_setWrapMode		( lua_State* L );

	//----------------------------------------------------------------//
	virtual void		ApplyValueAttrOp	( MOAIAttrOp& attrOp, u32 op ) = 0;
	virtual void		GetDelta			( MOAIAttrOp& attrOp, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const = 0;
	MOAIAnimKeySpan		GetSpan				( float time ) const;
	virtual void		GetValue			( MOAIAttrOp& attrOp, const MOAIAnimKeySpan& span ) const = 0;
	virtual void		GetZero				( MOAIAttrOp& attrOp ) const = 0;
	virtual void		ReserveSamples		( u32 total ) = 0;

public:
	
	DECL_ATTR_HELPER ( MOAIAnimCurveBase )
	
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
	bool				ApplyAttrOp				( u32 attrID, MOAIAttrOp& attrOp, u32 op );
	void				Clear					();
	virtual void		Draw					( u32 resolution ) const;
	u32					FindKeyID				( float time ) const;
	void				GetDelta				( MOAIAttrOp& attrOp, float t0, float t1 );
	const MOAIAnimKey&	GetKey					( u32 id ) const;
	float				GetLength				() const;
	void				GetValue				( MOAIAttrOp& attrOp, float time );
						MOAIAnimCurveBase		();
						~MOAIAnimCurveBase		();
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				ReserveKeys				( u32 total );
	void				SetKey					( u32 id, float time, u32 mode, float weight = 1.0f );
	u32					Size					() const;
	float				WrapTime				( float t, float &repeat ) const;
};

#endif
