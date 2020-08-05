// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIABSTRACTDSLINTERFACE_H
#define	MOAIABSTRACTDSLINTERFACE_H

#include <moai-core/MOAIAbstractDSLHandler.h>
#include <moai-core/MOAILua.h>

class MOAIAbstractDSLStream;

//================================================================//
// MOAIAbstractDSLInterface
//================================================================//
// TODO: doxygen
class MOAIAbstractDSLInterface :
	public virtual MOAILuaObject,
	public virtual MOAIAbstractDSLHandler {
protected:

	friend class MOAIAbstractDSLStream;

	//----------------------------------------------------------------//
	static int				_												( lua_State* L );

	//----------------------------------------------------------------//
	void					RegisterLuaAPI									( MOAILuaState& state );

	//----------------------------------------------------------------//
	virtual void			MOAIAbstractDSLInterface_InitializeBinding		( MOAIAbstractDSLStream& stream ) const = 0;
	virtual void			MOAIAbstractDSLInterface_RegisterLuaAPI			( MOAILuaState& state ) = 0;
	
public:

	//----------------------------------------------------------------//
	void					InitializeBinding								( MOAIAbstractDSLStream& stream ) const;
							MOAIAbstractDSLInterface						();
							~MOAIAbstractDSLInterface						();
	void					PushLuaAPI										( ZLTypeID apiTypeID, MOAILuaState& state );
};

#endif
