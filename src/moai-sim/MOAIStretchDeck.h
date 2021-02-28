// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAISTRETCHDECK_H
#define	MOAISTRETCHDECK_H

#include <moai-sim/MOAIDeck.h>

//================================================================//
// MOAIStretchDeck
//================================================================//
// TODO: doxygen
class MOAIStretchDeck :
	public virtual MOAIDeck {
protected:

	MOAI_LUA_OBJECT_VISITOR_FRIEND

	float			mStretchFactor;

	//----------------------------------------------------------------//
	static int		_setStretchFactor						( lua_State* L );
	
	//----------------------------------------------------------------//
	void			_RegisterLuaClass						( RTTIVisitorHistory& history, MOAILuaState& state );
	void			_RegisterLuaFuncs						( RTTIVisitorHistory& history, MOAILuaState& state );
	
public:
		
	//----------------------------------------------------------------//
	ZLVec3D			BindStretchVertexTransform				() const;
					MOAIStretchDeck							( ZLContext& context );
					~MOAIStretchDeck						();
};

#endif
