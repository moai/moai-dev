// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIAnimCurve_H
#define	MOAIAnimCurve_H

//================================================================//
// MOAIAnimKey
//================================================================//
class MOAIAnimKey {
public:

	ZLReal		mTime;
	u32			mMode;
	ZLReal		mWeight;
	
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

	ZLIndex		mKeyID;		// ID of first key in span
	ZLReal		mTime;		// Local time (within span)
	ZLReal		mCycle;		// Total number of times to add curve delta (repeat cycle)
	
	//----------------------------------------------------------------//
	MOAIAnimKeySpan () :
		mKeyID ( ZLIndexOp::INVALID ),
		mTime ( 0.0 ),
		mCycle ( 0.0 ) {
	}
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

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	ZLLeanArray < MOAIAnimKey > mKeys;

	ZLReal		mTime;
	u32			mWrapMode;

	//----------------------------------------------------------------//
	static int			_getLength			( lua_State* L );
	static int			_reserveKeys		( lua_State* L );
	static int			_setTime			( lua_State* L );
	static int			_setWrapMode		( lua_State* L );

	//----------------------------------------------------------------//
	virtual void		MOAIAnimCurve_ApplyValueAttrOp		( ZLAttribute& attr, u32 op ) = 0;
	virtual void		MOAIAnimCurve_Draw					( u32 resolution ) const;
	virtual void		MOAIAnimCurve_GetDelta				( ZLAttribute& attr, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const = 0;
	virtual ZLReal		MOAIAnimCurve_GetFloatForTime		( ZLReal t ) const;
	virtual void		MOAIAnimCurve_GetValue				( ZLAttribute& attr, const MOAIAnimKeySpan& span ) const = 0;
	virtual void		MOAIAnimCurve_GetZero				( ZLAttribute& attr ) const = 0;
	virtual void		MOAIAnimCurve_ReserveSamples		( u32 total ) = 0;

	//----------------------------------------------------------------//
	void				MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );
	bool				MOAINode_ApplyAttrOp				( ZLAttrID attrID, ZLAttribute& attr, u32 op );

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
	ZLIndex				FindKeyID				( ZLReal time ) const;
	void				GetDelta				( ZLAttribute& attr, ZLReal t0, ZLReal t1 );
	const MOAIAnimKey&	GetKey					( ZLIndex idx ) const;
	ZLReal				GetLength				() const;
	MOAIAnimKeySpan		GetSpan					( ZLReal time ) const;
	void				GetValue				( ZLAttribute& attr, ZLReal time );
						MOAIAnimCurve			();
						~MOAIAnimCurve			();
	void				ReserveKeys				( u32 total );
	void				SetKey					( ZLIndex idx, ZLReal time, u32 mode, ZLReal weight = 1.0f );
	u32					Size					() const;
	ZLReal				WrapTime				( ZLReal t, ZLReal &repeat ) const;
};

#endif
