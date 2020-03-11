// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIHEADTRANSFORM_H
#define	MOAIHEADTRANSFORM_H

#include <moai-sim/MOAIAbstractBaseTransform.h>

#if MOAI_WITH_LIBGVR

//================================================================//
// MOAIHeadTransform
//================================================================//
// TODO: doxygen
class MOAIHeadTransform :
	public virtual MOAIAction,
	public virtual MOAIAbstractBaseTransform,
	public ZLAffine3D {
protected:

	//----------------------------------------------------------------//
	static int		_pauseTracking				( lua_State* L );
	static int		_recenterTracking			( lua_State* L );

	//----------------------------------------------------------------//
	void			_RegisterLuaClass									( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs									( RTTIVisitorHistory& history, MOAILuaState& state );
	void			MOAIAbstractBaseTransform_BuildLocalToWorldMtx		( ZLAffine3D& localToWorldMtx );
	bool			MOAIAction_IsDone									();
	void			MOAIAction_Update									( double step );

public:

	DECL_LUA_FACTORY ( MOAIHeadTransform )
	
	//----------------------------------------------------------------//
					MOAIHeadTransform			();
					~MOAIHeadTransform			();
	
};

#endif
#endif
