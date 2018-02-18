// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIANIMCURVEINDEX_H
#define	MOAIANIMCURVEINDEX_H

#include <moai-sim/MOAIAnimCurve.h>
#include <moai-sim/MOAINode.h>

//================================================================//
// MOAIAnimCurveIndex
//================================================================//
/**	@lua	MOAIAnimCurveIndex
	@text	Implementation of animation curve for integer index values.
*/
class MOAIAnimCurveIndex :
	public virtual MOAIAnimCurve {
private:

	ZLLeanArray < ZLIndex > mSamples;
	ZLIndex mValue;

	//----------------------------------------------------------------//
	static int		_getValueAtTime		( lua_State* L );
	static int		_setKey				( lua_State* L );

	//----------------------------------------------------------------//
	ZLIndex		GetValue			( const MOAIAnimKeySpan& span ) const;
	
	//----------------------------------------------------------------//
	void				MOAIAnimCurve_ApplyValueAttrOp		( MOAIAttribute& attr, u32 op );
	void				MOAIAnimCurve_GetDelta				( MOAIAttribute& attr, const MOAIAnimKeySpan& span0, const MOAIAnimKeySpan& span1 ) const;
	float				MOAIAnimCurve_GetFloatForTime		( float t ) const;
	void				MOAIAnimCurve_GetValue				( MOAIAttribute& attr, const MOAIAnimKeySpan& span ) const;
	void				MOAIAnimCurve_GetZero				( MOAIAttribute& attr ) const;
	void				MOAIAnimCurve_ReserveSamples		( u32 total );
	void				MOAINode_Update						();

public:
	
	DECL_LUA_FACTORY ( MOAIAnimCurveIndex )
	
	//----------------------------------------------------------------//
	ZLIndex		GetValue				( float time ) const;
					MOAIAnimCurveIndex		();
					~MOAIAnimCurveIndex		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			SetSample				( ZLIndex idx, ZLIndex value );
};

#endif
